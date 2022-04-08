#include "algorithm.h"

void algLibrary(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {

}

void algCanonicalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {

}

void algParametricalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {

}

void algBrezenham(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {
    int x = 0;
    int y = ry;
    int delta = 2 * (1 - rx);

    while (y > 0) {
        plot(cx + x, cy + y, 1.0);
        plot(cx + x, cy - y, 1.0);
        plot(cx - x, cy + y, 1.0);
        plot(cx - x, cy - y, 1.0);

        if (delta < 0 && 2 * (delta + y) - 1 <= 0) {
            x++;
            delta += 2 * x + 1;
        }
        else if (delta > 0 && 2 * (delta - x) - 1 > 0) {
            y--;
            delta += -2 * y + 1;
        }
        else {
            x++;
            y--;
            delta += 2 * (x - y);
        }
    }
}

void algAveragePoint(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {

}
