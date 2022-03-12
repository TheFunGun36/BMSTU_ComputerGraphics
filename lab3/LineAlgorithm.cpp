#include "LineAlgorithm.h"

void algorithmDDA(QImage &pixmap, const QLine &line, const QColor &color) {
    int x1 = line.x1();
    int y1 = line.y1();
    int x2 = line.x2();
    int y2 = line.y2();

    qreal dx = x2 - x1;
    qreal dy = y2 - y1;

    int length = qMax(abs(dx), abs(dy));

    dx /= length;
    dy /= length;

    qreal x = x1;
    qreal y = y1;

    for (int i = 0; i <= length; i++) {
        pixmap.setPixel(x, y, color.rgba());
        x += dx;
        y += dy;
    }
}

void algorithmBrezenhamFloat(QImage &pixmap, const QLine &line, const QColor &color) {}
void algorithmBrezenhamInt(QImage &pixmap, const QLine &line, const QColor &color) {}
void algorithmBrezenhamMod(QImage &pixmap, const QLine &line, const QColor &color) {}
void algorithmWu(QImage &pixmap, const QLine &line, const QColor &color) {}
