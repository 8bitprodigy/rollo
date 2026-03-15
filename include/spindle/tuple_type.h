#ifndef SPINDLE_PAIR_TYPE_H
#define SPINDLE_PAIR_TYPE_H

#include "values.h"
#include <gimbal/meta/gimbal_type.h>
#include <gimbal/meta/gimbal_ivariant.h>


#define SPINDLE_PAIR_TYPE (SpindlePair_type())


typedef struct
SpindlePAIR
{
	GblVariant
		key,
		value;
}

GblType SpindlePair_type(void);

#endif /* SPINDLE_PAIR_TYPE_H */
