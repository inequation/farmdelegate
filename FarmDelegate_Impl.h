// reverse header guard - ONLY include this file inside FarmDelegate.h
#define FARMDELEGATE_H
#ifdef FARMDELEGATE_H

class FarmDelegate TEMPL_PARAMS {
	private:
		typedef RETURN_TYPE (*FUNC_PTR)(FUNC_ARGS_DECL);
		typedef RETURN_TYPE (FarmDelegate::*INDIR_FUNC_PTR)(FUNC_ARGS_DECL);
		#define METHOD_PTR(symbol)	RETURN_TYPE (TARGET_CLASS::*symbol)(FUNC_ARGS_DECL)
		typedef unsigned char MPTR[MPTR_SIZE];

	public:
		FarmDelegate() : m_inFuncPtr(NULL), m_objPtr(NULL) {
			// make sure MPTR is large enough to accomodate a method pointer
			assert(sizeof(MPTR) >= sizeof(INDIR_FUNC_PTR));
			memset(m_FMPtr.method, 0, sizeof(MPTR));
		}

		FarmDelegate(FUNC_PTR funcPtr) {
			// make sure MPTR is large enough to accomodate a method pointer
			assert(sizeof(MPTR) >= sizeof(INDIR_FUNC_PTR));
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
			// make sure MPTR is large enough to accomodate a method pointer
			assert(sizeof(MPTR) >= sizeof(INDIR_FUNC_PTR));
			m_objPtr = objPtr;
			memcpy(m_FMPtr.method, &methodPtr, sizeof(methodPtr));
			m_inFuncPtr = &FarmDelegate::CallMethod<TARGET_CLASS>;
		}

		FarmDelegate& operator=(FUNC_PTR funcPtr) {this->Set(funcPtr);}

		RETURN_TYPE operator()(FUNC_ARGS_DECL) {
			assert(m_inFuncPtr != NULL);
			return (this->*m_inFuncPtr)(FUNC_ARGS);
		}

	private:
		template <typename TARGET_CLASS>
		RETURN_TYPE CallMethod(FUNC_ARGS_DECL) {
			assert(m_objPtr != NULL);
			assert(m_FMPtr.method != 0);
			return (static_cast<TARGET_CLASS *>(m_objPtr)->*(*((METHOD_PTR(*))(m_FMPtr.method))))(FUNC_ARGS);
		}

		RETURN_TYPE CallFunc(FUNC_ARGS_DECL) {
			assert(m_FMPtr.func != NULL);
			return m_FMPtr.func(FUNC_ARGS);
		}

		INDIR_FUNC_PTR	m_inFuncPtr;
		void			*m_objPtr;
		union {
			FUNC_PTR	func;
			MPTR	method;
		}				m_FMPtr;

		#undef METHOD_PTR
};

#endif // FARMDELEGATE_H
