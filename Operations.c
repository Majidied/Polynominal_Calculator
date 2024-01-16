#include "Poly_project.h"

quot fract_sum(quot n1, quot n2) {
  quot sum;

  if (n1.down == 1 && n2.down == 1) {
    sum.up = n1.up + n2.up;
    sum.down = 1;
  } else if (n1.up && n2.up) {
    sum.up = n1.up * n2.down + n1.down * n2.up;
    sum.down = n1.down * n2.down;
  } else if (n1.up)
    sum = n2;
  else
    sum = n1;
  simplifyFraction(&(sum.up), &(sum.down));
  return (sum);
}

quot fract_sub(quot n1, quot n2) {
  quot sum;

  if (n1.down == 1 && n2.down == 1) {
    sum.up = n1.up - n2.up;
    sum.down = 1;
  } else if (n1.up && n2.up) {
    sum.up = n1.up * n2.down - n1.down * n2.up;
    sum.down = n1.down * n2.down;
  } else if (n1.up)
    sum = n2;
  else
    sum = n1;
  simplifyFraction(&(sum.up), &(sum.down));
  return (sum);
}

void free_polynode(poly_node_t *sum) {
  poly_node_t *next;

  while (sum) {
    next = sum->next;
    free(sum);
    sum = next;
  }
}

poly_node_t *add_poly(hash_table_t *pt, char *key1, char *key2) {
  size_t i = poly_key_index(key1, pt->size);
  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key1);
    return 0;
  }
  size_t j = poly_key_index(key2, pt->size);
  if (pt->array[j] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key2);
    return 0;
  }
  poly_node_t *p1 = pt->array[i]->poly;
  poly_node_t *p2 = pt->array[j]->poly;
  poly_node_t *sum = malloc(sizeof(poly_node_t));
  if (!sum || (!p1 && !p2)) {
    perror("memory alloacation failed");
    return (NULL);
  }
  sum->next = NULL;
  poly_node_t *tmp = sum;
  while (p1 || p2) {
    if (p1 && p2 && p1->pow == p2->pow) {
      tmp->pow = p1->pow;
      tmp->coff = fract_sum(p1->coff, p2->coff);
      p1 = p1->next;
      p2 = p2->next;
    } else if ((p1 && !p2) || p1->pow > p2->pow) {
      tmp->pow = p1->pow;
      tmp->coff = p1->coff;
      p1 = p1->next;
    } else if (p2) {
      tmp->pow = p2->pow;
      tmp->coff = p2->coff;
      p2 = p2->next;
    }
    tmp->next = malloc(sizeof(poly_node_t));
    if (!tmp->next) {
      free_polynode(sum);
      perror("Memory allocation failed in add_poly()\n");
      return (NULL);
    }
    if (p1 || p2)
      tmp = tmp->next;
    tmp->next = NULL;
  }
  return (sum);
}

poly_node_t *sub_poly(hash_table_t *pt, char *key1, char *key2) {
  size_t i = poly_key_index(key1, pt->size);
  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key1);
    return 0;
  }
  size_t j = poly_key_index(key2, pt->size);
  if (pt->array[j] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key2);
    return 0;
  }
  poly_node_t *p1 = pt->array[i]->poly;
  poly_node_t *p2 = pt->array[j]->poly;
  poly_node_t *sub = malloc(sizeof(poly_node_t));
  if (!sub) {
    perror("memory alloacation failed");
    return (NULL);
  }
  sub->next = NULL;
  poly_node_t *tmp = sub;
  int ind = 0;
  while (p1 || p2) {
    if (p1 && p2 && p1->pow == p2->pow) {
      if (p1->coff.up != p2->coff.up || p1->coff.down != p2->coff.down) {
        tmp->pow = p1->pow;
        tmp->coff = fract_sub(p1->coff, p2->coff);
        ind++;
      }
      p1 = p1->next;
      p2 = p2->next;
    } else if ((p1 && !p2) || ((p1 && p2) && p1->pow > p2->pow)) {
      tmp->pow = p1->pow;
      tmp->coff = p1->coff;
      p1 = p1->next;
      ind++;
    } else if (p2) {
      tmp->pow = p2->pow;
      tmp->coff.up = -p2->coff.up;
      tmp->coff.down = p2->coff.down;
      p2 = p2->next;
      ind++;
    }
    tmp->next = malloc(sizeof(poly_node_t));
    if (!tmp->next) {
      free_polynode(sub);
      perror("Memory allocation failed in add_poly()\n");
      return (NULL);
    }
    if ((p1 || p2) && ind) {
      ind = 0;
      tmp = tmp->next;
    }
    tmp->next = NULL;
  }
  return (sub);
}

