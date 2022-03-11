#include "Canvas.h"
#include "LineAlgorithm.h"
#include <qpainter.h>
#include <cassert>

static const int canvasSize = 500;

Canvas::Line::Line(const QLine &pos, LineColor clr, LineAlgorithm alg) {
    position = pos;
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
            algorithm = algorithmBrezenhamMod;
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
    image(canvasSize - 2, canvasSize - 2, QImage::Format::Format_RGB888) {
    setFixedSize(canvasSize, canvasSize);
    imageBackgroundColor = QColor(255, 255, 255);
    imageBorderColor = QColor(127, 127, 127);
    imageClear();
}

void Canvas::addLine(QLine line, LineColor color, LineAlgorithm alg) {
    Line l(line, color, alg);

    if (alg != LineAlgorithm::Library)
        imageAddLine(l);

    lines.push_back(l);
    update();
}

void Canvas::undo() {
    imageClear();
    lines.pop_back();
    for (const auto &line : lines) {
        if (line.algorithm)
            imageAddLine(line);
    }
    update();
}

void Canvas::clear() {
    lines.clear();
    imageClear();
    update();
}

void Canvas::paintEvent(QPaintEvent *e) {
    QPainter qp;
    qp.begin(this);
    qp.setPen(imageBorderColor);
    qp.drawRect(0, 0, canvasSize - 1, canvasSize - 1);
    qp.drawImage(1, 1, image);

    for (const auto &line : lines) {
        if (!line.algorithm)
            drawLine(qp, line);
    }
    
    qp.end();
}

void Canvas::drawLine(QPainter &qp, const Line &line) {
    assert(!line.algorithm);
    qp.setPen(line.color);
    qp.drawLine(line.position);
}

void Canvas::imageAddLine(const Line &line) {
    assert(line.algorithm);
    line.algorithm(image, line.position, line.color);
}

void Canvas::imageClear() {
    image.fill(imageBackgroundColor);
}
