#include "LineAlgorithm.h"

template <typename T>
static inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

void algorithmDDA(QImage &pixmap, const QLine &line, const QColor &color) {
    // Здесь qreal - тип данных действительного числа
    qreal dx = line.x2() - line.x1();
    qreal dy = line.y2() - line.y1();

    int length = qMax(abs(dx), abs(dy));

    dx /= length;
    dy /= length;

    qreal x = line.x1();
    qreal y = line.y1();

    for (int i = 0; i <= length; i++) {
        pixmap.setPixel(int(x), int(y), color.rgba());
        x += dx;
        y += dy;
    }
}

void algorithmBrezenhamFloat(QImage &pixmap, const QLine &line, const QColor &color) {
    QPoint d(line.x2() - line.x1(), line.y2() - line.y1());
    QPoint s(sign(d.x()), sign(d.y()));
    QPoint p(line.x1(), line.y1());

    bool swapped = false;
    if (d.y() > d.x()) {
        d = d.transposed();
        s = s.transposed();
        p = p.transposed();
        swapped = true;
    }

    qreal e = qreal(d.y()) / d.x() - 0.5;

    for (int i = 0; i <= d.x(); i++) {
        pixmap.setPixel(swapped ? p.transposed() : p, color.rgba());

        while (e >= 0) {
            p.setY(p.y() + s.y());
            e -= 1;
        }

        p.setX(p.x() + s.x());
        e += qreal(d.y()) / d.x();
    }
}

void algorithmBrezenhamInt(QImage &pixmap, const QLine &line, const QColor &color) {
    QPoint d(line.x2() - line.x1(), line.y2() - line.y1());
    QPoint s(sign(d.x()), sign(d.y()));
    QPoint p(line.x1(), line.y1());

    bool swapped = false;
    if (d.y() > d.x()) {
        d = d.transposed();
        s = s.transposed();
        p = p.transposed();
        swapped = true;
    }

    int e = 2 * d.y() - d.x();

    for (int i = 0; i <= d.x(); i++) {
        pixmap.setPixel(swapped ? p.transposed() : p, color.rgba());

        while (e >= 0) {
            p.setY(p.y() + s.y());
            e -= 2 * d.x();
        }

        p.setX(p.x() + s.x());
        e += 2 * d.y();
    }
}

void algorithmBrezenhamMod(QImage &pixmap, const QLine &line, const QColor &color) {}
void algorithmWu(QImage &pixmap, const QLine &line, const QColor &color) {}
