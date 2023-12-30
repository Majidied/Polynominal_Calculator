#include "poly_project.h"

poly_table_t *poly_table_create(unsigned long int size) {
    poly_table_t *pt = malloc(sizeof(poly_table_t));
    if (pt == NULL) {
        perror("Failed to allocate memory for polynomial table");
        exit(EXIT_FAILURE);
    }

    pt->size = size;
    pt->array = calloc(size, sizeof(poly_node_t *));
    if (pt->array == NULL) {
        perror("Failed to allocate memory for polynomial table array");
        exit(EXIT_FAILURE);
    }

    return pt;
}

unsigned long int poly_hash(int degree) {
    // Simple hash function using the degree as the key
    // You might need a more sophisticated hash function depending on your requirements
    return (unsigned long int)degree;
}

unsigned long int poly_key_index(int degree, unsigned long int size) {
    return poly_hash(degree) % size;
}

int poly_table_set(poly_table_t *pt, int degree, double coefficient) {
    unsigned long int index = poly_key_index(degree, pt->size);

    poly_node_t *new_node = malloc(sizeof(poly_node_t));
    if (new_node == NULL) {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }

    new_node->degree = degree;
    new_node->coefficient = coefficient;
    new_node->next = pt->array[index];
    pt->array[index] = new_node;

    return 0;  // You might want to handle errors differently
}

double poly_table_get(const poly_table_t *pt, int degree) {
    unsigned long int index = poly_key_index(degree, pt->size);

    poly_node_t *current = pt->array[index];
    while (current != NULL) {
        if (current->degree == degree) {
            return current->coefficient;
        }
        current = current->next;
    }

    return 0.0;  // Default coefficient if not found
}

void poly_table_print(const poly_table_t *pt) {
    for (unsigned long int i = 0; i < pt->size; ++i) {
        poly_node_t *current = pt->array[i];
        while (current != NULL) {
            printf("(%dx^%d) ", (int)current->coefficient, current->degree);
            current = current->next;
        }
    }
    printf("\n");
}

void poly_table_delete(poly_table_t *pt) {
    for (unsigned long int i = 0; i < pt->size; ++i) {
        poly_node_t *current = pt->array[i];
        while (current != NULL) {
            poly_node_t *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(pt->array);
    free(pt);
}
