#include "LineAlgorithm.h"
#include <chrono>

template <typename T>
static inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

static inline QColor colorBlend(QColor a, QColor b, double ratio) {
    return QColor(
        a.red() * ratio + b.red() * (1 - ratio),
        a.green() * ratio + b.green() * (1 - ratio),
        a.blue() * ratio + b.blue() * (1 - ratio)
    );
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
    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    if (deltaX > deltaY) {
        qreal error = qreal(deltaY) / deltaX - 0.5;
        while (x != line.x2()) {
            pixmap.setPixel(x, y, color.rgba());

            if (error >= 0) {
                y += stepY;
                error -= 1.;
            }

            error += qreal(deltaY) / deltaX;
            x += stepX;
        }
    }
    else {
        qreal error = qreal(deltaX) / deltaY - 0.5;
        while (y != line.y2()) {
            pixmap.setPixel(x, y, color.rgba());

            if (error >= 0) {
                x += stepX;
                error -= 1.0;
            }

            error += qreal(deltaX) / deltaY;
            y += stepY;
        }
    }
}

void algorithmBrezenhamInt(QImage &pixmap, const QLine &line, const QColor &color) {
    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    if (deltaX > deltaY) {
        int error = 2 * deltaY - deltaX;
        while (x != line.x2()) {
            pixmap.setPixel(x, y, color.rgba());

            if (error >= 0) {
                y += stepY;
                error -= 2 * deltaX;
            }

            error += 2 * deltaY;
            x += stepX;
        }
    }
    else {
        int error = 2 * deltaX - deltaY;
        while (y != line.y2()) {
            pixmap.setPixel(x, y, color.rgba());

            if (error >= 0) {
                x += stepX;
                error -= 2 * deltaY;
            }

            error += 2 * deltaX;
            y += stepY;
        }
    }
}

void algorithmBrezenhamAntialias(QImage &pixmap, const QLine &line, const QColor &color) {
    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    qreal maxI = 1.;
    qreal error = maxI / 2.;

    if (deltaX > deltaY) {
        qreal m = qreal(maxI) * deltaY / deltaX;
        qreal w = maxI - m;

        while (x != line.x2()) {
            pixmap.setPixelColor(x, y, colorBlend(color, pixmap.pixelColor(x, y), maxI - abs(error)));

            x += stepX;

            if (error <= w) {
                error += m;
            }
            else {
                y += stepY;
                error -= w;
            }
        }
    }
    else {
        qreal m = qreal(maxI) * deltaX / deltaY;
        qreal w = maxI - m;

        while (y != line.y2()) {
            pixmap.setPixelColor(x, y, colorBlend(color, pixmap.pixelColor(x, y), maxI - error));

            y += stepY;

            if (error <= w) {
                error += m;
            }
            else {
                x += stepX;
                error -= w;
            }
        }
    }
}

void algorithmWu(QImage &pixmap, const QLine &line, const QColor &color) {
    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    qreal maxI = 1.;
    qreal error = maxI / 2.;

    if (deltaX > deltaY) {
        qreal m = qreal(maxI) * deltaY / deltaX;
        qreal w = maxI - m;

        while (x != line.x2()) {
            QColor clr = colorBlend(color, pixmap.pixelColor(x, y), maxI - error);
            pixmap.setPixelColor(x, y, clr);
            clr = colorBlend(color, pixmap.pixelColor(x, y + stepY), error);
            pixmap.setPixelColor(x, y + stepY, clr);

            x += stepX;

            if (error <= w) {
                error += m;
            }
            else {
                y += stepY;
                error -= w;
            }
        }
    }
    else {
        qreal m = qreal(maxI) * deltaX / deltaY;
        qreal w = maxI - m;

        while (y != line.y2()) {
            QColor clr = colorBlend(color, pixmap.pixelColor(x, y), maxI - error);
            pixmap.setPixelColor(x, y, clr);
            clr = colorBlend(color, pixmap.pixelColor(x + stepX, y), error);
            pixmap.setPixelColor(x + stepX, y, clr);

            y += stepY;

            if (error <= w) {
                error += m;
            }
            else {
                x += stepX;
                error -= w;
            }
        }
    }
}

