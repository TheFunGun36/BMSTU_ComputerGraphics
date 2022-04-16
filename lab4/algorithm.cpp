#include "algorithm.h"
#include <cmath>
#include <algorithm>
#include <chrono>
#include <qdebug.h>

static inline void symPlot(long cx, long cy, long x, long y, double i, std::function<void(long, long, double)> plot) {
    plot(cx + x, cy + y, i);
    plot(cx + x, cy - y, i);
    plot(cx - x, cy + y, i);
    plot(cx - x, cy - y, i);
}

void algCanonicalEq(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot) {
    long long rx2 = rx * rx;
    long long ry2 = ry * ry;

    for (long long x = 0; x <= rx; x++) {
        long long y = round(ry * sqrt(1. - double(x * x) / rx2));
        symPlot(cx, cy, x, y, 1., plot);
    }

    for (long long y = 0; y <= ry; y++) {
        long long x = round(rx * sqrt(1. - double(y * y) / ry2));
        symPlot(cx, cy, x, y, 1., plot);
    }
}

void algParametricalEq(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot) {
    long long m = std::max(rx, ry);
    long long l = round(M_PI * m / 2);
    for (long long i = 0; i <= l; i++) {
        long long x = round(rx * cos(double(i) / m));
        long long y = round(ry * sin(double(i) / m));
        symPlot(cx, cy, x, y, 1.0, plot);
    }
}

void algBrezenham(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot) {
    long long rx2 = rx * rx;
    long long ry2 = ry * ry;
    long long x = 0;
    long long y = ry;
    long long S = rx2 * (1 - 2 * ry) + 2 * ry2;
    long long T = ry2 - 2 * ry2 * (2 * ry - 1);

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

void algAveragePoint(long cx, long cy, long rx, long ry, std::function<void(long, long, double)> plot) {
    // начальные положения
    long long x = 0;
    long long y = ry;

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

void algCanonicalEq(long cx, long cy, long r, std::function<void(long, long, double)> plot) {
    long r2 = r * r;
    for (long x = 0; x <= r; x++) {
        long y = round(sqrt(r2 - x * x));
        symPlot(cx, cy, x, y, 1., plot);
    }

    for (long y = 0; y <= r; y++){
        long x = round(sqrt(r2 - y * y));
        symPlot(cx, cy, x, y, 1., plot);
    }
}

void algParametricalEq(long cx, long cy, long r, std::function<void(long, long, double)> plot) {
    long l = round(M_PI * r / 2);  // длина четверти окружности
    for (long i = 0; i <= l; i++) {
        long x = round(r * cos(i / r));
        long y = round(r * sin(i / r));
        symPlot(cx, cy, x, y, 1., plot);
    }
}

void algBrezenham(long cx, long cy, long r, std::function<void(long, long, double)> plot) {
    long x = 0;
    long y = r;
    long d = 2 - 2 * r; // D(0,R)

    while (y >= 0) {
        symPlot(cx, cy, x, y, 1., plot);

        if (d < 0) {
            // пиксель лежит внутри окружности
            long buf = 2 * d + 2 * y - 1;
            x++;

            if (buf <= 0) {
                // горизонтальный шаг
                d = d + 2 * x + 1;
            }
            else {
                // диагональный шаг
                y -= 1;
                d = d + 2 * x - 2 * y + 2;
            }
        }
        else if (d > 0) {
            // пиксель лежит вне окружности
            long buf = 2 * d - 2 * x - 1;
            y--;

            if (buf > 0) {
                //вертикальный шаг
                d = d - 2 * y + 1;
            }
            else {
                //диагональный шаг
                x++;
                d = d + 2 * x - 2 * y + 2;
            }
        }
        else {
            // пиксель лежит на окружности
            // диагональный шаг
            x++;
            y--;
            d = d + 2 * x - 2 * y + 2;
        }
    }
}

void algAveragePoint(long cx, long cy, long r, std::function<void(long, long, double)> plot) {
    long x = 0;
    long y = r;
    double p = 5. / 4. - r;  // (x + 1)^2 + (y - 1/2)^2 - r^2
    while (y > x) {
        symPlot(cx, cy, x, y, 1., plot);
        symPlot(cx, cy, y, x, 1., plot);

        x++;

        if (p < 0.) {
            // средняя точка внутри окружности, ближе верхний пиксел, горизонтальный шаг
            p += 2 * x + 1;
        }
        else {
            // средняя точка вне окружности, ближе диагональный пиксел, диагональный шаг
            p += 2 * x - 2 * y + 5;
            y--;
        }
    }
}

double testAlgorithmEllipse(void (*f)(long, long, long, long, std::function<void(long, long, double)>), long iter, long radius) {
    using namespace std::chrono;
    auto plot = [](long, long, double) {};

    time_point begin = high_resolution_clock::now();
    for (long i = 0; i < iter; i++) {
        f(0, 0, radius, 2 * radius, plot);
    }
    time_point end = high_resolution_clock::now();

    double result = duration<double, std::nano>((end - begin) / iter / 3).count();
    return result;
}

double testAlgorithmCircle(void (*f)(long, long, long, std::function<void(long, long, double)>), long iter, long radius) {
    using namespace std::chrono;
    auto plot = [](long, long, double) {};

    time_point begin = high_resolution_clock::now();
    for (long i = 0; i < iter; i++) {
        f(0, 0, radius, plot);
    }
    time_point end = high_resolution_clock::now();

    double result = duration<double, std::nano>((end - begin) / iter / 3).count();
    return result;
}
