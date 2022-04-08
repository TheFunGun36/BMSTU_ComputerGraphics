#pragma once
#include <functional>

void algLibrary(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algCanonicalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algParametricalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algBrezenham(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
void algAveragePoint(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot);
