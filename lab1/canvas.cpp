#include "canvas.h"
#include <qpainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent) {
    tablePoints = nullptr;
    margin = 0.15;
    pointRadius = 3;
    minCellSize = 20;
    maxCellSize = 210;
    scale = 1;
    colorBackground = QColor(20, 40, 80);
    colorPoint = QColor(200, 240, 240);
    colorPointTask = QColor(100, 200, 100);
    colorPointTaskSecondary = QColor(60, 120, 90);
    colorTriangleBig = QColor(200, 0, 0, 50);
    colorTriangleSmall = QColor(0, 200, 0, 50);
    colorAxes = QColor(100, 150, 150);
    colorGrid = QColor(60, 60, 90);
    colorGridAccent = QColor(80, 120, 120);
}

void Canvas::setPointsTable(QTableWidget *tablePoints) {
    this->tablePoints = tablePoints;
    pointsTableChanged();
}

int Canvas::getScale() {
    return scale;
}

void Canvas::windowResized() {
    recalculateScreenPosition();
}

void Canvas::recalculateScreenPosition() {
    assert(tablePoints);
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
}

void Canvas::drawPoint(QPainter *qp, QPointF point) {
    point = mathToScreen(point);
    QRect pos;
    pos.setTopLeft(QPoint(point.x() - pointRadius, point.y() - pointRadius));
    pos.setBottomRight(QPoint(point.x() + pointRadius, point.y() + pointRadius));
    qp->drawEllipse(pos);
}

void Canvas::drawPoints(QPainter *qp) {
    qp->setBrush(colorPoint);
    qp->setPen(colorPoint);
    for (QPointF point : points)
        drawPoint(qp, point);

    qp->setBrush(colorPointTask);
    qp->setPen(colorPointTask);
    for (QPointF point : pointsTask)
        drawPoint(qp, point);

    qp->setBrush(colorPointTaskSecondary);
    qp->setPen(colorPointTaskSecondary);
    for (QPointF point : pointsTaskSecondary)
        drawPoint(qp, point);
}

void Canvas::drawAxes(QPainter *qp) {
    qp->setPen(QPen(QBrush(colorAxes), 2));
    QPointF center = mathToScreen(QPoint(0, 0));
    qp->drawLine(0, round(center.y()), width(), round(center.y()));
    qp->drawLine(round(center.x()), 0, round(center.x()), height());
}

void Canvas::drawGrid(QPainter *qp) {
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

    emit scaleChanged(scale);

    qp->setPen(colorGrid);
    for (double cur = center.x(); cur < width(); cur += delta)
        qp->drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.x(); cur > 0; cur -= delta)
        qp->drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.y(); cur < height(); cur += delta)
        qp->drawLine(0, round(cur), width(), round(cur));
    for (double cur = center.y(); cur > 0; cur -= delta)
        qp->drawLine(0, round(cur), width(), round(cur));

    delta *= 10;
    qp->setPen(colorGridAccent);
    for (double cur = center.x(); cur < width(); cur += delta)
        qp->drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.x(); cur > 0; cur -= delta)
        qp->drawLine(round(cur), 0, round(cur), height());
    for (double cur = center.y(); cur < height(); cur += delta)
        qp->drawLine(0, round(cur), width(), round(cur));
    for (double cur = center.y(); cur > 0; cur -= delta)
        qp->drawLine(0, round(cur), width(), round(cur));
}

void Canvas::drawTriangles(QPainter *qp) {
    QPointF pts[3];
    QPointF bisSide[3];

    for (int i = 0; i < 3; i++) {
        pts[i] = mathToScreen(pointsTask[i]);
        bisSide[i] = mathToScreen(pointsTaskSecondary[i]);
    }

    qp->setPen(colorPointTask);
    qp->drawLine(pts[0], pts[1]);
    qp->drawLine(pts[0], pts[2]);
    qp->drawLine(pts[1], pts[2]);

    qp->setPen(colorPointTaskSecondary);
    for (int i = 0; i < 3; i++)
        qp->drawLine(pts[i], bisSide[i]);

    qp->setBrush(colorTriangleBig);
    taskTriangle.getBisectLargestSubtriangle()->getVertexes(pts[0], pts[1], pts[2]);
    for (int i = 0; i < 3; i++)
        pts[i] = mathToScreen(pts[i]);
    qp->drawPolygon(pts, 3);

    qp->setBrush(colorTriangleSmall);
    taskTriangle.getBisectSmallestSubtriangle()->getVertexes(pts[0], pts[1], pts[2]);
    for (int i = 0; i < 3; i++)
        pts[i] = mathToScreen(pts[i]);
    qp->drawPolygon(pts, 3);
}

