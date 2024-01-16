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

bool is_valid_polynomial(const char *expression) {
  int i = 0;
  while (expression[i] != '\0') {
    if (!isdigit(expression[i]) && expression[i] != '/' &&
        expression[i] != '^' && expression[i] != '+' && expression[i] != '-' &&
        expression[i] != 'X') {
      return false;
    }
    if (expression[i] == '^' && !isdigit(expression[i + 1])) {
      return false;
    }
    if (expression[i] == '/') {
      if ((i == 0 || !isdigit(expression[i - 1])) ||
          (expression[i + 1] != '\0' && !isdigit(expression[i + 1]) &&
           expression[i + 1] != 'X')) {
        return false;
      }
    }
    if (expression[i] == 'X' && expression[i + 1] != '\0' &&
        expression[i + 1] != '^' && isdigit(expression[i + 1])) {
      return false;
    }

    i++;
  }

  return true;
}

bool integer(char *key){

    int i;
    for (i = 0; key[i] != '\0'; i++) {
        if (!isdigit((unsigned char)key[i])) {
            
            return false;
        }
    }
    return true;
}