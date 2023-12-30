#ifndef POLY_PROJECT_H
#define POLY_PROJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * struct poly_node_s - Node of a polynomial
 *
 * @degree: The degree of the term
 * @coefficient: The coefficient of the term
 * @next: A pointer to the next node of the Polynomial
 */
typedef struct poly_node_s
{
    int degree;
    double coefficient;
    struct poly_node_s *next;
} poly_node_t;

/**
 * struct poly_table_s - Polynomial table data structure
 *
 * @size: The size of the array
 * @array: An array of size @size
 * Each cell of this array is a pointer to the first node of a linked list,
 * because we want our PolynomialTable to use a Chaining collision handling
 */
typedef struct poly_table_s
{
    unsigned long int size;
    poly_node_t **array;
} poly_table_t;

poly_table_t *poly_table_create(unsigned long int size);
unsigned long int poly_hash(int degree);
unsigned long int poly_key_index(int degree, unsigned long int size);
int poly_table_set(poly_table_t *pt, int degree, double coefficient);
double poly_table_get(const poly_table_t *pt, int degree);
void poly_table_print(const poly_table_t *pt);
void poly_table_delete(poly_table_t *pt);

#endif
