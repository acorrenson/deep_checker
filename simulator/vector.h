#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    int len;
    int cap;
    int *tab;
} vector;

vector *vector_create();
void vector_delete(vector *v);

int vector_get(vector *v, int i);
void vector_insert(vector *v, int x);
int vector_pop(vector *v);

void vector_print(vector *v);

#endif // VECTOR_H
