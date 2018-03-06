//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2008 Greenplum, Inc.
//
//	@filename:
//		CStringConst.cpp
//
//	@doc:
//		Implementation of the wide character constant string class
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/common/clibwrapper.h"
#include "gpos/string/CStringConst.h"

using namespace gpos;


//---------------------------------------------------------------------------
//	@function:
//		CStringConst::CStringConst
//
//	@doc:
//		Initializes a constant string with a given character buffer. The string
//		does not own the memory
//
//---------------------------------------------------------------------------
CStringConst::CStringConst
	(
	const CHAR *wszBuf
	)
	:
	CStringBase
		(
		GPOS_SZ_LENGTH(wszBuf),
		false // fOwnsMemory
		),
	m_wszBuf(wszBuf)
{
	GPOS_ASSERT(NULL != wszBuf);
	GPOS_ASSERT(FValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CStringConst::CStringConst
//
//	@doc:
//		Initializes a constant string by making a copy of the given character buffer.
//		The string owns the memory.
//
//---------------------------------------------------------------------------
CStringConst::CStringConst
	(
	IMemoryPool *pmp,
	const CHAR *wszBuf
	)
	:
	CStringBase
		(
		GPOS_SZ_LENGTH(wszBuf),
		true // fOwnsMemory
		),
	m_wszBuf(NULL)
{
	GPOS_ASSERT(NULL != pmp);
	GPOS_ASSERT(NULL != wszBuf);

	if (0 == m_ulLength)
	{
		// string is empty
		m_wszBuf = &m_wcEmpty;
	}
	else
	{
		// make a copy of the string
		CHAR *wszTempBuf = GPOS_NEW_ARRAY(pmp, CHAR, m_ulLength + 1);
		clib::SzStrNCpy(wszTempBuf, wszBuf, m_ulLength + 1);
		m_wszBuf = wszTempBuf;
	}

	GPOS_ASSERT(FValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CStringConst::CStringConst
//
//	@doc:
//		Shallow copy constructor.
//
//---------------------------------------------------------------------------
CStringConst::CStringConst
	(
	const CStringConst& str
	)
	:
	CStringBase
		(
		str.UlLength(),
		false // fOwnsMemory
		),
	m_wszBuf(str.Sz())
{
	GPOS_ASSERT(NULL != m_wszBuf);
	GPOS_ASSERT(FValid());
}
//---------------------------------------------------------------------------
//	@function:
//		CStringConst::~CStringConst
//
//	@doc:
//		Destroys a constant string. This involves releasing the character buffer
//		provided the string owns it.
//
//---------------------------------------------------------------------------
CStringConst::~CStringConst()
{
	if (m_fOwnsMemory && m_wszBuf != &m_wcEmpty)
	{
		GPOS_DELETE_ARRAY(m_wszBuf);
	}
}

//---------------------------------------------------------------------------
//	@function:
//		CStringConst::Wsz
//
//	@doc:
//		Returns the wide character buffer
//
//---------------------------------------------------------------------------
const CHAR*
CStringConst::Sz() const
{
	return m_wszBuf;
}

// EOF

