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

    void addLine(QLine line, LineColor color, LineAlgorithm alg);
    void undo();
    void clear();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    struct Line {
        QLine position;
        QColor color;
        void (*algorithm)(QImage &pixmap, const QLine &line, const QColor &color);
        
        Line(const QLine &pos, LineColor clr, LineAlgorithm alg);
    };

    QList<Line> lines;
    QPixmap pixmap;
    QColor imageBackgroundColor;
    QColor imageBorderColor;

    void drawLine(const Line &line);
};
