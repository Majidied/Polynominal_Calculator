#ifndef POLY_PROJECT_H
#define POLY_PROJECT_H
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct quot_s {
  int up;
  int down;
} quot;

typedef struct poly_node_s {
  int pow;
  quot coff;
  struct poly_node_s *next;
} poly_node_t;

typedef struct hash_node_s {
  char *key;
  poly_node_t *poly;
} hash_node_t;

typedef struct hash_table_s {
  unsigned long int size;
  hash_node_t **array;
} hash_table_t;

bool          is_valid_polynomial(const char *expression);
hash_table_t  *poly_table_create(size_t size);
poly_node_t   *create_polynome(char *name);
void          poly_set(hash_table_t *pt, char *key, char *new_name);
size_t        poly_hash(char *name);
size_t        poly_key_index(char *name, size_t size);
void          poly_table_set(hash_table_t *pt, char *id, char *name);
void          poly_table_print(hash_table_t *pt, char *key);
void          poly_table_delete(hash_table_t *pt);
void          simplifyFraction(int *numerator, int *denominator);
int           findGCD(int a, int b);
bool          integer(char *key);
poly_node_t   *multiplication(hash_table_t *table, char *key1, char *key2);
void          displayPolynomial(const poly_node_t *poly);
poly_node_t   *addTerm(poly_node_t *poly, poly_node_t *term);
poly_node_t   *multiplyPolynomials(const poly_node_t *poly1, const poly_node_t *poly2);
poly_node_t   *pow_poly(hash_table_t *pt, char *key, int pw);
float         eval_poly(hash_table_t *pt, char *key, float x);
poly_node_t   *inter_poly(hash_table_t *pt, char *key);
poly_node_t   *sub_poly(hash_table_t *pt, char *key1, char *key2);
poly_node_t   *der_poly(hash_table_t *pt, char *key);
poly_node_t   *add_poly(hash_table_t *pt, char *key1, char *key2);
void           aff_poly(hash_table_t *pt, char *key1, char *key2);
#endif
