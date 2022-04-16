#pragma once
#include <functional>

void algCanonicalEq(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot);
void algParametricalEq(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot);
void algBrezenham(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot);
void algAveragePoint(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot);

void algCanonicalEq(long cx, long cy, long r, std::function<void(long, long, double)> plot);
void algParametricalEq(long cx, long cy, long r, std::function<void(long, long, double)> plot);
void algBrezenham(long cx, long cy, long r, std::function<void(long, long, double)> plot);
void algAveragePoint(long cx, long cy, long r, std::function<void(long, long, double)> plot);

double testAlgorithmEllipse(void (*f)(long, long, long, long, std::function<void(long, long, double)>), long iter, long radius);
double testAlgorithmCircle(void (*f)(long, long, long, std::function<void(long, long, double)>), long iter, long radius);
