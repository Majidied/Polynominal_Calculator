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

/**
 * struct poly_name_node_s - Node of a polynomial with a name as a key
 *
 * @name: The name of the polynomial
 * @poly: A pointer to the polynomial represented by a linked list of terms
 * @next: A pointer to the next node of the PolynomialTable
 */
typedef struct poly_name_node_s
{
    char name[20];  // Assuming a maximum name length of 19 characters
    poly_node_t *poly;
    struct poly_name_node_s *next;
} poly_name_node_t;

/**
 * struct poly_name_table_s - Polynomial table with names as keys
 *
 * @size: The size of the array
 * @array: An array of size @size
 * Each cell of this array is a pointer to the first node of a linked list,
 * because we want our PolynomialTable to use a Chaining collision handling
 */
typedef struct poly_name_table_s
{
    unsigned long int size;
    poly_name_node_t **array;
} poly_name_table_t;

poly_name_table_t *poly_name_table_create(unsigned long int size);
unsigned long int poly_name_hash(const char *name);
unsigned long int poly_name_key_index(const char *name, unsigned long int size);
int poly_name_table_set(poly_name_table_t *pt, const char *name, int degree, double coefficient);
double poly_name_table_get(const poly_name_table_t *pt, const char *name, int degree);
void poly_name_table_print(const poly_name_table_t *pt);
void poly_name_table_delete(poly_name_table_t *pt);

#endif
