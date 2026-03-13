#ifndef SPINDLE_VALUES_H
#define SPINDLE_VALUES_H


#include <cstdint>
#include <stdbool.h>
#include <stdint.h>

#include "btchwrk.h"


typedef enum
SpindleValueType
{
	/* Primitive valuess: */
	SPINDLE_TYPE_NIL = 0,
	SPINDLE_TYPE_CHARACTER,
	SPINDLE_TYPE_SYMBOL,
	SPINDLE_TYPE_BOOL,
	SPINDLE_TYPE_INTEGER,
	SPINDLE_TYPE_FRACTION,
	SPINDLE_TYPE_DECIMAL,
	/* Containers: */
	SPINDLE_TYPE_STRING = 6,
	SPINDLE_TYPE_ARRAY,
	SPINDLE_TYPE_LIST,
	SPINDLE_TYPE_TUPLE,
	SPINDLE_TYPE_MAP,
	SPINDLE_TYPE_TABLE,
	/* Callable */
	SPINDLE_TYPE_BLOCK,
	SPINDLE_TYPE_CALLABLE,
	/* Meta: */
	SPINDLE_TYPE_NUM_SPINDLE_TYPES
}
SpindleValueType;


typedef struct SpindleObject SpindleObject;
typedef struct SpindleValue  SpindleValue;
typedef struct SpindleTuple  SpindleTuple;
typedef struct SpindleTable  SpindleTable;


typedef struct
SpindleFraction
{
	intmax_t
		numerator,
		denominator;
}
SpindleFraction;

typedef struct
SpindleTuple
{
	SpindleValue
		key,
		value;
}
SpindleTuple;

typedef SpindleValue (*SpindleFunction) (SpindleValue args);

typedef union
SpindleFlags
{
	uint8_t flags;
	struct {
		bool uninitialized: 1;
		/* 1-7 reserved */
	}
}

/* Stack Allocated */
typedef struct
SpindleValue
{
	SpindleValueType type;
	union {
		bool             boolean;
		char             character;
		Symbol           symbol;
		intmax_t         integer;
		double           decimal;
		SpindleFraction  fraction;
		Text            *string;
		SpindleObject   *array;
		SpindleObject   *list;
		SpindleTuple    *tuple;
		SpindleTable    *map;
		SpindleTable    *table;
		SpindleFunction *callable;
	};
	SpindleFlags flags;
}
SpindleValue;

/* Heap Allocated */
typedef struct
SpindleObject
{
	SpindleValue  value;
	atomic_size_t references;
}
SpindleObject;


#endif /* SPINDLE_VALUES_H */

