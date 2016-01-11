#include "gamearea.h"

GameArea::GameArea(QWidget *parent) : QOpenGLWidget(parent)
{
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);

    m_pTraceList = new QList<Trace>();
    m_pPlayerCache = new QMap<uint, CacheEntry>();
    m_playerId = m_avatarId = 0;

    m_pUsernameDialog = new UsernameDialog(this);

    m_pClient = new QLazerDriveClient(this);
    connect(m_pClient, SIGNAL(connected(QLazerDrivePlayer)), this, SLOT(clientConnected(QLazerDrivePlayer)));
    connect(m_pClient, SIGNAL(nextColorReceived(uint,uint,uint)), m_pUsernameDialog, SLOT(setColor(uint, uint, uint)));
    connect(m_pClient, SIGNAL(playerTraceInitialized(uint,uint,uint,uint,uint,uint,uint,uint)), this, SLOT(clientPlayerTraceInitialized(uint,uint,uint,uint,uint,uint,uint,uint)));
    connect(m_pClient, SIGNAL(playerMoved(uint,uint,uint,qreal)), this, SLOT(clientPlayerMoved(uint,uint,uint,qreal)));
    connect(m_pClient, SIGNAL(playerEnteredTheGame(QLazerDrivePlayer,bool,bool)), this, SLOT(clientPlayerEnteredTheGame(QLazerDrivePlayer,bool,bool)));
    connect(m_pClient, SIGNAL(playerLeftTheGame(QLazerDrivePlayer,bool)), this, SLOT(clientPlayerLeftTheGame(QLazerDrivePlayer,bool)));
    connect(m_pClient, SIGNAL(existingPlayerInitialized(QLazerDrivePlayer,uint,uint)), this, SLOT(clientExistingPlayerInitialized(QLazerDrivePlayer,uint,uint)));
    connect(m_pClient, SIGNAL(playerThicknessChanged(uint,uint)), this, SLOT(clientPlayerThicknessChanged(uint,uint)));
    connect(m_pClient, SIGNAL(mapErased()), this, SLOT(clientMapErased()));
    connect(m_pClient, SIGNAL(playerPrintChanged(uint,bool)), this, SLOT(clientPlayerPrintChanged(uint,bool)));
    connect(m_pClient, SIGNAL(playerImuneChanged(uint,bool)), this, SLOT(clientPlayerImuneChanged(uint,bool)));
    connect(m_pClient, SIGNAL(playerReversed(uint,bool)), this, SLOT(clientPlayerReversed(uint,bool)));

    connect(m_pUsernameDialog, SIGNAL(completed(QString)), m_pClient, SLOT(enterTheGame(QString)));
    connect(m_pUsernameDialog, SIGNAL(nextColor()), m_pClient, SLOT(nextColor()));

    m_pClient->connectToServer("one.eu.lazerdrive.io");

    connect(&m_RefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    // 30 fps
    m_RefreshTimer.start(1000 / 30);
}

GameArea::~GameArea()
{
    delete m_pTraceList;
    delete m_pPlayerCache;
}

void GameArea::clientConnected(QLazerDrivePlayer player)
{
    m_pUsernameDialog->setUsername(player.name());
    m_pUsernameDialog->setColor(player.r(), player.g(), player.b());
    m_pUsernameDialog->show();
}

void GameArea::clientPlayerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias)
{
    m_pPlayerCache->insert(player.id(), CacheEntry(player));
    if (isMyself) {
        if (isAlias) {
            m_avatarId = player.id();
        } else {
            m_playerId = player.id();
        }
    }
}

void GameArea::clientPlayerLeftTheGame(QLazerDrivePlayer player, bool isAlias)
{
    Q_UNUSED(isAlias);
    m_pPlayerCache->remove(player.id());
}

void GameArea::clientExistingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    m_pPlayerCache->insert(player.id(), CacheEntry(player, x, y));
}

void GameArea::clientPlayerThicknessChanged(uint playerId, uint thickness)
{
    CacheEntry cached = m_pPlayerCache->value(playerId);
    cached.thickness = thickness;
    m_pPlayerCache->insert(playerId, cached);
}

void GameArea::clientMapErased()
{
    m_pTraceList->clear();
}

void GameArea::clientPlayerPrintChanged(uint playerId, bool isPrinting)
{
    CacheEntry cached = m_pPlayerCache->value(playerId);
    cached.isPrinting = isPrinting;
    m_pPlayerCache->insert(playerId, cached);
}

