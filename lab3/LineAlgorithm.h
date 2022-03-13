#pragma once
#include <qimage.h>

void algorithmDDA(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmBrezenhamFloat(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmBrezenhamInt(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmBrezenhamAntialias(QImage &pixmap, const QLine &line, const QColor &color);
void algorithmWu(QImage &pixmap, const QLine &line, const QColor &color);

int countLaddersDDA(const QLine &line);
int countLaddersBrezenhamFloat(const QLine &line);
int countLaddersBrezenhamInt(const QLine &line);
int countLaddersBrezenhamAntialias(const QLine &line);
int countLaddersWu(const QLine &line);

long long testAlgorithm(int iterations, int lineLength, void (*algorithm)(QImage &, const QLine &, const QColor &));
