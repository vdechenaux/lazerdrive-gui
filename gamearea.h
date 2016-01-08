#ifndef GAMEAREA_H
#define GAMEAREA_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>

class GameArea : public QOpenGLWidget
{
    Q_OBJECT
    QTimer m_RefreshTimer;
    void paintEvent(QPaintEvent *event);
public:
    explicit GameArea(QWidget *parent = 0);

signals:

public slots:
};

#endif // GAMEAREA_H
