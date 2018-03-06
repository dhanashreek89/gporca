//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2008 Greenplum, Inc.
//
//	@filename:
//		CString.cpp
//
//	@doc:
//		Implementation of the wide character string class.
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/string/CString.h"

using namespace gpos;


//---------------------------------------------------------------------------
//	@function:
//		CString::CString
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CString::CString
	(
	ULONG ulLength
	)
	:
	CStringBase
		(
		ulLength,
		true // fOwnsMemory
		),
	m_wszBuf(NULL)
{}


//---------------------------------------------------------------------------
//	@function:
//		CString::Wsz
//
//	@doc:
//		Returns the wide character buffer storing the string
//
//---------------------------------------------------------------------------
const CHAR*
CString::Sz() const
{
	return m_wszBuf;
}


//---------------------------------------------------------------------------
//	@function:
//		CString::Append
//
//	@doc:
//		Appends a string to the current string
//
//---------------------------------------------------------------------------
void
CString::Append
	(
	const CStringBase *pstr
	)
{
	GPOS_ASSERT(NULL != pstr);
	if (0 < pstr->UlLength())
	{
		AppendBuffer(pstr->Sz());
	}
	GPOS_ASSERT(FValid());
}

// EOF

