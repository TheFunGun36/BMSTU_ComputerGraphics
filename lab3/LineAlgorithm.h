#pragma once
#include <qimage.h>

void algorithmDDA(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmBrezenhamFloat(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmBrezenhamInt(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmBrezenhamAntialias(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmWu(QImage &pixmap, const QLine &line, const QColor &color);
