#pragma once
#include <functional>

void algCanonicalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algParametricalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algBrezenham(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algAveragePoint(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);

double testAlgorithm(void (*f)(int, int, int, int, std::function<void(int, int, double)>), int iter);
