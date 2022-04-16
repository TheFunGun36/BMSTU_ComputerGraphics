#include "canvas.h"
#include <qdebug.h>
#include <qpainter.h>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , pixmap(800, 800)
    , colorBg(QColor(255, 255, 255))
    , penBorder(QColor(180, 180, 180))
    , penLine(QColor(0, 0, 0))
    , penRect(QColor(200, 200, 200), 1, Qt::DashLine)
    , polygons()
    , currentPoly()
    , currentLine() {
    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    pixmap.fill(colorBg);
    mode = Mode::idle;
}

void Canvas::mouseMoveEvent(QMouseEvent *e) {
    if (mode != Mode::idle) {
        if (mode == Mode::addLine)
            currentLine.setP2(e->pos());
        else //if (mode == Mode::addStraightLine)
            currentLine.setP2(pointStraight(currentLine.p1(), e->pos()));

        mouseSavedPosition = e->pos();
        recalculateMaxRect();
        update();
    }
}

void Canvas::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        if (mode == Mode::idle) {
            mode = Mode::addLine;
            currentPoly.append(e->pos());
            currentLine.setP1(e->pos());
            currentLine.setP2(e->pos());
        }
        else {
            currentPoly.append(currentLine.p2());
            currentLine.setP1(currentLine.p2());
        }
    }
    else if (e->button() == Qt::RightButton && mode != Mode::idle) {
        if (currentPoly.size() >= 3) {
            addPolygonToPixmap(currentPoly);
            polygons.append(currentPoly);
        }
        mouseInputStop();
        update();
    }
}

void Canvas::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        mouseInputStop();
        update();
        break;
    case Qt::Key_Shift:
        if (mode == Mode::addLine) {
            mode = Mode::addStraightLine;
            currentLine.setP2(pointStraight(currentLine.p1(), currentLine.p2()));
            update();
        }
        break;
    }
}

void Canvas::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Shift && mode == Mode::addStraightLine) {
        mode = Mode::addLine;
        currentLine.setP2(mouseSavedPosition);
        update();
    }
}

void Canvas::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);

    QPainter qp(this);
    qp.setPen(penBorder);
    qp.drawRect(rect());
    qp.setPen(penLine);
    qp.drawPixmap(pixmap.rect(), pixmap);

    if (mode != Mode::idle) {
        qp.drawPolyline(currentPoly);
        qp.drawLine(currentLine);
    }

    qp.setPen(penRect);
    qp.drawRect(polygonRect);
}

void Canvas::addPolygonToPixmap(const QPolygon &p) {
    QPainter qp(&pixmap);
    qp.drawPolygon(p);
}

QPoint Canvas::pointStraight(QPoint base, QPoint point) {
    if (qAbs(point.y() - base.y()) >= qAbs(point.x() - base.x())) {
        // vertical
        point.setX(base.x());
    }
    else {
        // horizontal
        point.setY(base.y());
    }

    return point;
}

void Canvas::recalculateMaxRect() {
    QRect r;
    if (mode != Mode::idle) {
        r.setTop(qMin(currentLine.p1().y(), currentLine.p2().y()));
        r.setLeft(qMin(currentLine.p1().x(), currentLine.p2().x()));
        r.setBottom(qMax(currentLine.p1().y(), currentLine.p2().y()));
        r.setRight(qMax(currentLine.p1().x(), currentLine.p2().x()));

        for (const auto &pt : currentPoly)
            r = adjustMaxRect(r, pt);
    }
    else if (!polygons.isEmpty()) {
        r.setTopLeft(polygons.first().first());
        r.setBottomRight(polygons.first().first());
    }

    for (const auto &pl : polygons)
        for (const auto &pt : pl)
            r = adjustMaxRect(r, pt);

    polygonRect = r;
}

QRect Canvas::adjustMaxRect(QRect r, QPoint pt) {
    if (r.top() > pt.y())
        r.setTop(pt.y());
    else if (r.bottom() < pt.y())
        r.setBottom(pt.y());

    if (r.left() > pt.x())
        r.setLeft(pt.x());
    else if (r.right() < pt.x())
        r.setRight(pt.x());

    return r;
}

void Canvas::clear() {
    pixmap.fill(colorBg);
    polygons.clear();
    mouseInputStop();
    update();
}

void Canvas::changeFillSpeed(int value) {
    fillDelay = 99 - value;
}

void Canvas::mouseInputStop() {
    mode = Mode::idle;
    currentLine = QLine();
    currentPoly.clear();
    recalculateMaxRect();
}
