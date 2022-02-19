#include "Canvas.h"
#include <qpainter.h>

Canvas::Canvas(QWidget *parent)
    : scene(this), QGraphicsView(parent) {
    setScene(&scene);
    margin = 0.15;
    minCellSize = 20;
    maxCellSize = 210;
    scale = 1;
    colorBackground = QColor(255, 255, 255);
    colorAxes = QColor(0, 100, 150);
    colorGrid = QColor(0, 200, 200);
    colorGridAccent = QColor(0, 50, 150);

    scene.setBackgroundBrush(colorBackground);

    //drawAxes();
    //drawGrid();
}

int Canvas::getScale() {
    return scale;
}

void Canvas::windowResized() {
    recalculateScreenPosition();
}

void Canvas::recalculateScreenPosition() {
    /*
    double ratio = static_cast<double>(width()) / height();

    QPointF point1 = { tablePoints->item(0, 0)->text().toDouble(), tablePoints->item(0, 1)->text().toDouble() };
    QPointF point2 = { tablePoints->item(1, 0)->text().toDouble(), tablePoints->item(1, 1)->text().toDouble() };
    screenPosition = { point1, point2 };
    int pointsAmount = tablePoints->rowCount();

    for (QPointF point : points) {
        if (point.x() > screenPosition.right())
            screenPosition.setRight(point.x());
        else if (point.x() < screenPosition.left())
            screenPosition.setLeft(point.x());

        if (point.y() < screenPosition.top())
            screenPosition.setTop(point.y());
        else if (point.y() > screenPosition.bottom())
            screenPosition.setBottom(point.y());
    };

    if (qFuzzyIsNull(screenPosition.height()) || screenPosition.width() / screenPosition.height() > ratio) {
        screenPosition.setHeight(screenPosition.width() / ratio);
    }
    else {
        screenPosition.setWidth(ratio * screenPosition.height());
    }

    if (qFuzzyIsNull(screenPosition.width()) || qFuzzyIsNull(screenPosition.height())) {
        screenPosition.setWidth(width());
        screenPosition.setHeight(height());
    }

    QPointF marginValue = QPointF(margin * screenPosition.width(), margin * screenPosition.height());
    screenPosition.setLeft(screenPosition.left() - marginValue.x());
    screenPosition.setRight(screenPosition.right() + marginValue.x());
    screenPosition.setTop(screenPosition.top() - marginValue.y());
    screenPosition.setBottom(screenPosition.bottom() + marginValue.y());
    */
    
    screenPosition.setTopLeft(QPointF(-12, -12));
    screenPosition.setBottomRight(QPointF(12, 12));
}

/*
void Canvas::drawAxes() {
    QPen pen(QBrush(colorAxes), 2);
    QPointF center = mathToScreen(QPoint(0, 0));
    scene.addLine(QLineF(0, round(center.y()), width(), round(center.y())), pen);
    scene.addLine(QLineF(round(center.x()), 0, round(center.x()), height()), pen);
}

void Canvas::drawGrid(QPainter &qp) {
    QPointF center = mathToScreen(QPoint(0, 0));

    double delta = mathToScreen(QPoint(1, 0)).x() - center.x();

    scale = 1;
    while (delta < minCellSize) {
        delta *= 10;
        scale *= 10;
    }
    while (delta > maxCellSize) {
        delta /= 10;
        scale /= 10;
    }

    qp.setPen(colorGrid);
    for (double cur = center.x(); cur < width(); cur += delta)
        qp.drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.x(); cur > 0; cur -= delta)
        qp.drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.y(); cur < height(); cur += delta)
        qp.drawLine(0, round(cur), width(), round(cur));
    for (double cur = center.y(); cur > 0; cur -= delta)
        qp.drawLine(0, round(cur), width(), round(cur));

    delta *= 10;
    qp.setPen(colorGridAccent);
    for (double cur = center.x(); cur < width(); cur += delta)
        qp.drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.x(); cur > 0; cur -= delta)
        qp.drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.y(); cur < height(); cur += delta)
        qp.drawLine(0, round(cur), width(), round(cur));
    for (double cur = center.y(); cur > 0; cur -= delta)
        qp.drawLine(0, round(cur), width(), round(cur));
}

void Canvas::drawBackground(QPainter &qp) {
    qp.setBrush(colorBackground);
    qp.setPen(colorBackground);
    qp.drawRect(0, 0, width(), height());
}*/

QPointF Canvas::mathToScreen(QPointF point) const {
    //Перетаскиваем угол камеры к нужной позиции
    point -= screenPosition.topLeft();

    // Меняем масштаб
    point.setX(point.x() * width() / screenPosition.width());
    point.setY(point.y() * height() / screenPosition.height());

    // Инвертируем ось y
    point.setY(height() - point.y());

    return point;
}

QPointF Canvas::screenToMath(QPointF point) const {
    // Инвертируем ось y
    point.setY(height() - point.y());

    // Меняем масштаб
    point.setX(point.x() / width() * screenPosition.width());
    point.setY(point.y() / height() * screenPosition.height());

    //Перетаскиваем угол камеры к нужной позиции
    point += screenPosition.topLeft();

    return point;
}
