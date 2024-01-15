#include "Poly_project.h"

quot    fract_sum(quot n1, quot n2)
{
    quot sum;

    if (n1.down == 1 && n2.down == 1)
    {
        sum.up = n1.up + n2.up;
        sum.down = 1;
    }
    else if (n1.up && n2.up)
    {
        sum.up = n1.up * n2.down + n1.down * n2.up;
        sum.down = n1.down * n2.down;
    }
    else if (n1.up)
        sum = n2;
    else
        sum = n1;
    simplifyFraction(&(sum.up), &(sum.down));
    return (sum);
}

quot    fract_sub(quot n1, quot n2)
{
    quot sum;

    if (n1.down == 1 && n2.down == 1)
    {
        sum.up = n1.up - n2.up;
        sum.down = 1;
    }
    else if (n1.up && n2.up)
    {
        sum.up = n1.up * n2.down - n1.down * n2.up;
        sum.down = n1.down * n2.down;
    }
    else if (n1.up)
        sum = n2;
    else
        sum = n1;
    simplifyFraction(&(sum.up), &(sum.down));
    return (sum);
}

void    free_polynode(poly_node_t *sum)
{
    poly_node_t *next;

    while (sum)
    {
        next = sum->next;
        free(sum);
        sum = next;
    }
}

poly_node_t *add_poly(poly_node_t *p1, poly_node_t *p2)
{
    poly_node_t *sum = malloc(sizeof(poly_node_t));

    if (!sum || (!p1 && !p2))
    {
        perror("memory alloacation failed");
        return (NULL);
    }
    sum->next = NULL;
    poly_node_t *tmp = sum;
    while (p1 || p2)
    {
        if (p1 && p2 && p1->pow == p2->pow)
        {
            tmp->pow = p1->pow;
            tmp->coff = fract_sum(p1->coff, p2->coff);
            p1 = p1->next;
            p2 = p2->next;
        }
        else if ((p1 && !p2) || p1->pow > p2->pow)
        {
            tmp->pow = p1->pow;
            tmp->coff = p1->coff;
            p1 = p1->next;
        }
        else if (p2)
        {
            tmp->pow = p2->pow;
            tmp->coff = p2->coff;
            p2 = p2->next;
        }
        tmp->next = malloc(sizeof(poly_node_t));
        if (!tmp->next)
        {
            free_polynode(sum);
            perror("Memory allocation failed in add_poly()\n");
            return (NULL);
        }
        tmp = tmp->next;
        tmp->next = NULL;
    }
    return (sum);
}

