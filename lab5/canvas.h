#ifndef CANVAS_H
#define CANVAS_H
#include <qwidget.h>
#include <qlist.h>
#include <qpolygon.h>
#include <qrect.h>
#include <qline.h>
#include <qpen.h>
#include <qpixmap.h>
#include <QMouseEvent>
#include <QKeyEvent>

class Canvas : public QWidget {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);

public slots:
    void clear();
    void changeFillSpeed(int value);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    static const int canvasSize = 1000;
    QPixmap pixmap;
    QColor colorBg;
    QPen penBorder;
    QPen penLine;
    QPen penRect;

    QList<QPolygon> polygons;
    QPolygon currentPoly;
    QLine currentLine;
    QPoint mouseSavedPosition;
    QRect polygonRect;
    int fillDelay;

    enum class Mode {
        idle,
        addLine,
        addStraightLine
    };
    Mode mode;

    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;
    void addPolygonToPixmap(const QPolygon &p);
    static QPoint pointStraight(QPoint base, QPoint point);
    static QRect adjustMaxRect(QRect rect, QPoint p);
    void recalculateMaxRect();
    void mouseInputStop();
};


#endif // CANVAS_H
