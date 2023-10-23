#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>

//const double PI = acos(-1);

enum status_codes {
    ok, 
    OVERFLOWED,
    INVALID, 
    MEMORY_ISSUES,
    UNABLE_TO_OPEN_FILE
};

void print_error(int st) {
    switch (st)
    {
    case OVERFLOWED:
        printf("OverflowED!\n");
        break;
    case MEMORY_ISSUES:
        printf("Memory issues!\n");
        break;
    case UNABLE_TO_OPEN_FILE:
        printf("Unable to open a file.\n");
        break;
    default:
        break;
    }
}

typedef struct {
    double x, y;
} vec2d;

double cross(const vec2d a, const vec2d b) {
    return a.x * b.y - b.x * a.y;
}

int is_convex(int n, ...) {
    if (n < 3) return false;
    va_list param;
    va_start(param, n);
    vec2d a, b;
    double x1, x2, y1, y2, x3, y3;
    x1 = va_arg(param, double);
    y1 = va_arg(param, double);
    x2 = va_arg(param, double);
    y2 = va_arg(param, double);
    x3 = va_arg(param, double);
    y3 = va_arg(param, double);
    a.x = x2-x1;
    a.y = y2-y1;
    b.x = x3-x2;
    b.y = y3-y2;
    double cr = cross(a, b);
    for (int i = 2; i < n; i++) {
        x2 = x3;
        y2 = y3;
        x3 = va_arg(param, double);
        y3 = va_arg(param, double);
        a.x = b.x;
        a.y = b.y;
        b.x = x3-x2;
        b.y = y3-y2;
        double cr2 = cross(a, b);
        if (cr * cr2 <= 0) {
            va_end(param);
            return false;
        }
    }
    va_end(param);
    return true;
}

int polynomial_value(double* ans, double x, int n, ...) {
    if (n < 0) {
        return INVALID;
    }
    va_list param;
    va_start(param, n);
    *ans = va_arg(param, double);
    for (int i = 0; i < n; i++) {
        *ans *= x;
        *ans += va_arg(param, double);
    }
    va_end(param);
    return ok;
}

int main() {
    printf("%d\n", is_convex(4, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0));
    double ans;
    int st = polynomial_value(&ans, -600.0, 2, 1.0, 2.0, 1.0);
    if (st != ok) {
        print_error(st);
        return 1;
    }
    else {
        printf("%lf\n", ans);
    }
    return 0;
}