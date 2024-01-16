#include "Poly_project.h"
#define MAX_LENGTH 100

void displayHelp() {
  printf("Available commands:\n");
  printf("  LET <key> = <polynomial> : Assign a polynomial to a key (e.g., LET "
         "P = 2X^2 + 3X + 1)\n");
  printf("  SET <key> = <polynomial> : Same as LET\n");
  printf("  DISPLAY <key> : Display the polynomial assigned to a key (e.g., "
         "DISPLAY P)\n");
  printf("  ADD <key1>,<key2> : Add two polynomials and store the result in "
         "key1 (e.g., ADD P,Q)\n");
  printf("  SUB <key1>,<key2> : Subtract key2 from key1 and store the result "
         "in key1 (e.g., SUB P,Q)\n");
  printf("  MUL <key1>,<key2> : Multiply two polynomials and store the result "
         "in key1 (e.g., MUL P,Q)\n");
  printf("  POW <key1>,<key2> : Raise key1 to the power of key2 and store the "
         "result in key1 (e.g., POW P,Q)\n");
  printf("  AFFECT <key1>,<key2> : Assign the polynomial of key2 to key1 "
         "(e.g., AFFECT P,Q)\n");
  printf("  DER <key> : Differentiate the polynomial assigned to key (e.g., "
         "DER P)\n");
  printf(
      "  INT <key> : Integrate the polynomial assigned to key (e.g., INT P)\n");
  printf("  EVAL <key1> for X=<key2> : Evaluate the polynomial assigned to "
         "key1 for X=key2 (e.g., EVAL P for X=2)\n");
  printf("  HELP : Display available commands\n");
  printf("  EXIT : Exit the program\n");
}

int main() {
  char buffer[MAX_LENGTH];
  char *key, *key2, *poly;
  hash_table_t *poly_table = poly_table_create(10);

  while (1) {
    printf(">>");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
      key = malloc(MAX_LENGTH);
      key2 = malloc(MAX_LENGTH);
      poly = malloc(MAX_LENGTH);

      if (key == NULL || key2 == NULL || poly == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
      }

      if (sscanf(buffer, "LET %[^=] = %[^\n]", key, poly) == 2) {
        if (!is_valid_polynomial(poly)) {
          printf("Error: Invalid polynomial. Check missing terms or avoid "
                 "spaces.\n");
        } else {
          poly_table_set(poly_table, key, poly);
        }
      } else if (sscanf(buffer, "SET %[^=] = %[^\n]", key, poly) == 2) {
        if (!is_valid_polynomial(poly)) {
          printf("Error: Invalid polynomial. Check missing terms or avoid "
                 "spaces.\n");
        } else {
          poly_set(poly_table, key, poly);
        }
      } else if (sscanf(buffer, "DISPLAY %s", key) == 1) {
        poly_table_print(poly_table, key);
      } else if (sscanf(buffer, "ADD %[^,],%[^ \n]", key, key2) == 2) {
        printf("Key: %s\n", key);
        printf("Key2: %s\n", key2);
      } else if (sscanf(buffer, "SUB %[^,],%[^ \n]", key, key2) == 2) {
        printf("Key: %s\n", key);
        printf("Key2: %s\n", key2);
      } else if (sscanf(buffer, "MUL %[^,],%[^ \n]", key, key2) == 2) {
        poly_node_t *result = multiplication(poly_table, key, key2);
        displayPolynomial(result);
      } else if (sscanf(buffer, "POW %[^,],%[^ \n]", key, key2) == 2) {
        if(integer(key2)){
        int exp = atoi(key2);
        poly_node_t *result = pow_poly(poly_table,key, exp);
        displayPolynomial(result);
        } else {
            printf("Invalid exponent '%s'. Exponent must be a non-negative integer.\n", key2);
        }
      } else if (sscanf(buffer, "AFFECT %[^,],%[^ \n]", key, key2) == 2) {
        printf("Key: %s\n", key);
        printf("Key2: %s\n", key2);
      } else if (sscanf(buffer, "DER %s", key) == 1) {
        printf("Key: %s\n", key);
      } else if (sscanf(buffer, "INT %s", key) == 1) {
        printf("Key: %s\n", key);
      } else if (sscanf(buffer, "EVAL %s for X=%s", key, key2) == 2) {
        if(integer(key2)){
        int exp = atoi(key2);
        printf("Result: %d\n",eval_poly(poly_table,key, exp));
        } else {
            printf("Invalid value for X='%s'. X must be a number.\n", key2);
        }
      } else if (strcasecmp(buffer, "HELP\n") == 0) {
        displayHelp();
      } else if (strcmp(buffer, "EXIT\n") == 0) {
        poly_table_delete(poly_table);
        break;
      } else {
        printf("Invalid input format\n");
      }

      free(key);
      free(key2);
      free(poly);
    } else {
      perror("Failed to read input");
    }
  }
  return 0;
}