poly_node_t *pow_poly(hash_table_t *pt, char *key, int pw) {
  size_t i = poly_key_index(key, pt->size);
  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name exists.\n");
    return NULL;
  }
  poly_node_t *tmp = pt->array[i]->poly;
  for (int j = 1; j < pw; j++) {
    tmp = multiplyPolynomials(tmp, pt->array[i]->poly);
  }
  return (tmp);
}

void aff_poly(hash_table_t *pt, char *key1, char *key2) {
  size_t i = poly_key_index(key1, pt->size);
  size_t j = poly_key_index(key2, pt->size);

  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key1);
    return ; // Change to NULL since it's a pointer return type
  }

  poly_node_t *p1 = pt->array[i]->poly;
  hash_node_t *new_node = malloc(sizeof(hash_node_t));
  if (new_node == NULL) {
    perror("Failed to allocate memory for polynomial node");
    exit(EXIT_FAILURE);
  }
  new_node->key = key2;

  poly_node_t *cpy = malloc(sizeof(poly_node_t));
  if (!cpy) {
    perror("Memory allocation failed");
    return ;
  }

  cpy->next = NULL;
  poly_node_t *tmp = cpy;

  while (p1) {
    tmp->pow = p1->pow;
    tmp->coff = p1->coff;

    p1 = p1->next;

    if (p1) {
      tmp->next = malloc(sizeof(poly_node_t));
      if (!tmp->next) {
        free_polynode(cpy);
        perror("Memory allocation failed in aff_poly()\n");
        return ;
      }
      tmp = tmp->next;
      tmp->next = NULL;
    }
  }

  new_node->poly = cpy;
  pt->array[j] = new_node;
}

poly_node_t *der_poly(hash_table_t *pt, char *key) {
  size_t i = poly_key_index(key, pt->size);
  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key);
    return 0;
  }
  poly_node_t *p1 = pt->array[i]->poly;
  poly_node_t *der = malloc(sizeof(poly_node_t));
  if (!der || !p1) {
    perror("memory alloacation failed");
    return (NULL);
  }
  der->next = NULL;
  poly_node_t *tmp = der;
  while (p1) {
    if (p1->pow) {
      tmp->pow = p1->pow - 1;
      tmp->coff.up = p1->coff.up * p1->pow;
      tmp->coff.down = p1->coff.down;
      simplifyFraction(&tmp->coff.up, &tmp->coff.down);
      tmp->next = malloc(sizeof(poly_node_t));
    } else {
      tmp->pow = 0;
      tmp->coff.up = 0;
      tmp->coff.up = 1;
      tmp->next = malloc(sizeof(poly_node_t));
    }
    if (!tmp->next) {
      free_polynode(der);
      perror("Memory allocation failed in add_poly()\n");
      return (NULL);
    }
    if (p1->next)
      tmp = tmp->next;
    p1 = p1->next;
    tmp->next = NULL;
  }
  return (der);
}

poly_node_t *inter_poly(hash_table_t *pt, char *key) {

  size_t i = poly_key_index(key, pt->size);
  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key);
    return 0;
  }
  poly_node_t *p1 = pt->array[i]->poly;
  poly_node_t *inter = malloc(sizeof(poly_node_t));
  if (!inter || !p1) {
    perror("memory alloacation failed");
    return (NULL);
  }
  inter->next = NULL;
  poly_node_t *tmp = inter;
  while (p1) {
    if (p1->pow) {
      tmp->pow = p1->pow + 1;
      tmp->coff.up = p1->coff.up;
      tmp->coff.down = p1->coff.down * tmp->pow;
      simplifyFraction(&tmp->coff.up, &tmp->coff.down);
      tmp->next = malloc(sizeof(poly_node_t));
    } else {
      tmp->pow = 1;
      tmp->coff = p1->coff;
      tmp->next = malloc(sizeof(poly_node_t));
    }
    if (!tmp->next) {
      free_polynode(inter);
      perror("Memory allocation failed in add_poly()\n");
      return (NULL);
    }
    if (p1->next)
      tmp = tmp->next;
    p1 = p1->next;
    tmp->next = NULL;
  }
  return (inter);
}

float power(float x, int p) {
  if (p == 0)
    return (1);
  return (x * power(x, p - 1));
}

float eval_poly(hash_table_t *pt, char *key, float x) {
  size_t i = poly_key_index(key, pt->size);
  if (pt->array[i] == NULL) {
    printf("No polynomial with the given name '%s' exists.\n", key);
    return 0;
  }

  poly_node_t *p1 = pt->array[i]->poly;
  float res = 0;

  while (p1) {
    res +=
        ((float)p1->coff.up * (float)power(x, p1->pow)) / (float)p1->coff.down;
    p1 = p1->next;
  }

  return res;
}
