#include "gamearea.h"

GameArea::GameArea(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_RefreshTimer, SIGNAL(timeout()), this, SLOT(update()));
    // 30 fps
    m_RefreshTimer.start(1000 / 30);
}

void GameArea::paintEvent(QPaintEvent *event)
{
    int width = this->width();
    int height = this->height();

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), QColor(4, 20, 40));

    painter.setPen(QColor(8, 40, 80));
    for (int x=0; x<width; x+=100) {
        painter.drawLine(x, 0, x, height);
    }
    for (int y=0; y<height; y+=100) {
        painter.drawLine(0, y, width, y);
    }

    painter.end();
}
