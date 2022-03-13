#include "Canvas.h"
#include "LineAlgorithm.h"
#include <qpainter.h>
#include <cassert>

static const int canvasSize = 500;

Canvas::Sun::Sun(const QLine &base, LineColor clr, LineAlgorithm alg, int amount) {
    lines.push_back(base);

    if (amount > 1) {
        qreal angle = 2 * M_PI / amount;
        QLineF line = base;
        qreal dx = base.p2().x() - base.p1().x();
        qreal dy = base.p2().y() - base.p1().y();
        for (int i = 1; i < amount; i++) {
            qreal cosa = qCos(i * angle);
            qreal sina = qSin(i * angle);
            qreal x = base.p1().x() + dx * cosa + dy * sina;
            qreal y = base.p1().y() - dx * sina + dy * cosa;
            line.setP2(QPointF(x, y));
            lines.push_back(line.toLine());
        }
    }

    algorithm = nullptr;

    switch (alg) {
        case LineAlgorithm::digitalDiffAnalyzer:
            algorithm = algorithmDDA;
            break;
        case LineAlgorithm::brezenhamFloat:
            algorithm = algorithmBrezenhamFloat;
            break;
        case LineAlgorithm::brezenhamInt:
            algorithm = algorithmBrezenhamInt;
            break;
        case LineAlgorithm::brezenhamMod:
            algorithm = algorithmBrezenhamAntialias;
            break;
        case LineAlgorithm::Wu:
            algorithm = algorithmWu;
            break;
    }

    switch (clr) {
        case LineColor::black:
            color = QColor(0, 0, 0);
            break;
        case LineColor::white:
            color = QColor(255, 255, 255);
            break;
        case LineColor::red:
            color = QColor(255, 0, 0);
            break;
        case LineColor::green:
            color = QColor(0, 255, 0);
            break;
        case LineColor::blue:
            color = QColor(0, 0, 255);
            break;
        default:
            color = QColor(127, 127, 127);
    }
}

Canvas::Canvas(QWidget *parent)
    : QOpenGLWidget(parent),
    pixmap(canvasSize - 2, canvasSize - 2) {
    setFixedSize(canvasSize, canvasSize);
    imageBackgroundColor = QColor(255, 255, 255);
    imageBorderColor = QColor(127, 127, 127);
    pixmap.fill(imageBackgroundColor);
}

void Canvas::addSun(QLine line, LineColor color, LineAlgorithm alg, int lines) {
    Sun s(line, color, alg, lines);
    drawSun(s);
    suns.push_back(s);
    update();
}

void Canvas::undo() {
    if (!suns.isEmpty()) {
        pixmap.fill(imageBackgroundColor);
        suns.pop_back();
        for (const auto &sun : suns)
            drawSun(sun);
        update();
    }
}

void Canvas::clear() {
    if (!suns.isEmpty()) {
        suns.clear();
        pixmap.fill(imageBackgroundColor);
        update();
    }
}

void Canvas::paintEvent(QPaintEvent *e) {
    QPainter qp;
    qp.begin(this);
    qp.setPen(imageBorderColor);
    qp.drawRect(0, 0, canvasSize - 1, canvasSize - 1);
    qp.drawPixmap(1, 1, pixmap);
    qp.end();
}

void Canvas::drawSun(const Sun &sun) {
    if (sun.algorithm) {
        QImage image = pixmap.toImage();
        for (const QLine line : sun.lines)
            sun.algorithm(image, line, sun.color);
        pixmap = QPixmap::fromImage(image);
    }
    else {
        QPainter qp(&pixmap);
        qp.setPen(sun.color);
        for (const QLine line : sun.lines)
            qp.drawLine(line);
    }
}
