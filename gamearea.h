#ifndef GAMEAREA_H
#define GAMEAREA_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QLazerDrivePlayer>
#include <QLazerDriveClient>
#include "usernamedialog.h"

class GameArea : public QOpenGLWidget
{
    Q_OBJECT
    QLazerDriveClient *m_pClient;
    UsernameDialog *m_pUsernameDialog;
    QTimer m_RefreshTimer;
    uint m_playerId, m_avatarId;
    void paintEvent(QPaintEvent *event);
public:
    explicit GameArea(QWidget *parent = 0);
    ~GameArea();

    struct Trace {
        uint playerId, x, y, angle, thickness, r, g, b;
        Trace(uint _playerId, uint _x, uint _y, uint _angle, uint _thickness, uint _r, uint _g, uint _b)
        {
            playerId = _playerId; x = _x; y = _y; angle = _angle; thickness = _thickness; r = _r; g = _g; b = _b;
        }
    };

    struct CacheEntry {
        QLazerDrivePlayer player;
        uint thickness;
        CacheEntry(QLazerDrivePlayer _player = QLazerDrivePlayer(), uint _thickness = 50)
        {
            player = _player;
            thickness = _thickness;
        }
    };
signals:

private slots:
    void clientPlayerTraceInitialized(uint playerId, uint x, uint y, uint angle, uint thickness, uint r, uint g, uint b);
    void clientPlayerMoved(uint playerId, uint x, uint y, qreal angle);
    void clientConnected(QLazerDrivePlayer player);
    void clientPlayerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias);
    void clientPlayerLeftTheGame(QLazerDrivePlayer player, bool isAlias);
    void clientExistingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y);
    void clientPlayerThicknessChanged(uint playerId, uint thickness);
private:
    QList<Trace> *m_pTraceList;
    QMap<uint, CacheEntry> *m_pPlayerCache;
};

#endif // GAMEAREA_H
