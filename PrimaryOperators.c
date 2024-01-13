#include "poly_project.h"

poly_name_table_t *poly_name_table_create(unsigned long int size) {
    poly_name_table_t *pt = malloc(sizeof(poly_name_table_t));
    if (pt == NULL) {
        perror("Failed to allocate memory for polynomial table");
        exit(EXIT_FAILURE);
    }

    pt->size = size;
    pt->array = calloc(size, sizeof(poly_name_node_t *));
    if (pt->array == NULL) {
        perror("Failed to allocate memory for polynomial table array");
        exit(EXIT_FAILURE);
    }

    return pt;
}

unsigned long int poly_name_hash(const char *name) {
    unsigned long int hash = 5381;
    int c;

    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

unsigned long int poly_name_key_index(const char *name, unsigned long int size) {
    return poly_name_hash(name) % size;
}

int poly_name_table_set(poly_name_table_t *pt, const char *name, int degree, double coefficient) {
    if (name == NULL) {
        fprintf(stderr, "Error: Name is NULL\n");
        return -1;
    }
    unsigned long int index = poly_name_key_index(name, pt->size);

    poly_name_node_t *new_node = malloc(sizeof(poly_name_node_t));
    if (new_node == NULL) {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }

    strncpy(new_node->name, name, sizeof(new_node->name) - 1);
    new_node->name[sizeof(new_node->name) - 1] = '\0';
    new_node->poly = malloc(sizeof(poly_node_t));
    if (new_node->poly == NULL) {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }

    new_node->poly->degree = degree;
    new_node->poly->coefficient = coefficient;
    new_node->poly->next = pt->array[index];
    pt->array[index] = new_node;

    return 0;
}
double poly_name_table_get(const poly_name_table_t *pt, const char *name, int degree) {
    unsigned long int index = poly_name_key_index(name, pt->size);

    poly_name_node_t *current = pt->array[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->poly->degree == degree) {
            return current->poly->coefficient;
        }
        current = current->next;
    }

    return 0.0;
}

void poly_name_table_print(const poly_name_table_t *pt) {
    for (unsigned long int i = 0; i < pt->size; ++i) {
        poly_name_node_t *current = pt->array[i];
        while (current != NULL) {
            printf("%s: (%dx^%d) ", current->name, (int)current->poly->coefficient, current->poly->degree);
            current = current->next;
        }
    }
    printf("\n");
}

void poly_name_table_delete(poly_name_table_t *pt) {
    for (unsigned long int i = 0; i < pt->size; ++i) {
        poly_name_node_t *current = pt->array[i];
        while (current != NULL) {
            poly_name_node_t *temp = current;
            current = current->next;
            free(temp->poly);
            free(temp);
        }
    }

    free(pt->array);
    free(pt);
}