poly_node_t *sub_poly(poly_node_t *p1, poly_node_t *p2)
{
    poly_node_t *sub = malloc(sizeof(poly_node_t));

    if (!sub)
    {
        perror("memory alloacation failed");
        return (NULL);
    }
    sub->next = NULL;
    poly_node_t *tmp = sub;
    while (p1 || p2)
    {
        if (p1 && p2 && p1->pow == p2->pow)
        {
            if (p1->coff.up != p2->coff.up || p1->coff.down != p2->coff.down)
            {
                tmp->pow = p1->pow;
                tmp->coff = fract_sub(p1->coff, p2->coff);
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        else if ((p1 && !p2) || p1->pow > p2->pow)
        {
            tmp->pow = p1->pow;
            tmp->coff = p1->coff;
            p1 = p1->next;
        }
        else if (p2)
        {
            tmp->pow = p2->pow;
            tmp->coff.up = -p2->coff.up;
            tmp->coff.down = p2->coff.down;
            p2 = p2->next;
        }
        tmp->next = malloc(sizeof(poly_node_t));
        if (!tmp->next)
        {
            free_polynode(sub);
            perror("Memory allocation failed in add_poly()\n");
            return (NULL);
        }
        tmp = tmp->next;
        tmp->next = NULL;
    }
    return (sub);
}

poly_node_t *pow_poly(poly_node_t *p1, int pw)
{
    poly_node_t *pow = malloc(sizeof(poly_node_t));

    if (!pow || !p1)
    {
        perror("memory alloacation failed");
        return (NULL);
    }
    pow->next = NULL;
    poly_node_t *tmp = pow;
    while (p1)
    {
        tmp->pow = p1->pow * pw;
        tmp->coff = p1->coff;
        p1 = p1->next;
        tmp->next = malloc(sizeof(poly_node_t));
        if (!tmp->next)
        {
            free_polynode(pow);
            perror("Memory allocation failed in add_poly()\n");
            return (NULL);
        }
        tmp = tmp->next;
        tmp->next = NULL;
    }
    return (pow);
}

poly_node_t *aff_poly(poly_node_t *p1)
{
    poly_node_t *cpy = malloc(sizeof(poly_node_t));

    if (!cpy || !p1)
    {
        perror("memory alloacation failed");
        return (NULL);
    }
    cpy->next = NULL;
    poly_node_t *tmp = cpy;
    while (p1)
    {
        tmp->pow = p1->pow;
        tmp->coff = p1->coff;
        p1 = p1->next;
        tmp->next = malloc(sizeof(poly_node_t));
        if (!tmp->next)
        {
            free_polynode(cpy);
            perror("Memory allocation failed in add_poly()\n");
            return (NULL);
        }
        tmp = tmp->next;
        tmp->next = NULL;
    }
    return (cpy);
}

poly_node_t *der_poly(poly_node_t *p1)
{
    poly_node_t *der = malloc(sizeof(poly_node_t));

    if (!der || !p1)
    {
        perror("memory alloacation failed");
        return (NULL);
    }
    der->next = NULL;
    poly_node_t *tmp = der;
    while (p1)
    {
        if (p1->pow)
        {
            tmp->pow = p1->pow - 1;
            tmp->coff.up = p1->coff.up * p1->pow;
            tmp->coff.down = p1->coff.down;
            simplifyFraction(&tmp->coff.up, &tmp->coff.down);
            p1 = p1->next;
            tmp->next = malloc(sizeof(poly_node_t));
        }
        else
        {
            tmp->pow = 0;
            tmp->coff.up = 0;
            tmp->coff.up = 1;
            p1 = p1->next;
            tmp->next = malloc(sizeof(poly_node_t));
        }
        if (!tmp->next)
        {
            free_polynode(der);
            perror("Memory allocation failed in add_poly()\n");
            return (NULL);
        }
        tmp = tmp->next;
        tmp->next = NULL;
    }
    return (der);
}

poly_node_t *inter_poly(poly_node_t *p1)
{
    poly_node_t *inter = malloc(sizeof(poly_node_t));

    if (!inter || !p1)
    {
        perror("memory alloacation failed");
        return (NULL);
    }
    inter->next = NULL;
    poly_node_t *tmp = inter;
    while (p1)
    {
        if (p1->pow)
        {
            tmp->pow = p1->pow - 1;
            tmp->coff.up = p1->coff.up;
            tmp->coff.down = p1->coff.down * tmp->pow;
            simplifyFraction(&tmp->coff.up, &tmp->coff.down);
            p1 = p1->next;
            tmp->next = malloc(sizeof(poly_node_t));
        }
        else
        {
            tmp->pow = 1;
            tmp->coff = p1->coff;
            p1 = p1->next;
            tmp->next = malloc(sizeof(poly_node_t));
        }
        if (!tmp->next)
        {
            free_polynode(inter);
            perror("Memory allocation failed in add_poly()\n");
            return (NULL);
        }
        tmp = tmp->next;
        tmp->next = NULL;
    }
    return (inter);
}

int pow(int x, int p)
{
    if (p == 0)
        return (1);
    return (x * pow(x, p - 1));
}

int eval_poly(poly_node_t *p1, int x)
{
    int res = 0;

    while(p1)
    {
        res += p1->coff.up / p1->coff.down * pow(x, p1->pow);
        p1 = p1->next;
    }
    return (res);
}
