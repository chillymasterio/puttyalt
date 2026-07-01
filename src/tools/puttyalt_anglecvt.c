/* puttyalt_anglecvt.c - Angle unit conversions (deg/rad/grad).
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
#define ANG_PI 3.14159265358979323846
double angc_deg2rad(double d) { return d * ANG_PI / 180.0; }
double angc_rad2deg(double r) { return r * 180.0 / ANG_PI; }
double angc_normalize_deg(double d) { while (d < 0) d += 360; while (d >= 360) d -= 360; return d; }
double angc_shortest_diff(double a, double b) {
    double diff = angc_normalize_deg(b - a);
    if (diff > 180) diff -= 360;
    return diff;
}
