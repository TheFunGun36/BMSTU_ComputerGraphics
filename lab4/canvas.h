#pragma once
#include <qopenglwidget.h>
#include <qlist.h>

struct Ellipse {
    int cx;
    int cy;
    int rx;
    int ry;

    inline Ellipse operator+(const Ellipse &other) {
        Ellipse result;
        result.cx = cx;
        result.cy = cy;
        result.rx = rx + other.rx;
        result.ry = ry + other.ry;
        return result;
    }

    inline Ellipse operator-(const Ellipse &other) {
        Ellipse result;
        result.cx = cx;
        result.cy = cy;
        result.rx = rx - other.rx;
        result.ry = ry - other.ry;
        return result;
    }

    inline Ellipse operator/(const double value) {
        Ellipse result;
        result.cx = cx;
        result.cy = cy;
        result.rx = qRound(rx / value);
        result.ry = qRound(ry / value);
        return result;
    }
};

enum class Color {
    black,
    white,
    red,
    green,
    blue
};

enum class Algorithm {
    canonicalEquation,
    parametricalEquation,
    brezenham,
    averagePoint
};

class Canvas : public QOpenGLWidget {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);

    void addEllipse(Ellipse ellipse, Color color, Algorithm algorithm);
    void addCircle(Ellipse ellipse, Color color, Algorithm algorithm);
    void addEllipseSpectrum(Ellipse begin, Ellipse end, int amount, Color color, Algorithm algorithm);
    void addCircleSpectrum(Ellipse begin, Ellipse end, int amount, Color color, Algorithm algorithm);
    void clear();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    static const int canvasSize = 700;

    QPixmap pixmap;
    QColor imageBackgroundColor;
    QColor imageBorderColor;

    void drawEllipse(Ellipse ellipse, QColor clr, Algorithm alg);
    void drawCircle(Ellipse ellipse, QColor clr, Algorithm alg);
};
