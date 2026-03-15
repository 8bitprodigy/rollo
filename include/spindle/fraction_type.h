#ifndef SPINDLE_FRACTION_TYPE_H
#define SPINDLE_FRACTION_TYPE_H

#include <gimbal/meta/gimbal_type.h>
#include <gimbal/meta/gimbal_ivariant.h>


#define SPINDLE_FRACTION_TYPE (SpindleFraction_type())


typedef struct 
SpindleFraction 
{
	int64_t
		numerator,
		denominator;
}
SpindleFraction;

GblType SpindleFraction_type(void);

#endif /* SPINDLE_FRACTION_TYPE_H */