void GameArea::clientPlayerImuneChanged(uint playerId, bool isImune)
{
    CacheEntry cached = m_pPlayerCache->value(playerId);
    cached.isImune = isImune;
    m_pPlayerCache->insert(playerId, cached);
}

void GameArea::clientPlayerReversed(uint playerId, bool isReversed)
{
    CacheEntry cached = m_pPlayerCache->value(playerId);
    cached.isReversed = isReversed;
    m_pPlayerCache->insert(playerId, cached);
}

void GameArea::clientPlayerTraceInitialized(uint playerId, uint x, uint y, uint angle, uint thickness, uint r, uint g, uint b)
{
    m_pTraceList->append(Trace(playerId, x, y, angle, thickness, r, g, b));
}

void GameArea::clientPlayerMoved(uint playerId, uint x, uint y, qreal angle)
{
    CacheEntry cached = m_pPlayerCache->value(playerId);
    cached.x = x;
    cached.y = y;
    m_pPlayerCache->insert(playerId, cached);
    if (cached.isImune || !cached.isPrinting)
        return;
    m_pTraceList->append(Trace(playerId, x, y, angle, cached.thickness, cached.player.r(), cached.player.g(), cached.player.b()));
}

void GameArea::paintEvent(QPaintEvent *event)
{
    int roomWidth = 12000, roomHeight = 12000;
    int width = this->geometry().width();
    int height = this->geometry().height();
    double factor = 0.1;

    CacheEntry me = m_pPlayerCache->value(m_playerId);
    int playerX = me.x == 0 && me.y == 0 ? roomWidth/2 : me.x;
    int playerY = me.x == 0 && me.y == 0 ? roomHeight/2 : me.y;

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), QColor(2, 10, 20));

    int diffLeftBorder = 0 - playerX;
    int diffTopBorder = 0 - playerY;
    int diffRightBorder = roomWidth - playerX;
    int diffBottomBorder = roomHeight - playerY;
    int leftBorderScreenUnit = (width/2) + (diffLeftBorder * factor);
    int topBorderScreenUnit = (height/2) + (diffTopBorder * factor);
    int rightBorderScreenUnit = (width/2) + (diffRightBorder * factor);
    int bottomBorderScreenUnit = (height/2) + (diffBottomBorder * factor);
    painter.fillRect(QRect(qMax(leftBorderScreenUnit, 0), qMax(topBorderScreenUnit, 0), rightBorderScreenUnit, bottomBorderScreenUnit), QColor(4, 20, 40));

    painter.setPen(QColor(8, 40, 80));
    for (int x=leftBorderScreenUnit; x<rightBorderScreenUnit; x+=100) {
        if (x<0)
            continue;
        painter.drawLine(x, topBorderScreenUnit, x, bottomBorderScreenUnit);
    }
    for (int y=topBorderScreenUnit; y<bottomBorderScreenUnit; y+=100) {
        if (y<0)
            continue;
        painter.drawLine(leftBorderScreenUnit, y, rightBorderScreenUnit, y);
    }

    painter.setPen(Qt::NoPen);
    foreach (Trace trace, *m_pTraceList) {
        int xDiff = trace.x - playerX;
        int yDiff = trace.y - playerY;
        int thicknessScaled = trace.thickness * factor;

        QPoint point((width / 2) + (xDiff * factor), (height / 2) + (yDiff * factor));
        painter.setBrush(QBrush(QColor(trace.r, trace.g, trace.b)));
        painter.drawEllipse(point, thicknessScaled, thicknessScaled);
    }

    foreach (CacheEntry cached, *m_pPlayerCache) {
        int xDiff = cached.x - playerX;
        int yDiff = cached.y - playerY;
        int thicknessScaled = cached.thickness * factor;

        QPoint point((width / 2) + (xDiff * factor), (height / 2) + (yDiff * factor));
        painter.setBrush(QBrush(QColor(cached.player.r(), cached.player.g(), cached.player.b())));
        painter.drawEllipse(point, thicknessScaled, thicknessScaled);
    }

    painter.end();
}

void GameArea::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    if (event->key() == Qt::Key_Right) {
        m_pClient->pressArrow(QLazerDrivePlayer::RightPressed);
    }
    else if (event->key() == Qt::Key_Left) {
        m_pClient->pressArrow(QLazerDrivePlayer::LeftPressed);
    }
}

void GameArea::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left) {
        m_pClient->releaseArrow();
    }
}
