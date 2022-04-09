#include "canvas.h"
#include "algorithm.h"
#include <qpainter.h>
#include <cassert>
#include <functional>
#include <qimage.h>

Canvas::Canvas(QWidget *parent)
    : QOpenGLWidget(parent),
    pixmap(canvasSize - 2, canvasSize - 2) {
    setFixedSize(canvasSize, canvasSize);
    imageBackgroundColor = QColor(255, 255, 255);
    imageBorderColor = QColor(127, 127, 127);
    pixmap.fill(imageBackgroundColor);
}

void Canvas::clear() {
    pixmap.fill(imageBackgroundColor);
    update();
}

void Canvas::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter qp;
    qp.begin(this);
    qp.setPen(imageBorderColor);
    qp.drawRect(0, 0, canvasSize - 1, canvasSize - 1);
    qp.drawPixmap(1, 1, pixmap);
    qp.end();
}

static QColor fromColor(Color clr) {
    QColor result;

    switch (clr) {
        case Color::black:
            result = QColor(0, 0, 0);
            break;
        case Color::red:
            result = QColor(255, 0, 0);
            break;
        case Color::green:
            result = QColor(0, 255, 0);
            break;
        case Color::blue:
            result = QColor(0, 0, 255);
            break;
        case Color::white:
            result = QColor(255, 255, 255);
            break;
    }

    return result;
}

void Canvas::addEllipse(Ellipse ellipse, Color color, Algorithm algorithm) {
    drawEllipse(ellipse, fromColor(color), algorithm);
}

void Canvas::addSpectrum(Ellipse begin, Ellipse end, int amount, Color color, Algorithm algorithm) {
    QColor clr = fromColor(color);
    Ellipse diff = (end - begin) / (amount - 1.);

    for (int i = 0; i < amount; i++) {
        drawEllipse(begin, clr, algorithm);
        begin = begin + diff;
    }
}

static QColor blendColor(QColor main, QColor bg, double r) {
    assert(r <= 1.0);
    const double rr = 1.0 - r;
    return QColor(
                main.red() * r - bg.red() * rr,
                main.green() * r - bg.green() * rr,
                main.blue() * r - bg.blue() * rr
                );
}

void Canvas::drawEllipse(Ellipse ellipse, QColor color, Algorithm algorithm) {
    QImage image(pixmap.toImage());
    auto plot = [&image, color](int x, int y, double i){
        QColor blendedColor = blendColor(color, image.pixelColor(x, y), i);
        image.setPixelColor(x, y, blendedColor);
    };

    switch (algorithm) {
        case Algorithm::brezenham:
            algBrezenham(ellipse.cx, ellipse.cy, ellipse.rx, ellipse.ry, plot);
            break;
        case Algorithm::canonicalEquation:
            algCanonicalEq(ellipse.cx, ellipse.cy, ellipse.rx, ellipse.ry, plot);
            break;
        case Algorithm::parametricalEquation:
            algParametricalEq(ellipse.cx, ellipse.cy, ellipse.rx, ellipse.ry, plot);
            break;
        case Algorithm::averagePoint:
            algAveragePoint(ellipse.cx, ellipse.cy, ellipse.rx, ellipse.ry, plot);
            break;
        default:
            throw "biba";
    }

    pixmap = QPixmap::fromImage(image);
    update();
}
