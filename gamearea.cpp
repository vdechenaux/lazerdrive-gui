#include "gamearea.h"

GameArea::GameArea(QWidget *parent) : QOpenGLWidget(parent)
{
    setMinimumSize(800, 600);

    m_pUsernameDialog = new UsernameDialog(this);

    m_pClient = new QLazerDriveClient(this);
    connect(m_pClient, SIGNAL(connected(QLazerDrivePlayer)), this, SLOT(clientConnected(QLazerDrivePlayer)));
    connect(m_pClient, SIGNAL(nextColorReceived(uint,uint,uint)), m_pUsernameDialog, SLOT(setColor(uint, uint, uint)));
    connect(m_pClient, SIGNAL(playerTraceInitialized(uint,uint,uint,uint,uint,uint,uint,uint)), this, SLOT(clientPlayerTraceInitialized(uint,uint,uint,uint,uint,uint,uint,uint)));
    connect(m_pClient, SIGNAL(playerMoved(uint,uint,uint,qreal)), this, SLOT(clientPlayerMoved(uint,uint,uint,qreal)));

    connect(m_pUsernameDialog, SIGNAL(completed(QString)), m_pClient, SLOT(enterTheGame(QString)));
    connect(m_pUsernameDialog, SIGNAL(nextColor()), m_pClient, SLOT(nextColor()));

    m_pClient->connectToServer("one.eu.lazerdrive.io");

    connect(&m_RefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    // 30 fps
    m_RefreshTimer.start(1000 / 30);
}

void GameArea::clientConnected(QLazerDrivePlayer player)
{
    m_pUsernameDialog->setUsername(player.name());
    m_pUsernameDialog->setColor(player.r(), player.g(), player.b());
    m_pUsernameDialog->show();
}

void GameArea::clientPlayerTraceInitialized(uint playerId, uint x, uint y, uint angle, uint thickness, uint r, uint g, uint b)
{
    m_traceList.append(Trace(playerId, x, y, angle, thickness, r, g, b));
}

void GameArea::clientPlayerMoved(uint playerId, uint x, uint y, qreal angle)
{
    m_traceList.append(Trace(playerId, x, y, angle, 50, 255, 0, 0)); // TODO: load color + thickness from cache
}

void GameArea::paintEvent(QPaintEvent *event)
{
    int roomWidth = 12000, roomHeight = 12000;
    int width = this->geometry().width();
    int height = this->geometry().height();
    double factor = 0.1;

    int playerX = roomWidth / 2;
    int playerY = roomHeight / 2;

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), QColor(2, 10, 20));

    int diffLeftBorder = 0 - playerX;
    int diffTopBorder = 0 - playerY;
    int diffRightBorder = roomWidth - playerX;
    int diffBottomBorder = roomHeight - playerY;
    int leftBorderScreenUnit = qMax((width/2) + (diffLeftBorder * factor), 0.0);
    int topBorderScreenUnit = qMax((height/2) + (diffTopBorder * factor), 0.0);
    int rightBorderScreenUnit = (width/2) + (diffRightBorder * factor);
    int bottomBorderScreenUnit = (height/2) + (diffBottomBorder * factor);
    painter.fillRect(QRect(leftBorderScreenUnit, topBorderScreenUnit, rightBorderScreenUnit, bottomBorderScreenUnit), QColor(4, 20, 40));

    painter.setPen(QColor(8, 40, 80));
    for (int x=leftBorderScreenUnit; x<rightBorderScreenUnit; x+=100) {
        painter.drawLine(x, topBorderScreenUnit, x, bottomBorderScreenUnit);
    }
    for (int y=topBorderScreenUnit; y<bottomBorderScreenUnit; y+=100) {
        painter.drawLine(leftBorderScreenUnit, y, rightBorderScreenUnit, y);
    }

    painter.setPen(Qt::NoPen);
    foreach (Trace trace, m_traceList) {
        int xDiff = trace.x - playerX;
        int yDiff = trace.y - playerY;
        int thicknessScaled = trace.thickness * factor;

        QPoint point((width / 2) + (xDiff * factor), (height / 2) + (yDiff * factor));
        painter.setBrush(QBrush(QColor(trace.r, trace.g, trace.b)));
        painter.drawEllipse(point, thicknessScaled, thicknessScaled);
    }

    painter.end();
}
