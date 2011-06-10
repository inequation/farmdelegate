#ifndef FARMDELEGATE_H
#define FARMDELEGATE_H

#include <cassert>

#ifndef __GXX_EXPERIMENTAL_CXX0X__
	#warning C++0x extensions not detected, this might not compile!
#endif // __GXX_EXPERIMENTAL_CXX0X__

template <typename RETURN_TYPE, typename... TX>
class FarmDelegate {
	private:
		typedef RETURN_TYPE (*FUNC_PTR)(TX... ARGS);
		typedef RETURN_TYPE (FarmDelegate::*INDIR_FUNC_PTR)(TX... ARGS);
		#define METHOD_PTR(symbol)	RETURN_TYPE (TARGET_CLASS::*symbol)(TX... ARGS)
		typedef unsigned long long LLONG_PTR;

	public:
		FarmDelegate() : m_inFuncPtr(NULL), m_objPtr(NULL) {
			// make sure LLONG_PTR is large enough to accomodate a method pointer
			assert(sizeof(LLONG_PTR) == sizeof(INDIR_FUNC_PTR));
			m_FMPtr.method = 0;
		}

		FarmDelegate(FUNC_PTR funcPtr) {
			// make sure LLONG_PTR is large enough to accomodate a method pointer
			assert(sizeof(LLONG_PTR) == sizeof(INDIR_FUNC_PTR));
			this->Set(funcPtr);
		}

		void Set(FUNC_PTR funcPtr) {
			m_FMPtr.func = funcPtr;
			m_inFuncPtr = &FarmDelegate::CallFunc;
		}

		template <typename TARGET_CLASS>
		void Set(TARGET_CLASS *objPtr, METHOD_PTR(methodPtr)) {
			assert(objPtr != NULL);
			assert(methodPtr != NULL);
			m_objPtr = objPtr;
			m_FMPtr.method = *((LLONG_PTR *)(&methodPtr));
			m_inFuncPtr = &FarmDelegate::CallMethod<TARGET_CLASS>;
		}

		RETURN_TYPE operator()(TX... ARGS) {
			assert(m_inFuncPtr != NULL);
			return (this->*m_inFuncPtr)(ARGS...);
		}

	private:
		template <typename TARGET_CLASS>
		RETURN_TYPE CallMethod(TX... ARGS) {
			assert(m_objPtr != NULL);
			assert(m_FMPtr.method != 0);
			METHOD_PTR(meth) = *((METHOD_PTR(*))(&m_FMPtr.method));
			return (static_cast<TARGET_CLASS *>(m_objPtr)->*meth)(ARGS...);
		}

		RETURN_TYPE CallFunc(TX... ARGS) {
			assert(m_FMPtr.func != NULL);
			return m_FMPtr.func(ARGS...);
		}

		INDIR_FUNC_PTR	m_inFuncPtr;
		void			*m_objPtr;
		union {
			FUNC_PTR	func;
			LLONG_PTR	method;
		}				m_FMPtr;

		#undef METHOD_PTR
};

#endif // FARMDELEGATE_H
