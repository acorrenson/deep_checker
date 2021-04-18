#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

vector *vector_create() {
  vector *v = malloc(sizeof(vector));
  v->len = 0;
  v->cap = 0;
  v->tab = malloc(0);
  return v;
}

void vector_delete(vector *v) {
  free(v->tab);
  free(v);
}

void vector_insert(vector *v, vector_t x) {
  if (v->len == v->cap) {
    v->cap = (v->cap == 0 ? 4 : v->cap * 2);
    v->tab = realloc(v->tab, v->cap * sizeof(x));
  }
  v->tab[v->len] = x;
  v->len++;
}

vector_t vector_pop(vector *v) {
  v->len--;
  return v->tab[v->len];
}
