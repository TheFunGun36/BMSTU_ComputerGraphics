#include "canvas.h"
#include <qdebug.h>
#include <qpainter.h>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , pixmap(800, 800)
    , colorBg(QColor(255, 255, 255))
    , colorFill(QColor(0, 0, 0))
    , penBorder(QColor(180, 180, 180))
    , penLine(QColor(0, 0, 0))
    , penRect(QColor(0, 0, 0, 50), 1, Qt::DashLine)
    , penSeparator(QColor(0, 0, 0), 1, Qt::DashDotLine)
    , polygons()
    , currentPoly()
    , currentLine()
    , fillDelay(99)
    , timerDraw(this) {
    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    timerDraw.setSingleShot(true);
    timerDraw.setTimerType(Qt::PreciseTimer);

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
        if (currentPoly.size() >= 3)
            polygons.append(currentPoly);
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
    qp.drawPixmap(pixmap.rect(), pixmap);
    for (const auto &poly : polygons)
        qp.drawPolygon(poly);
    {
        QRect r = rect();
        r.setBottom(r.bottom() - 1);
        r.setRight(r.right() - 1);
        qp.setPen(penBorder);
        qp.drawRect(r);
    }

    qp.setPen(penLine);
    if (mode != Mode::idle) {
        qp.drawPolyline(currentPoly);
        qp.drawLine(currentLine);
        drawMisc(qp);
    }
    else if (!polygons.isEmpty()) {
        drawMisc(qp);
    }
}

void Canvas::drawMisc(QPainter &qp) {
    if (shouldDrawRect) {
        qp.setPen(penRect);
        qp.drawRect(polygonRect);
    }
    if (shouldDrawSeparator) {
        qp.setPen(penSeparator);
        qp.drawLine(separator);
    }
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
    separator.setP1(QPoint(r.center().x(), r.top() - 10));
    separator.setP2(QPoint(r.center().x(), r.bottom() + 10));
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
    timerDraw.stop();
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

void Canvas::setSeparatorDraw(bool draw) {
    shouldDrawSeparator = draw;
    update();
}

void Canvas::setRectDraw(bool draw) {
    shouldDrawRect = draw;
    update();
}

void Canvas::updateTimer() {
    timerDraw.disconnect();
    timerDraw.setInterval(fillDelay / 4);
}

void Canvas::beginFill() {
    if (polygons.isEmpty())
        return;

    drawState = DrawState();
    drawState.currentPoly = polygons.cbegin();
    drawState.currentPoly--;

    updateTimer();
    connect(&timerDraw, &QTimer::timeout, this, &Canvas::drawNextPoly);
    timerDraw.start();
    //drawNextSize(); // with delay
}

void Canvas::drawNextPoly() {
    drawState.currentPoly++;
    if (drawState.currentPoly == polygons.cend()) {
        return;
    }

    drawState.currentPoint = drawState.currentPoly->cbegin();
    drawState.currentLine.setP2(*drawState.currentPoint);

    updateTimer();
    connect(&timerDraw, &QTimer::timeout, this, &Canvas::drawNextSide);
    timerDraw.start();
}

void Canvas::drawNextSide() {
    drawState.currentLine.setP1(drawState.currentLine.p2());
    drawState.currentPoint++;
    if (drawState.lastLine) {
        drawState.lastLine = false;
        updateTimer();
        connect(&timerDraw, &QTimer::timeout, this, &Canvas::drawNextPoly);
        timerDraw.start();
        return;
    }

    if (drawState.currentPoint == drawState.currentPoly->cend()) {
        drawState.currentLine.setP2(drawState.currentPoly->first());
        drawState.lastLine = true;
    }
    else
        drawState.currentLine.setP2(*drawState.currentPoint);

    drawState.stepX = (0 < drawState.currentLine.dx()) - (drawState.currentLine.dx() < 0);
    drawState.stepY = (0 < drawState.currentLine.dy()) - (drawState.currentLine.dy() < 0);
    drawState.absDx = abs(drawState.currentLine.dx());
    drawState.absDy = abs(drawState.currentLine.dy());
    drawState.dxGreaterDy = drawState.absDx > drawState.absDy;
    drawState.x = drawState.currentLine.x1();
    drawState.y = drawState.currentLine.y1();
    if (drawState.dxGreaterDy)
        drawState.error = 2 * drawState.absDy - drawState.absDx;
    else
        drawState.error = 2 * drawState.absDx - drawState.absDy;

    updateTimer();
    connect(&timerDraw, &QTimer::timeout, this, &Canvas::drawNextLine);
    timerDraw.start();
}

void Canvas::drawNextLine() {
    if (drawState.x == drawState.currentLine.x2() &&
            drawState.y == drawState.currentLine.y2()) {
        //drawLine(QPoint(drawState.x, drawState.y));
        updateTimer();
        connect(&timerDraw, &QTimer::timeout, this, &Canvas::drawNextSide);
        timerDraw.start();
        return;
    }

    if (drawState.absDx > drawState.absDy) {
        if (drawState.error >= 0) {
            drawLine(QPoint(drawState.x, drawState.y));
            drawState.y += drawState.stepY;
            drawState.error -= 2 * drawState.absDx;
        }

        drawState.error += 2 * drawState.absDy;
        drawState.x += drawState.stepX;
    }
    else {
        drawLine(QPoint(drawState.x, drawState.y));

        if (drawState.error >= 0) {
            drawState.x += drawState.stepX;
            drawState.error -= 2 * drawState.absDy;
        }

        drawState.error += 2 * drawState.absDx;
        drawState.y += drawState.stepY;
    }

    updateTimer();
    connect(&timerDraw, &QTimer::timeout, this, &Canvas::drawNextLine);
    timerDraw.start();
}

void Canvas::drawLine(QPoint point) {
    QImage image = pixmap.toImage();
    int endpoint = separator.x1();

    int x = point.x();
    if (x < endpoint) {
        while (x < separator.x1()) {
            flipPixel(image, QPoint(x, point.y()));
            x++;
        }
    }
    else {
        while (x >= separator.x1()) {
            flipPixel(image, QPoint(x, point.y()));
            x--;
        }
    }

    pixmap.convertFromImage(image);
    update();
}

void Canvas::flipPixel(QImage &image, QPoint pos) {
    if (image.pixelColor(pos) == colorFill)
        image.setPixelColor(pos, colorBg);
    else
        image.setPixelColor(pos, colorFill);
}

void Canvas::setColorFill(QColor color) {
    colorFill = color;
}
