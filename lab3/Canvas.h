#pragma once
#include <qopenglwidget.h>
#include <qlist.h>

enum class LineColor {
    black,
    white,
    red,
    green,
    blue
};

enum class LineAlgorithm {
    digitalDiffAnalyzer,
    brezenhamFloat,
    brezenhamInt,
    brezenhamMod,
    Wu,
    Library
};

class Canvas : public QOpenGLWidget {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);

    void addSun(QLine line, LineColor color, LineAlgorithm algorithm, int linesAmount = 1);
    void undo();
    void clear();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    static const int canvasSize = 700;
    struct Sun {
        QList<QLine> lines;
        QColor color;
        void (*algorithm)(QImage &pixmap, const QLine &line, const QColor &color);

        Sun(const QLine &base, LineColor clr, LineAlgorithm alg, int amount = 1);
    };

    QList<Sun> suns;
    QPixmap pixmap;
    QColor imageBackgroundColor;
    QColor imageBorderColor;

    void drawSun(const Sun &line);
};
