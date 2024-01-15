#include "Poly_project.h"
int main()
{
    hash_table_t *poly_table = poly_table_create(10);

    poly_table_set(poly_table, "P", " 2 +2 +2 +2 + 2X +3/2X^2 - X^2");
    poly_table_set(poly_table, "Q", "5 - X^2 + 2/3X^3");


    poly_table_print(poly_table, "P");
    poly_table_print(poly_table, "Q");

    poly_set(poly_table, "P", "2 + X^2");

    poly_table_print(poly_table, "P");

    poly_node_t *result = multiplication(poly_table, "P", "Q");

    displayPolynomial(result);

    poly_table_delete(poly_table);

    return 0;
}
