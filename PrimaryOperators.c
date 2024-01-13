#include "Poly_project.h"

hash_table_t *poly_table_create(size_t size)
{
    hash_table_t *pt = malloc(sizeof(hash_table_t));
    if (pt == NULL) {
        perror("Failed to allocate memory for polynomial table");
        exit(EXIT_FAILURE);
    }

    pt->size = size;
    pt->array = calloc(size, sizeof(hash_node_t *));

    if (pt->array == NULL) {
        perror("Failed to allocate memory for polynomial table array");
        exit(EXIT_FAILURE);
    }

    return pt;
}

size_t poly_hash(char *name) {
    int c;
    size_t hash = 5381;


    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

size_t poly_key_index(char *name, size_t size)
{
    return poly_hash(name) % size;
}

poly_node_t *create_polynome(char *name)
{
    poly_node_t *monome, *head;
    int found_x;

    if (name == NULL || *name == '\0') {
        fprintf(stderr, "Error: Name is NULL\n");
        return NULL;
    }
    monome = malloc(sizeof(poly_node_t));
    if (!monome)
    {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }
    monome->next = NULL;
    monome->coff.down = 1;
    monome->coff.up = 0;
    head = monome;
    found_x = 0;
    while (*name != '\0')
    {
        if (*name <= '9' && *name >= '0' || *name == '-' || *name == '+')
        {
            if (found_x)
            {
                if (*(name - 1) == '^')
                {
                    monome->pow = atoi(name);
                    name += findlen(monome->pow);
                }
                else
                    monome->pow = 1;
                monome->next = malloc(sizeof(poly_node_t));
                if (!monome->next)
                {
                    perror("Failed to allocate memory for polynomial node");
                    exit(EXIT_FAILURE);
                }
                found_x = 0;
                monome = monome->next;
                monome->next = NULL;
                monome->coff.down = 1;
                monome->coff.up = 0;
                name++;
            }
            else
            {
                monome->coff.up = atoi(name);
                if (*name == '+')
                    name++;
                name += findlen(monome->coff.up);
                if (*name == '/')
                {
                    name++;
                    monome->coff.down = atoi(name);
                    name += findlen(monome->coff.down);
                }
                else
                    monome->coff.down = 1;
                if (*name != 'x' && *name != 'X')
                    monome->pow = 0;
            }
        }
        else if (*name == 'x' || *name == 'X')
        {
            if (!monome->coff.up)
                monome->coff.up = 1;
            found_x = 1;
            name++;
        }
        else
            name++;
    }
    return (head);
}

void poly_table_set(hash_table_t *pt, char *name)
{
    poly_node_t *polynome;
    hash_node_t *new_node;
    size_t      index;
    char        id[20];
    int         i = 0;
    while (*name != '=')
    {
        id[i++] = name[0];
        name++;
    }
    polynome = create_polynome(name);
    index = poly_key_index(id, pt->size);
    new_node = malloc(sizeof(hash_node_t));
    if (new_node == NULL)
    {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->key, id, strlen(id) - 1);
    new_node->key[strlen(new_node->key) - 1] = '\0';
    new_node->poly = malloc(sizeof(poly_node_t *));
    if (new_node->poly == NULL)
    {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }
    new_node->poly = &polynome;
    pt->array[index] = new_node;
}

void poly_table_print(hash_table_t *pt, char *key)
{
    size_t i = poly_key_index(key, pt->size);
    poly_node_t **tmp = pt->array[i]->poly;

    printf("%s :", key);
    while (!(*tmp))
    {
        if ((*tmp)->coff.up)
        {
            printf("%d", (*tmp)->coff.up);
            if ((*tmp)->coff.down != 1)
                printf("/%d", (*tmp)->coff.down);
        }
        if ((*tmp)->pow)
        {
            printf("x");
            if ((*tmp)->pow > 1)
                printf("^%d", (*tmp)->pow);
        }
    }
    printf("\n");
}

void poly_set(hash_table_t *pt, char *key, char *new_name)
{
    size_t i = poly_key_index(key, pt->size);
    poly_node_t **tmp = pt->array[i]->poly;
    poly_node_t *new;

    *(pt->array[i]->poly) = create_polynome(new_name);
    while (*tmp){
        new = (*tmp)->next;
        free(*tmp);
        *tmp = new;
    }
}

void poly_table_delete(hash_table_t *pt)
{
    for (size_t i = 0; i < pt->size; ++i)
    {
        hash_node_t *current = pt->array[i];
        hash_node_t *temp = current;
        current = current;
        free(temp->poly);
        free(temp);
    }

    free(pt->array);
    free(pt);
}
