//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2010 Greenplum, Inc.
//
//	@filename:
//		CString.h
//
//	@doc:
//		Wide character string interface.
//---------------------------------------------------------------------------
#ifndef GPOS_CString_H
#define GPOS_CString_H

#include "gpos/string/CStringBase.h"

#define GPOS_MAX_FMT_STR_LENGTH (10*1024*1024) // 10MB

namespace gpos
{
	//---------------------------------------------------------------------------
	//	@class:
	//		CString
	//
	//	@doc:
	//		Wide character String interface.
	//		Internally, the class uses a null-terminated CHAR buffer to store the string characters.
	//		The class API provides functions for accessing the wide-character buffer and its length,
	//		as well as functions that modify the current string by appending another string to it,
	//		or that construct a string according to a given format.
	//		For constant strings consider using the CStringConst class.
	//
	//---------------------------------------------------------------------------
	class CString : public CStringBase
	{
		protected:

			// null-terminated wide character buffer
			CHAR *m_wszBuf;

			// appends the contents of a buffer to the current string
			virtual void AppendBuffer(const CHAR *wszBuf) = 0;
			
		public:

			// ctor
			CString(ULONG ulLength);

			// dtor
			virtual ~CString()
			{}
					
			// returns the wide character buffer storing the string
			const CHAR* Sz() const;
			
			// appends a string
			void Append(const CStringBase *pstr);

			// appends a formatted string
			virtual
			void AppendFormat(const CHAR *wszFormat, ...) = 0;

			// appends a string and replaces character with string
			virtual
			void AppendEscape(const CStringBase *pstr, CHAR wc, const CHAR *wszReplace) = 0;

			// appends a null terminated character array
			virtual
			void AppendCharArray(const CHAR *sz) = 0;

			// resets string
			virtual void Reset() = 0;
	};
}

#endif // !GPOS_CString_H

// EOF

