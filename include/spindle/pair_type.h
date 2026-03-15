#ifndef SPINDLE_PAIR_TYPE_H
#define SPINDLE_PAIR_TYPE_H

#include "values.h"
#include <gimbal/meta/gimbal_type.h>
#include <gimbal/meta/gimbal_ivariant.h>


#define SPINDLE_PAIR_TYPE (SpindleTuple_type())


typedef struct
SpindlePAIR
{
	GblVariant
		key,
		value;
}


#endif /* SPINDLE_PAIR_TYPE_H */
