#ifndef FARMDELEGATESIMPLE_H
#define FARMDELEGATESIMPLE_H

#include <cassert>

#ifndef __GXX_EXPERIMENTAL_CXX0X__
	#warning C++0x extensions not detected, this might not compile!
#endif // __GXX_EXPERIMENTAL_CXX0X__

template <typename RETURN_TYPE, typename... TX>
class FarmDelegateSimple {
	private:
		typedef RETURN_TYPE (*FUNC_PTR)(TX... ARGS);
		#define METHOD_PTR(symbol)	RETURN_TYPE (TARGET_CLASS::*symbol)(TX... ARGS)

	public:
		FarmDelegateSimple() : m_funcPtr(NULL) {}
		FarmDelegateSimple(FUNC_PTR funcPtr) : m_funcPtr(funcPtr) {}

		void Set(FUNC_PTR funcPtr) {m_funcPtr = funcPtr;}
		template <typename TARGET_CLASS, TARGET_CLASS *objPtr, METHOD_PTR(methodPtr)>
		void Set() {m_funcPtr = CallMethod<TARGET_CLASS, objPtr, methodPtr>;}

		RETURN_TYPE operator()(TX... ARGS) {
			assert(m_funcPtr != NULL);
			return m_funcPtr(ARGS...);
		}

	private:
		template <typename TARGET_CLASS, TARGET_CLASS *objPtr, METHOD_PTR(methodPtr)>
		static RETURN_TYPE CallMethod(TX... ARGS) {
			assert(methodPtr != NULL);
			//assert(objPtr != NULL);
			return (objPtr->*methodPtr)(ARGS...);
		}

		FUNC_PTR		m_funcPtr;

		#undef METHOD_PTR
};

#endif // FARMDELEGATEMUTABLE_H