int countLaddersDDA(const QLine &line) {
    int result = 0;

    qreal dx = line.x2() - line.x1();
    qreal dy = line.y2() - line.y1();

    int length = qMax(abs(dx), abs(dy));

    dx /= length;
    dy /= length;

    qreal x = line.x1();
    qreal y = line.y1();

    int prevX = x;
    int prevY = y;

    for (int i = 0; i <= length; i++) {
        x += dx;
        y += dy;

        result += prevX != int(x) && prevY != int(y);
        prevX = int(x);
        prevY = int(y);
    }

    return result;
}

int countLaddersBrezenhamFloat(const QLine &line) {
    int result = 0;

    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    if (deltaX > deltaY) {
        qreal error = qreal(deltaY) / deltaX - 0.5;
        while (x != line.x2()) {

            if (error >= 0) {
                y += stepY;
                error -= 1.;
                result++;
            }

            error += qreal(deltaY) / deltaX;
            x += stepX;
        }
    }
    else {
        qreal error = qreal(deltaX) / deltaY - 0.5;
        while (y != line.y2()) {

            if (error >= 0) {
                x += stepX;
                error -= 1.0;
                result++;
            }

            error += qreal(deltaX) / deltaY;
            y += stepY;
        }
    }

    return result;
}

int countLaddersBrezenhamInt(const QLine &line) {
    int result = 0;

    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    if (deltaX > deltaY) {
        int error = 2 * deltaY - deltaX;
        while (x != line.x2()) {
            if (error >= 0) {
                y += stepY;
                error -= 2 * deltaX;
                result++;
            }

            error += 2 * deltaY;
            x += stepX;
        }
    }
    else {
        int error = 2 * deltaX - deltaY;
        while (y != line.y2()) {
            if (error >= 0) {
                x += stepX;
                error -= 2 * deltaY;
                result++;
            }

            error += 2 * deltaX;
            y += stepY;
        }
    }

    return result;
}

int countLaddersBrezenhamAntialias(const QLine &line) {
    int result = 0;

    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    qreal maxI = 1.;
    qreal error = maxI / 2.;

    if (deltaX > deltaY) {
        qreal m = qreal(maxI) * deltaY / deltaX;
        qreal w = maxI - m;

        while (x != line.x2()) {
            x += stepX;

            if (error <= w) {
                error += m;
            }
            else {
                y += stepY;
                result++;
                error -= w;
            }
        }
    }
    else {
        qreal m = qreal(maxI) * deltaX / deltaY;
        qreal w = maxI - m;

        while (y != line.y2()) {
            y += stepY;

            if (error <= w) {
                error += m;
            }
            else {
                x += stepX;
                result++;
                error -= w;
            }
        }
    }

    return result;
}

int countLaddersWu(const QLine &line) {
    int result = 0;

    int deltaX = line.x2() - line.x1();
    int deltaY = line.y2() - line.y1();

    int stepX = sign(deltaX);
    int stepY = sign(deltaY);

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    int x = line.x1();
    int y = line.y1();

    qreal maxI = 1.;
    qreal error = maxI / 2.;

    if (deltaX > deltaY) {
        qreal m = qreal(maxI) * deltaY / deltaX;
        qreal w = maxI - m;

        while (x != line.x2()) {
            x += stepX;

            if (error <= w) {
                error += m;
            }
            else {
                y += stepY;
                error -= w;
                result++;
            }
        }
    }
    else {
        qreal m = qreal(maxI) * deltaX / deltaY;
        qreal w = maxI - m;

        while (y != line.y2()) {
            y += stepY;

            if (error <= w) {
                error += m;
            }
            else {
                x += stepX;
                error -= w;
                result++;
            }
        }
    }

    return result;
}

long long testAlgorithm(int iterations, int lineLength, void (*algorithm)(QImage &, const QLine &, const QColor &)) {
    using namespace std::chrono;
    const qreal sqrt2 = sqrt(2);
    QImage image(lineLength / sqrt2 + 1, lineLength / sqrt2 + 1, QImage::Format_RGB32);
    QLine line(0, 0, lineLength / sqrt2, lineLength / sqrt2);

    for (int i = 0; i < iterations / 2; i++) {
        image.fill(Qt::black);
        algorithm(image, line, QColor(255, 255, 255));
    }

    long long result = 0;
    for (int i = 0; i < iterations; i++) {
        image.fill(Qt::black);

        auto begin = high_resolution_clock::now();
        algorithm(image, line, QColor(255, 255, 255));
        auto end = high_resolution_clock::now();

        result += (end - begin).count();
    }

    return result;
}