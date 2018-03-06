//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2010 Greenplum, Inc.
//
//	@filename:
//		CStringConst.h
//
//	@doc:
//		Constant string class
//---------------------------------------------------------------------------
#ifndef GPOS_CStringConst_H
#define GPOS_CStringConst_H

#include "gpos/string/CStringBase.h"

namespace gpos
{
	//---------------------------------------------------------------------------
	//	@class:
	//		CStringConst
	//
	//	@doc:
	//		Constant string class.
	//		The class represents constant strings, which cannot be modified after creation.
	//		The class can either own its own memory, or be supplied with an external
	//		memory buffer holding the string characters.
	//		For a general string class that can be modified, see CWString.
	//
	//---------------------------------------------------------------------------
	class CStringConst : public CStringBase
	{
		private:
			// null terminated wide character buffer
			const CHAR *m_wszBuf;
			
		public:
			// ctors
			CStringConst(const CHAR *wszBuf);
			CStringConst(IMemoryPool *pmp, const CHAR *wszBuf);
			
			// shallow copy ctor
			CStringConst(const CStringConst&);
			
			//dtor
			~CStringConst();
			
			// returns the wide character buffer storing the string
			const CHAR* Sz() const;
	};
}

#endif // #ifndef GPOS_CStringConst_H

// EOF

