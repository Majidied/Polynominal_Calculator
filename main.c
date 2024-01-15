#include "Poly_project.h"
int main()
{
    // Create a polynomial hash table with a size of 10
    hash_table_t *poly_table = poly_table_create(10);

    // Set polynomials in the table
    poly_table_set(poly_table, "P", " 2 + 2X +3/2X^2 - X^2");
    poly_table_set(poly_table, "Q", "5 - X^2 + 2/3X^3");

    // Print polynomials in the table
    poly_table_print(poly_table, "P");
    poly_table_print(poly_table, "Q");

    // Update polynomial "P11"
    poly_set(poly_table, "P", "2 + X^2");

    // Print the updated polynomial
    poly_table_print(poly_table, "P");

    // Delete the polynomial hash table
    poly_table_delete(poly_table);

    return 0;
}