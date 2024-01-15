#include "Poly_project.h"
int findGCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to simplify a fraction
void simplifyFraction(int *numerator, int *denominator) {
    int gcd = findGCD(*numerator, *denominator);

    // Ensure the denominator is not zero
    if (gcd != 0) {
        *numerator /= gcd;
        *denominator /= gcd;
    }
}