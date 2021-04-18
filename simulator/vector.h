#ifndef VECTOR_H
#define VECTOR_H

typedef unsigned vector_t;

typedef struct {
  int len;
  int cap;
  vector_t *tab;
} vector;

vector *vector_create();
void vector_delete(vector *v);

void vector_insert(vector *v, vector_t x);
vector_t vector_pop(vector *v);

#endif // VECTOR_H
