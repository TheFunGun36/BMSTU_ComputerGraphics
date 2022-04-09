#include "algorithm.h"
#include <cmath>
#include <algorithm>
#include <chrono>
#include <qdebug.h>

static inline void symPlot(int cx, int cy, int x, int y, double i, std::function<void(int, int, double)> plot) {
    plot(cx + x, cy + y, i);
    plot(cx + x, cy - y, i);
    plot(cx - x, cy + y, i);
    plot(cx - x, cy - y, i);
}

void algCanonicalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {
    int rx2 = rx * rx;
    int ry2 = ry * ry;

    for (int x = 0; x <= rx; x++) {
        int y = round(ry * sqrt(1.0 - double(x * x) / rx2));
        symPlot(cx, cy, x, y, 1.0, plot);
    }

    for (int y = 0; y <= ry; y++) {
        int x = round(rx * sqrt(1.0 - double(y * y) / ry2));
        symPlot(cx, cy, x, y, 1.0, plot);
    }
}

void algParametricalEq(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {
    int m = std::max(rx, ry);
    int l = round(M_PI * m / 2);
    for (int i = 0; i <= l; i++) {
        int x = round(rx * cos(double(i) / m));
        int y = round(ry * sin(double(i) / m));
        symPlot(cx, cy, x, y, 1.0, plot);
    }
}

void algBrezenham(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {
    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int x = 0;
    int y = ry;
    int S = rx2 * (1 - 2 * ry) + 2 * ry2;
    int T = ry2 - 2 * ry2 * (2 * ry - 1);

    symPlot(cx, cy, x, y, 1.0, plot);
    do {
        if (S<0) {
            S += 2 * ry2 * (2 * x + 3);
            T += 4 * ry2 * (x + 1);
            x++;
        }
        else if (T<0) {
            S += 2 * ry2 * (2 * x + 3) - 4 * rx2 * (y - 1);
            T += 4 * ry2 * (x + 1) - 2 * rx2 * (2 * y - 3);
            x++;
            y--;
        }
        else {
            S -= 4 * rx2 * (y - 1);
            T -= 2 * rx2 * (2 * y - 3);
            y--;
        }

        symPlot(cx, cy, x, y, 1.0, plot);
    } while (y>0);
}

void algAveragePoint(int cx, int cy, int rx, int ry, std::function<void(int, int, double)> plot) {
    // начальные положения
    int x = 0;
    int y = ry;

    // квадраты полуосей (для упрощения вычислений)
    long long rx2 = rx * rx;
    long long ry2 = ry * ry;

    // начальное значение параметра принятия решения в области tg<1
    double p = ry2 - rx2 * (ry - 0.25);

    // пока тангенс угла наклона меньше 1
    while (ry2 * x < rx2 * y) {
        symPlot(cx, cy, x, y, 1.0, plot);

        x++;

        if (p < 0) {
            // средняя точка внутри эллипса, ближе верхний пиксел, горизонтальный шаг
            p += (2 * x + 1) * ry2;
        }
        else {
            // средняя точка вне эллипса, ближе диагональный пиксел, диагональный шаг
            y--;
            p += 2 * (ry2 * x - rx2 * y) + ry2;
        }
    }

    // начальное значение параметра принятия решения в области tg>1 в точке (х + 0.5, y - 1) полседнего положения
    double e1 = ry2 * (x + 0.5) * (x + 0.5);
    double e2 = rx2 * (y - 1) * (y - 1);
    double e3 = rx2 * ry2;
    p = e1 + e2 - e3;

    while (y >= 0) {
        symPlot(cx, cy, x, y, 1.0, plot);

        y--;

        if (p > 0) {
            p -= (2 * y + 1) * rx2;
        }
        else {
            x++;
            p += 2 * (ry2 * x - rx2 * y) + rx2;
        }
    }
}

double testAlgorithm(void (*f)(int, int, int, int, std::function<void(int, int, double)>), int iter) {
    using namespace std::chrono;
    auto plot = [](int, int, double) {};

    for (int i = 0; i < iter / 2; i++) {
        f(350, 350, 350, 100, plot);
        f(350, 350, 100, 350, plot);
        f(350, 350, 500, 500, plot);
    }

    time_point begin = high_resolution_clock::now();
    for (int i = 0; i < iter; i++) {
        f(350, 350, 350, 100, plot);
        f(350, 350, 100, 350, plot);
        f(350, 350, 500, 500, plot);
    }
    time_point end = high_resolution_clock::now();

    double result = duration<double, std::micro>((end - begin) / iter / 3).count();
    return result;
}
