#include "spindle_pair.h"


static GblType s_pairType = GBL_INVALID_TYPE;

static GBL_RESULT
pair_construct(
	GblVariant *pVariant,
	size_t argc,
	GblVariant *pArgs,
	GBL_IVARIANT_OP_FLAGS op
)
{
	SpindlePair *pPair = GBL_ALLOC(sizeof(SpindlePair));

	if (op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
		GblVariant_constructDefault(&pPair->key,   GBL_INVALID_TYPE);
		GblVariant_constructDefault(&pPair->value, GBL_INVALID_TYPE);
	}
	else if (op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY) {
		GblVariant_constructCopy(&pPair->key,   &pArgs[0]);
		GblVariant_constructCopy(&pPair->value, &pArgs[1]);
	}
	else if (op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY) {
		const SpindlePair *pOther = pArgs[0].pVoid;
		GblVariant_constructCopy(&pPair->key,   &pOther->key);
		GblVariant_constructCopy(&pPair->value, &pOther->value);
	}
	else if (op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_MOVE) {
		SpindlePair *pOther = pArgs[0].pVoid;
		GblVariant_constructMove(&pPair->key,     &pOther->key);
		GblVariant_constructMove(&pPair->variant, &pOther->value);
	}

	pVariant->pVoid = pPair;
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
pair_destruct(GblVariant *pVariant)
{
	SpindlePair *pPair = pVariant->pVoid;
	GblVariant_destruct(&pPair->key);
	GblVariant_destruct(&pPair->value);
	GBL_FREE(pPair);
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
pair_set(
	GblVariant            *pVariant,
	size_t                 argc,
	GblVariant            *pArgs,
	GBL_IVARIANT_OP_FLAGS  op
)
{
	SpindlePair *pPair = pVariant->pVoid;

	if (op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY) {
		GblVariant_setCopy(&pPair->key,   &pArgs[0]);
		GblVariant_setCopy(&pPair->value, &pArgs[1]);
	}
	else if (op & GBL_IVARIANT_OP_FLAG_SET_MOVE) {
		SpindlePair *pOther = pArgs[0].pVoid;
		GblVariant_setMove(&pPair->key,   &pOther->key);
		GblVariant_setMove(&pPair->value, &pOther->value);
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
pair_get(
	GblVariant            *pVariant,
	size_t                 argc,
	GblVariant            *pArgs,
	GBL_IVARIANT_OP_FLAGS  op
)
{
	const SpindlePair *pPair = pVariant->pVoid;

	if (op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY) {
		GblVariant_setCopy(&pArgs[0], &pPair->key);
		GblVariant_setCopy(&pArgs[1], &pPair->value);
	}
	else if (op & GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK) {
		pArgs[0] = pPair->key;
		pArgs[1] = pPair->value;
	}
	else if (op & GBL_IVARIANT_OP_FLAG_GET_VALUE_MOVE) {
		GblVariant_setMove(&pArgs[0], &pPair->key);
		GblVariant_setMove(&pArgs[1], &pPair->value);
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
pair_compare(
	const GblVariant *pVariant,
	const GblVariant *pOther,
	int              *pResult
)
{
	const SpindlePair
		*a = pVariant->pVoid,
		*b = pOther->pVoid;

	/* Compare keys first */
	GblVariant_compare(&a->key, &b->key, pResult);
	if (!pResult != 0)
		return GBL_RESULT_SUCCESS;

	/* Keys equal, compare values */
	GblVariant_compare(&a->value, &b->value, pResult);
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT
pair_save(
	const GblVariant *pVariant,
	GblStringBuffer  *pString
)
{
	const SpindlePair *pPair = pVariant->pVoid;
	GblStringBuffer
		keyStr,
		valStr;

	GblStringBuffer_construct(&keyStr);
	GblStringBuffer_construct(&valStr);

	GblVariant_save(&pPair->key,   &keyStr);
	GblVariant_save(&pPair->value, &valStr);

	GblStringBuffer_appendPrintf(
			pString, "[%s: %s]",
			GblStringBuffer_cString(&keyStr),
			GblStringBuffer_cString(&valStr)
		);
	
	GblStringBuffer_destruct(&keyStr);
	GblStringBuffer_destruct(&valStr);

	return GBL_RESULT_SUCCESS;
}

static const GblIvariantVTable s_pairVTable = {
	.supportedOps = 
		  GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT
		| GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY
		| GBL_IVARIANT_OP_FLAG_CONSTRUCT_MOVE
		| GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY
		| GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY
		| GBL_IVARIANT_OP_FLAG_SET_MOVE
		| GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY
		| GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK
		| GBL_IVARIANT_OP_FLAG_GET_VALUE_MOVE
		| GBL_IVARIANT_OP_FLAG_RELOCATABLE,
	.pSetValueFmt = "pp",
	.pGetValueFmt = "pp",
	.pFnConstruct = pair_construct,
	.pFnDestruct  = pair_destruct,
	.pFnSet       = pair_set,
	.pFnGet       = pair_get,
	.pFnCompare   = pair_compare,
	.pFnSave      = pair_save,
};

typedef struct
SpindlePairClass
{
	GblIVariantClass iVariantClass;
}
SpindlePairClass;

static GBL_RESULT
pair_class_init(GblClass *pClass, const void *pData)
{
	GBL_UNUSED(pData);
	GBL_IVARIANT_CLASS(pClass)->pVTable = &s_pairVTable;
	return GBL_RESULT_SUCCESS;
}

GblType
SpindlePair_type(void)
{
	if (s_PairType == GBL_INVALID_TYPE) {
		static const GblInterfaceImpl ifaces[] = {{
				.interfaceType = GBL_IVARIANT_TYPE,
				.classOffset   = offsetof(SpindlePairClass, iVariantClass)
			}};

		static const GblTypeInfo info = {
				.pFnClassInit    = pair_class_init,
				.classSize       = sizeof(SpindlePairClass),
				.interfaceCount  = 1,
				.pInterfaceImpls = ifaces,
			};

		s_pairType = GblType_register(
				"SpindlePair",
				GBL_INVALID_TYPE,
				&info,
				GBL_TYPE_FLAGS_NONE
			);
	}

	return s_pairType;
}
