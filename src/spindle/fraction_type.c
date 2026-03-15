#include "fraction_type.h"


static GblType s_fractionType = GBL_INVALID_TYPE;

static GBL_RESULT 
fraction_construct(
	GblVariant *pVariant,
	size_t argc,
	GblVariant *pArgs,
	GBL_IVARIANT_OP_FLAGS op
)
{
	GBL_UNUSED(argc, pArgs);
	if (op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
		SpindleFraction * pfrac = GBL_ALLOC(sizeof(SpindleFraction));
		pfrac->numerator   = 0;
		pfrac->denominator = 1;
		pVariant->pVoid    = pfrac;
	}
	else if (op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY) {
		SpindleFraction *pfrac = GBL_ALLOC(sizeof(SpindleFraction));
		pfrac->numerator   = GblVariant_toInt64(&pArgs[0]);
		pfrac->denominator = GblVariant_toInt64(&pargs[1]);
		pVariant->pVoid    = pfrac;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
fraction_destruct( GblVariant *pVariant) 
{
	GBL_FREE(pVariant->pVoid);
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
fraction_compare(
	const GblVariant *pVariant,
	const GblVariant *pOther,
	int              *pResult
)
{
	const SpindleFraction
		*a = pVariant->pVoid,
		*b = pOther->pVoid;

	/* Cross multiply to compare: a/b vs c/d => a*d vs c*b */
	int64_t 
		lhs = a->numerator * b->denominator,
		rhs = b->numerator * a->denominator;

	*pResult = (lhs > rhs) - (lhs < rhs);
	
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT fraction_save(
	const GblVariant *pVariant,
	GblStringBuffer  *pString
)
{
	const SpindleFraction *pfrac = pVariant->pVoid;
	GblStringBuffer_appendPrintf(
			pString, "%lld/%lld",
			(long long)pfrac->numerator,
			(long long)pfrac->denominator
		);

	return GBL_RESULT_SUCCESS;
}

static const GblIVariantVTable s_fractionVTable = {
	.supportedOps =
		  GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT
		| GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY
		| GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY
		| GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY
		| GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK
		| GBL_IVARIANT_OP_FLAG_RELOCATABLE,
	.pSetValueFmt = "ll",
	.pGetValueFmt = "ll",
	.pFnConstruct = fraction_construct,
	.pFnDestruct  = fraction_destruct,
	.pFnCompare   = fraction_compare,
	.pFnSave      = fraction_save,
};

typedef struct SpindleFractionClass {
	GblIVariantClass iVariantClass;
}
SpindleFractionClass;

static GBL_RESULT
fraction_class_init(GblClass *pClass, const void *pData)
{
	GBL_UNUSED(pData);
	GBL_IVARIANT_CLASS(pClass)->pVTable = &s_fractionVTable;
	return GBL_RESULT_SUCCESS;
}

GblType
SpindleFraction_type(void)
{
	if (s_fractionType == GBL_INVALID_TYPE) {
		static const GblInterfaceImpl ifaces[] = {{
				.interfaceType = GBL_IVARIANT_TYPE,
				.classOffset   = offsetof(SpindleFractionClass, iVariantClass)
			}};

		static const GblTypeInfo info = {
				.pFnClassInit    = fraction_class_init,
				.classSize       = sizeof(SpindleFractionClass),
				.interfacecount  = 1,
				.pInterfaceImpls = ifaces,
			};

		s_fractionType = GblType_register(
				"SpindleFraction",
				GBL_INVALID_TYPE,
				&info,
				GBL_TYPE_FLAGS_NONE
			);
		
	}

	return s_fractionType;
}
