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
    void paintEvent(QPaintEvent *event);
public:
    explicit GameArea(QWidget *parent = 0);

    struct Trace {
        uint playerId, x, y, angle, thickness, r, g, b;
        Trace(uint _playerId, uint _x, uint _y, uint _angle, uint _thickness, uint _r, uint _g, uint _b)
        {
            playerId = _playerId; x = _x; y = _y; angle = _angle; thickness = _thickness; r = _r; g = _g; b = _b;
        }
    };
signals:

private slots:
    void clientPlayerTraceInitialized(uint playerId, uint x, uint y, uint angle, uint thickness, uint r, uint g, uint b);
    void clientPlayerMoved(uint playerId, uint x, uint y, qreal angle);
    void clientConnected(QLazerDrivePlayer player);
private:
    QList<Trace> m_traceList;
};

#endif // GAMEAREA_H
