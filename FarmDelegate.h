#ifndef FARMDELEGATE_H
#define FARMDELEGATE_H

#include <cassert>
#include <cstring>

// compiler-based detection of maximum method pointer size
#if defined(__GNUC__)			// G++
	#define MPTR_SIZE 8
#elif defined(_MSC_VER)			// MS Visual C++
	#define MPTR_SIZE 16
#elif defined(__INTEL_COMPILER)	// Intel C++ Compiler
	// these pointers are 20-byte on Itanium
	#if defined(_M_IA64) || defined(__IA64__) || defined(__ia64)
		#define MPTR_SIZE 20
	#else
		#define MPTR_SIZE 16
	#endif
#elif defined(__DMC__)			// Digital Mars Compiler
	#define MPTR_SIZE 4
#elif defined(__BORLANDC__)		// Borland C++
	#define MPTR_SIZE 12
#elif defined(__WATCOMC__)
	#define MPTR_SIZE 12
#else
	#warning Could not determine compiler, falling back to method pointer size failsafe of 20 bytes
	#define MPTR_SIZE 20
#endif

#ifdef __GXX_EXPERIMENTAL_CXX0X__
	// use C++0x variadic templates

	#define TEMPL_PARAMS
	#define FUNC_ARGS_DECL	TX... ARGS
	#define FUNC_ARGS		ARGS...
	template <typename RETURN_TYPE = void, typename... TX>
	#include "FarmDelegate_Impl.h"
	#undef FUNC_ARGS
	#undef FUNC_ARGS_DECL
	#undef TEMPL_PARAMS
#else // __GXX_EXPERIMENTAL_CXX0X__
	// use template specializations

	// 3 arguments
	#define TEMPL_PARAMS
	#define FUNC_ARGS_DECL	T1 arg1, T2 arg2, T3 arg3
	#define FUNC_ARGS		arg1, arg2, arg3
	template <typename RETURN_TYPE, typename T1 = void, typename T2 = void, typename T3 = void>
	#include "FarmDelegate_Impl.h"
	#undef FUNC_ARGS
	#undef FUNC_ARGS_DECL
	#undef TEMPL_PARAMS

	// 2 arguments
	#define TEMPL_PARAMS	<RETURN_TYPE, T1, T2, void>
	#define FUNC_ARGS_DECL	T1 arg1, T2 arg2
	#define FUNC_ARGS		arg1, arg2
	template <typename RETURN_TYPE, typename T1, typename T2>
	#include "FarmDelegate_Impl.h"
	#undef FUNC_ARGS
	#undef FUNC_ARGS_DECL
	#undef TEMPL_PARAMS

	// 1 argument
	#define TEMPL_PARAMS	<RETURN_TYPE, T1, void, void>
	#define FUNC_ARGS_DECL	T1 arg1
	#define FUNC_ARGS		arg1
	template <typename RETURN_TYPE, typename T1>
	#include "FarmDelegate_Impl.h"
	#undef FUNC_ARGS
	#undef FUNC_ARGS_DECL
	#undef TEMPL_PARAMS

	// no arguments
	#define TEMPL_PARAMS	<RETURN_TYPE, void, void, void>
	#define FUNC_ARGS_DECL
	#define FUNC_ARGS
	template <typename RETURN_TYPE>
	#include "FarmDelegate_Impl.h"
	#undef FUNC_ARGS
	#undef FUNC_ARGS_DECL
	#undef TEMPL_PARAMS
#endif // __GXX_EXPERIMENTAL_CXX0X__

#undef MPTR_SIZE

#endif // FARMDELEGATE_H
