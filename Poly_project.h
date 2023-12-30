#ifndef POLY_PROJECT_H
#define POLY_PROJECT_H

#include "libc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct quot_s
{
	int up;
	int down;
 } quot;

typedef struct poly_node_s
{
	int pow;
	quot coff;
	struct poly_node_s *next;
} poly_node_t;


typedef struct hash_node_s
{
	char *key;
	poly_node_t **poly;
} hash_node_t;

typedef struct hash_table_s
{
    unsigned long int size;
    hash_node_t **array;
} hash_table_t;

hash_table_t *poly_table_create(size_t size);
poly_node_t *create_polynome(char *name);
void poly_set(hash_table_t *pt, char *key, char *new_name);
size_t poly_hash(char *name);
size_t poly_key_index(char *name, size_t size);
void poly_table_set(poly_name_table_t *pt, char *name);
void poly_table_print(hash_table_t *pt);
void poly_table_delete(hash_table_t *pt);

#endif
