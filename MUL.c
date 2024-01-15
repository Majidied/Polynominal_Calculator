#include "Poly_project.h"
poly_node_t *multiplyPolynomials(const poly_node_t *poly1, const poly_node_t *poly2) {
    if (poly1 == NULL || poly2 == NULL) {
        return NULL;
    }

    poly_node_t *result = NULL;

    for (const poly_node_t *term1 = poly1; term1 != NULL; term1 = term1->next) {
        for (const poly_node_t *term2 = poly2; term2 != NULL; term2 = term2->next) {
            poly_node_t *productTerm = (poly_node_t *)malloc(sizeof(poly_node_t));
            if (productTerm == NULL) {
                perror("Failed to allocate memory for product term");
                exit(EXIT_FAILURE);
            }

            productTerm->coff.up = term1->coff.up * term2->coff.up;
            productTerm->coff.down = term1->coff.down * term2->coff.down;
            productTerm->pow = term1->pow + term2->pow;
            productTerm->next = NULL;

            result = addTerm(result, productTerm);
        }
    }

    return result;
}



poly_node_t *multiplication(hash_table_t *table, char *key1, char *key2){
    size_t i = poly_key_index(key1, table->size), j = poly_key_index(key2, table->size);
    poly_node_t *tmp1 = table->array[i]->poly;
    poly_node_t *tmp2 = table->array[j]->poly;
    return multiplyPolynomials(tmp1, tmp2);
}