void Canvas::drawBackground(QPainter *qp) {
    qp->setBrush(QBrush(colorBackground));
    qp->setPen(colorBackground);
    qp->drawRect(0, 0, width(), height());
}

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

void Canvas::paintEvent(QPaintEvent *event) {
    assert(tablePoints);

    QPainter qp(this);

    drawBackground(&qp);
    drawGrid(&qp);
    drawAxes(&qp);
    if (!pointsTask.isEmpty())
        drawTriangles(&qp);
    drawPoints(&qp);
}

QPointF Canvas::getPoint(int index) const {
    if (!tablePoints->item(index, 0) || !tablePoints->item(index, 1))
        throw Exception::cantReadCell;
    return QPointF(tablePoints->item(index, 0)->text().toDouble(), tablePoints->item(index, 1)->text().toDouble());
}

void Canvas::taskCalculate() {
    /*
     * На плоскости дано множество точек.
     * Найти такой треугольник с вершинами в точках этого множества,
     * у которого минимальна разность площадей между наибольшей и наименьшей
     * из шести треугольников, образованных пересечением биссектрис
     */

     // Необходимо выделить ответ на задачу, записав координаты конечных точек в pointsTask
     // дополнительные точки пересечения нужно записать в pointsTaskSecondary
    int pointsAmount = tablePoints->rowCount();

    // Проводим полный перебор. Сохраняем лучший результат
    double bestResult = -1;
    int bestPoints[3] = { 0, 1, 2 };
    Triangle bestTriangle;

    // Выделяем 3 тестовые точки. Прогоняем их по алгоритму из задачи
    for (int p1 = 0; p1 < pointsAmount - 2; p1++) {
        for (int p2 = p1 + 1; p2 < pointsAmount - 1; p2++) {
            for (int p3 = p2 + 1; p3 < pointsAmount; p3++) {
                Triangle triangle(points[p1], points[p2], points[p3]);
                double result = taskCalculateTriangle(triangle);

                if (result > 0 && (result < bestResult || bestResult < 0)) {
                    bestResult = result;
                    bestPoints[0] = p1;
                    bestPoints[1] = p2;
                    bestPoints[2] = p3;
                    bestTriangle = triangle;
                }
            }
        }
    }

    pointsTask.clear();
    pointsTaskSecondary.clear();
    taskTriangle = Triangle();
    if (bestResult < 0) {
        // Не смогли найти валидного треугольника
        emit newAnswer(-1, -1);
    }
    else {
        // Нашли лучший треугольник, перемещаем его точки в pointsTask
        pointsTask.reserve(3);
        for (int i = 0; i < 3; i++)
            pointsTask.push_back(points[bestPoints[i]]);

        // Убираем точки из points
        for (int i = 2; i >= 0; i--)
            points.remove(bestPoints[i]);

        // Добавляем точки пересечения биссектрис со сторонами в pointsTaskSecondary
        QPointF p1, p2, p3;
        pointsTaskSecondary.reserve(4);
        bestTriangle.getBisectSideOverlap(p1, p2, p3);
        pointsTaskSecondary.push_back(p1);
        pointsTaskSecondary.push_back(p2);
        pointsTaskSecondary.push_back(p3);
        pointsTaskSecondary.push_back(bestTriangle.getBisectOverlap());
        taskTriangle = bestTriangle;

        emit newAnswer(bestTriangle.getBisectSmallestSubtriangle()->getArea(),
            bestTriangle.getBisectLargestSubtriangle()->getArea());
    }
}

double Canvas::taskCalculateTriangle(Triangle &triangle) const {
    // Функция должна вернуть -1, если исходный треугольник невалиден
    // Функция должна вернуть разницу между площадями
    // наибольшего и наименьшего треугольников, образованных биссектрисами
    double result = 0.0;

    if (!triangle.isValid()) {
        result = -1.0;
    }
    else {
        std::shared_ptr<Triangle> big = triangle.getBisectLargestSubtriangle();
        std::shared_ptr<Triangle> small = triangle.getBisectSmallestSubtriangle();
        result = big->getArea() - small->getArea();
    }

    return result;
}

void Canvas::pointsTableChanged() {
    int pointsAmount = tablePoints->rowCount();
    try {
        getPoint(0);
    }
    catch (Exception e) {
        if (e == Exception::cantReadCell)
            return;
        else
            throw e;
    }

    points.clear();
    points.reserve(pointsAmount);
    for (int i = 0; i < pointsAmount; i++)
        points.push_back(getPoint(i));

    recalculateScreenPosition();
    taskCalculate();
    update();
}
