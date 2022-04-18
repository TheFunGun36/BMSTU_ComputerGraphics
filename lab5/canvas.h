#ifndef CANVAS_H
#define CANVAS_H
#include <qwidget.h>
#include <qlist.h>
#include <qpolygon.h>
#include <qrect.h>
#include <qline.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <QMouseEvent>
#include <QKeyEvent>

class Canvas : public QWidget {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);

public slots:
    void clear();
    void changeFillSpeed(int value);
    void setSeparatorDraw(bool draw);
    void setRectDraw(bool draw);
    void setColorFill(QColor color);
    void beginFill();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    static const int canvasSize = 1000;
    QPixmap pixmap;
    QColor colorBg;
    QColor colorFill;
    QPen penBorder;
    QPen penLine;
    QPen penRect;
    QPen penSeparator;

    QList<QPolygon> polygons;
    QPolygon currentPoly;
    QLine currentLine;
    QLine separator;
    QPoint mouseSavedPosition;
    QRect polygonRect;
    int fillDelay;
    bool shouldDrawSeparator;
    bool shouldDrawRect;

    enum class Mode {
        idle,
        addLine,
        addStraightLine
    };
    Mode mode;

    QTimer timerDraw;
    struct DrawState {
        QList<QPolygon>::const_iterator currentPoly;
        QPolygon::const_iterator currentPoint;
        QLine currentLine;
        int absDx;
        int absDy;
        int stepX;
        int stepY;
        int error;
        int x;
        int y;
        bool dxGreaterDy;
        bool lastLine;

        DrawState() = default;
    };
    DrawState drawState;

    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;

    static QPoint pointStraight(QPoint base, QPoint point);
    static QRect adjustMaxRect(QRect rect, QPoint p);
    void recalculateMaxRect();
    void mouseInputStop();
    void drawMisc(QPainter &qp);

    void drawLine(QPoint point);
    void updateTimer();
    void flipPixel(QImage &image, QPoint pos);
private slots:
    void drawNextPoly();
    void drawNextSide();
    void drawNextLine();
};

#endif // CANVAS_H
