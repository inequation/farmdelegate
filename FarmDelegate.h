#ifndef FARMDELEGATE_H
#define FARMDELEGATE_H

#include <cassert>

template <typename RETURN_TYPE, typename... TX>
class FarmDelegate {
	private:
		typedef RETURN_TYPE (*FUNC_PTR)(TX... ARGS);
		#define METHOD_PTR(symbol)	RETURN_TYPE (TARGET_CLASS::*symbol)(TX... ARGS)

	public:
		FarmDelegate() : m_funcPtr(NULL) {}
		FarmDelegate(FUNC_PTR funcPtr) : m_funcPtr(funcPtr) {}

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

#endif // FARMDELEGATE_H
