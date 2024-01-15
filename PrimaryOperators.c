#include "Poly_project.h"
hash_table_t *poly_table_create(size_t size)
{
    hash_table_t *pt = malloc(sizeof(hash_table_t));
    if (pt == NULL)
    {
        perror("Failed to allocate memory for polynomial table");
        exit(EXIT_FAILURE);
    }

    pt->size = size;
    pt->array = calloc(size, sizeof(hash_node_t *));

    if (pt->array == NULL)
    {
        perror("Failed to allocate memory for polynomial table array");
        exit(EXIT_FAILURE);
    }

    return pt;
}

size_t poly_hash(char *name)
{
    int c;
    size_t hash = 5381;

    while ((c = *name++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

size_t poly_key_index(char *name, size_t size)
{
    return poly_hash(name) % size;
}

poly_node_t *addTerm(poly_node_t *poly, poly_node_t *term) {
    poly_node_t *current = poly;
    poly_node_t *prev = NULL;

    while (current != NULL && current->pow > term->pow) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->pow == term->pow) {
        current->coff.up = current->coff.up * term->coff.down + term->coff.up * current->coff.down;
        current->coff.down *= term->coff.down;
        simplifyFraction(&current->coff.up, &current->coff.down);

        free(term);
        return poly;
    }

    if (prev == NULL) {
        term->next = poly;
        return term;
    }
    prev->next = term;
    term->next = current;

    return poly;
}


void parseNumber(const char *str, int *numerator, int *denominator)
{
    char temp[100];
    int i = 0;

    while (*str != '+' && *str != '-' && *str != '\0')
    {
        temp[i] = *str;
        str++;
        i++;
    }

    temp[i] = '\0';

    if (strchr(temp, '/'))
    {
        sscanf(temp, "%d/%d", numerator, denominator);
    }
    else if (sscanf(temp, "%d", numerator) == 1)
    {
        *denominator = 1;
    }
    else
    {
        *numerator = 1;
        *denominator = 1;
    }
}

poly_node_t *create_polynome(char *input)
{
    poly_node_t *poly = NULL;

    while (*input == ' ')
    {
        input++;
    }

    while (*input != '\0')
    {
        int isNegative = 0;

        if (*input == '-')
        {
            isNegative = 1;
            input++;
        }
        else if (*input == '+')
        {
            input++;
        }

        poly_node_t *term = (poly_node_t *)malloc(sizeof(poly_node_t));
        term->pow = 0;
        term->next = NULL;

        char temp[100];
        int i = 0;

        while (*input != '+' && *input != '-' && *input != '\0')
        {
            temp[i] = *input;
            input++;
            i++;
        }

        temp[i] = '\0';

        char *varPart = strchr(temp, 'X');

        if (varPart != NULL)
        {
            if (sscanf(varPart, "X^%d", &term->pow) != 1)
            {
                term->pow = 1;
            }
        }

        int coefNumerator, coefDenominator;
        parseNumber(temp, &coefNumerator, &coefDenominator);

        term->coff.up = isNegative ? -coefNumerator : coefNumerator;
        term->coff.down = coefDenominator;

        poly = addTerm(poly, term);

        size_t length = strcspn(input, "+-");
        input += length;
    }
    return poly;
}

void poly_table_set(hash_table_t *pt, char *id, char *name)
{
    // Create a new polynomial node and copy the polynomial data
    poly_node_t *polynome = create_polynome(name);

    // Find the index in the hash table array
    size_t index = poly_key_index(id, pt->size);

    // Create a new hash node and allocate memory for the key
    hash_node_t *new_node = malloc(sizeof(hash_node_t));
    if (new_node == NULL)
    {
        perror("Failed to allocate memory for polynomial node");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the key and copy it
    new_node->key = malloc(strlen(id) + 1);
    if (new_node->key == NULL)
    {
        perror("Failed to allocate memory for key");
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->key, id);

    // Set the polynomial node in the hash node
    new_node->poly = polynome;

    // Set the hash node in the hash table array
    pt->array[index] = new_node;
}

void displayPolynomial(const poly_node_t *poly)
{
    const poly_node_t *currentTerm = poly;
    int isFirstTerm = 1;

    while (currentTerm != NULL)
    {
        if (!isFirstTerm)
        {
            if (currentTerm->coff.up >= 0)
            {
                printf(" + ");
            }
            else
            {
                printf(" - ");
            }
        }
        else if (currentTerm->coff.up < 0)
        {
            printf("-");
        }

        if (currentTerm->pow == 0)
        {
            if (currentTerm->coff.down != 1)
            {
                printf("%d/%d", abs(currentTerm->coff.up), abs(currentTerm->coff.down));
            }
            else
            {
                printf("%d", abs(currentTerm->coff.up));
            }
        }
        else
        {
            if (abs(currentTerm->coff.up) != 1 || currentTerm->pow == 0)
            {
                printf("%d", abs(currentTerm->coff.up));
            }

            

            if (currentTerm->coff.down != 1 && currentTerm->pow != 0)
            {
                if (currentTerm->coff.up == 1)
            {
                printf("%d", currentTerm->coff.up);
            }
                printf("/%d", currentTerm->coff.down);
            }

            if (currentTerm->pow != 0)
            {
                printf("X");
            }

            if (currentTerm->pow > 1)
            {
                printf("^%d", currentTerm->pow);
            }
        }

        isFirstTerm = 0;
        currentTerm = currentTerm->next;
    }

    printf("\n");
}

void poly_table_print(hash_table_t *pt, char *key)
{
    size_t i = poly_key_index(key, pt->size);
    poly_node_t *tmp = pt->array[i]->poly;

    printf("%s : ", key);
    displayPolynomial(tmp);
}

void poly_set(hash_table_t *pt, char *key, char *new_name)
{
    size_t i = poly_key_index(key, pt->size);
    poly_node_t *tmp = pt->array[i]->poly;
    poly_node_t *new_poly = create_polynome(new_name);

    // Free the existing linked list
    while (tmp != NULL)
    {
        poly_node_t *next = tmp->next;
        free(tmp);
        tmp = next;
    }

    // Update the pointer to the new polynomial
    pt->array[i]->poly = new_poly;
}

void poly_table_delete(hash_table_t *pt)
{
    for (size_t i = 0; i < pt->size; ++i)
    {
        hash_node_t *current = pt->array[i];
        hash_node_t *temp = current;
        current = current;
        free(temp->key);
        free(temp->poly);
        free(temp);
    }

    free(pt->array);
    free(pt);
}

