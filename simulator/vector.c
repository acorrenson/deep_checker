#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

vector *vector_create()
{
    vector *v = malloc(sizeof(vector));
    v->len = 0;
    v->cap = 0;
    v->tab = malloc(0);
    return v;
}

void vector_delete(vector *v)
{
    free(v->tab);
    free(v);
}

int vector_get(vector *v, int i)
{
    return v->tab[i];
}

void vector_insert(vector *v, int x)
{
    if (v->len == v->cap)
    {
        v->cap = (v->cap == 0 ? 4 : v->cap * 2);
        v->tab = realloc(v->tab, v->cap * sizeof(x));
    }
    v->tab[v->len] = x;
    v->len++;
}

int vector_pop(vector *v)
{
    v->len--;
    return v->tab[v->len];
}

void vector_print(vector *v)
{
    for (int i = 0; i < v->len; i++)
    {
        printf("%d\n", v->tab[i]);
    }
}
