#pragma once

#include "CVECpredefines.h"

#ifdef CVEC_sys_strdup
# 	undef CVEC_sys_strdup
#endif

#if defined(CVEC_OS_WIN32)
# 	include <string.h>
# 	define CVEC_sys_strdup 		_strdup
#else
# 	ifdef _POSIX_C_SOURCE 
# 		undef _POSIX_C_SOURCE 
# 	endif
# 	define _POSIX_C_SOURCE 200809L
# 	include <string.h>
# 	define CVEC_sys_strdup 		strdup
#endif // defined(CVEC_OS_WIN32)

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CVEC_NPOS 						-1
#define CVEC_SUCCESS 					0
#define CVEC_FAIL 						1
#define CVEC_FAIL_ALLOCATION 			2  		// Or reallocation
#define CVEC_FORCECAP_FAIL 				3
#define CVEC_ELEM_NOT_FOUND 			4
#define CVEC_GET_NULL  					5
#define CVEC_OUT_BOUNDS  				6
#define CVEC_NOT_ENOUGH_CAP 			7

#define __CVEC_CAP_ADDITION 			1

#define __CVEC_SET_NULL(vec, ptrSym) \
	vec ptrSym size = 0;             \
	vec ptrSym cap = 0;              \
	vec ptrSym elemLen = 0;          \
	vec ptrSym forceCap = false;     \
	vec ptrSym dynamicCap = false;    \
	vec ptrSym initialized = false   \

typedef struct
{
	bool initialized;
	bool __usedSplit;

	void *data;

	bool forceCap;
	bool dynamicCap;

	size_t size;
	size_t cap;
	size_t elemLen;
} CVEC;

/* MAIN FUNCTIONS */
// PUBLIC
CVEC cvec_init(int _cap, size_t _elemSize);
CVEC cvec_initCopy(const CVEC *_src);

int cvec_emptyAll(CVEC *_vec);
int cvec_destroy(CVEC *_vec);
int cvec_merge(CVEC *_toMerge, const CVEC *_input);
int cvec_remove(CVEC *_vec, const size_t _index);
int cvec_popBack(CVEC *_vec);
int cvec_popFront(CVEC *_vec);
int cvec_reverse(CVEC *_vec);
int cvec_shrink(CVEC *_vec);
int cvec_split(CVEC *_vec, char *_str, const char *_del);

bool cvec_at(const CVEC *_vec, const size_t _index);
bool cvec_atCap(const CVEC *_vec, const size_t _index);

void *cvec_get(const CVEC *_vec, const size_t _index);
void cvec_swap(CVEC *_a, CVEC *_b);

void cvec_clear(CVEC *_vec);

// PRIVATE
int __cvec_push(CVEC *_vec, void *_elem);
int __cvec_pushFront(CVEC *_vec, void *_elem);
int __cvec_pushIndex(CVEC *_vec, const size_t _index, void *_elem);
int __cvec_set(CVEC *_vec, const size_t _index, void *_set);
int __cvec_delIndex(CVEC *_vec, void *_elem, const size_t start);
int __cvec_del(CVEC *_vec, void *_elem);
int __cvec_find(const CVEC *_vec, void *_find);

int __cvec_destroySplit(CVEC *_vec);

bool __cvec_hasEnoughCap(const CVEC *_vec, const size_t _additions);

/**/

#define __CVEC_MACRO_MOD_CALL(func, vec, type, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = CVEC_NOT_ENOUGH_CAP; \
	if (__cvec_hasEnoughCap(vec, 1)) \
		_CVEC_TMP_RETVAL_ = func(vec, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

// Capacity modifiers
#define cvec_push(vec, type, val) \
	__CVEC_MACRO_MOD_CALL(__cvec_push, vec, type, val)

#define cvec_pushFront(vec, type, val) \
	__CVEC_MACRO_MOD_CALL(__cvec_pushFront, vec, type, val)

#define cvec_pushIndex(vec, type, pos, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_; \
	if (__cvec_hasEnoughCap(vec, 1)) \
		_CVEC_TMP_RETVAL_  = __cvec_pushIndex(vec, pos, &_CVEC_TMP_X_VAR_); \
	else \
		_CVEC_TMP_RETVAL_ = CVEC_NOT_ENOUGH_CAP; \
	_CVEC_TMP_RETVAL_; \
})

////

#define cvec_set(vec, type, val, index) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_set(vec, index, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

#define cvec_delIndex(vec, type, val, index) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_delIndex(vec, &_CVEC_TMP_X_VAR_, index); \
	_CVEC_TMP_RETVAL_; \
})

#define cvec_del(vec, type, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_del(vec, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

#define cvec_find(vec, type, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_find(vec, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

