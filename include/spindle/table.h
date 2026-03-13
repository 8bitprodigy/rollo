#ifndef TABLE_H
#define TABLE_H


#include <stdint.h>

#include "dynamicarray.h"
#include "values.h"


SpindleTable *SpindleTable_merge( SpindleValue *left, SpindleValue       *right);
void          SpindleTable_append(SpindleTable *self, SpindleValue        value);
SpindleValue *SpindleTable_at(    SpindleTable *self, size_t              index);
SpindleValue *SpindleTable_get(   SpindleTable *self, const SpindleValue *key);
void          SpindleTable_put(   SpindleTable *self, SpindleValue        key, SpindleValue value)

#endif /* TABLE_H */
