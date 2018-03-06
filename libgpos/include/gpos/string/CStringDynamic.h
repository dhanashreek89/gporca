//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2010 Greenplum, Inc.
//
//	@filename:
//		CStringDynamic.h
//
//	@doc:
//		Wide character string class with dynamic buffer allocation.
//---------------------------------------------------------------------------
#ifndef GPOS_CStringDynamic_H
#define GPOS_CStringDynamic_H

#include "gpos/memory/CMemoryPoolManager.h"
#include "gpos/string/CString.h"

#define GPOS_STR_DYNAMIC_CAPACITY_INIT	(1 << 7)
#define GPOS_STR_DYNAMIC_STATIC_BUFFER	(1 << 10)

namespace gpos
{
	//---------------------------------------------------------------------------
	//	@class:
	//		CStringDynamic
	//
	//	@doc:
	//		Implementation of the string interface with dynamic buffer allocation.
	//		This CStringDynamic class dynamically allocates memory when constructing a new
	//		string, or when modifying a string. The memory is released at destruction time
	//		or when the string is reset.
	//
	//---------------------------------------------------------------------------
	class CStringDynamic : public CString
	{
		private:

			// string memory pool used for allocating new memory for the string
			IMemoryPool *m_pmp;

			// string capacity
			ULONG m_ulCapacity;

			// increase string capacity
			void IncreaseCapacity(ULONG ulRequested);

			// find capacity that fits requested string size
			static
			ULONG UlCapacity(ULONG ulRequested);

			// private copy ctor
			CStringDynamic(const CStringDynamic&);

		protected:

			// appends the contents of a buffer to the current string
			void AppendBuffer(const CHAR *wszBuf);

		public:

			// ctor
			CStringDynamic(IMemoryPool *pmp);

			// ctor - copies passed string
			CStringDynamic(IMemoryPool *pmp, const CHAR *wszBuf);

			// appends a string and replaces character with string
			void AppendEscape(const CStringBase *pstr, CHAR wc, const CHAR *wszReplace);

			// appends a formatted string
			void AppendFormat(const CHAR *wszFormat, ...);

			// appends a null terminated character array
			virtual
			void AppendCharArray(const CHAR *sz);

			// dtor
			virtual ~CStringDynamic();

			// resets string
			void Reset();
	};
}

#endif // !GPOS_CStringDynamic_H

// EOF

