//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2009 Greenplum, Inc.
//
//	@filename:
//		CName.cpp
//
//	@doc:
//		Metadata name of objects
//		Encapsulates encoding etc. so optimizer logic does not have to
//		deal with it. 
//		Only assumption, name string is NULL terminated;
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/string/CStringStatic.h"
#include "gpopt/metadata/CName.h"

using namespace gpopt;

//---------------------------------------------------------------------------
//	@function:
//		CName::CName
//
//	@doc:
//		ctor
//		deep copy of the provided string
//
//---------------------------------------------------------------------------
CName::CName
	(
	IMemoryPool *pmp,
	const CStringStatic *pstr
	)
	:m_pstrName(NULL),
	 m_fDeepCopy(true)
{
	m_pstrName = GPOS_NEW(pmp) CStringStatic(pstr->Sz(), 1024);
}

//---------------------------------------------------------------------------
//	@function:
//		CName::CName
//
//	@doc:
//		ctor
//		the string object can become property of the CName object, or not, as
//		specified by the fOwnsMemory argument
//
//---------------------------------------------------------------------------
CName::CName
	(
	const CStringStatic *pstr,
	BOOL fOwnsMemory
	)
	:
	m_pstrName(pstr),
	m_fDeepCopy(fOwnsMemory)
{
	GPOS_ASSERT(NULL != m_pstrName);
	//GPOS_ASSERT(m_pstrName->FValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CName::CName
//
//	@doc:
//		ctor
//		combine 2 names to one compound name; used to incrementally add
//		names for canonical multi-part names
//
//---------------------------------------------------------------------------
CName::CName
	(
	IMemoryPool *pmp,
	const CName &nameFirst,
	const CName &nameSecond
	)
	:
	m_pstrName(NULL),
	m_fDeepCopy(false)
{
	CStringStatic *pstrTmp = GPOS_NEW(pmp) CStringStatic((nameFirst.Pstr())->Sz(), 1024);
	pstrTmp->Append(nameSecond.Pstr());
	
	m_pstrName = GPOS_NEW(pmp) CStringStatic(pstrTmp->Sz(), 1024);
	m_fDeepCopy = true;
	
	// release tmp string
	GPOS_DELETE(pstrTmp);
}


//---------------------------------------------------------------------------
//	@function:
//		CName::CName
//
//	@doc:
//		ctor
//		shallow copy constructor
//
//---------------------------------------------------------------------------
CName::CName
	(
	const CName &name
	)
	:
	m_pstrName(name.Pstr()),
	m_fDeepCopy(false)
{
	GPOS_ASSERT(NULL != m_pstrName->Sz());
	//GPOS_ASSERT(m_pstrName->FValid());
}


//---------------------------------------------------------------------------
//	@function:
//		CName::CName
//
//	@doc:
//		ctor
//		deep copy constructor
//
//---------------------------------------------------------------------------
CName::CName
	(
	IMemoryPool *pmp,
	const CName &name
	)
	:
	m_pstrName(NULL),
	m_fDeepCopy(false)
{
	DeepCopy(pmp, name.Pstr());
}


//---------------------------------------------------------------------------
//	@function:
//		CName::~CName
//
//	@doc:
//		dtor
//
//---------------------------------------------------------------------------
CName::~CName()
{
	//GPOS_ASSERT(m_pstrName->FValid());

	if (m_fDeepCopy)
	{
		GPOS_DELETE(m_pstrName);
	}
}



//---------------------------------------------------------------------------
//	@function:
//		CName::DeepCopy
//
//	@doc:
//		Deep copying of string
//
//---------------------------------------------------------------------------
void
CName::DeepCopy
	(
	IMemoryPool *pmp,
	const CStringStatic *pstr
	)
{
	m_pstrName = GPOS_NEW(pmp) CStringStatic(pstr->Sz(), 1024);
	m_fDeepCopy = true;
}


#ifdef GPOS_DEBUG
//---------------------------------------------------------------------------
//	@function:
//		CName::FEquals
//
//	@doc:
//		comparison of names
//
//---------------------------------------------------------------------------
BOOL
CName::FEquals
	(
	const CName &name
	)
	const 
{
	return m_pstrName->FEquals((name.Pstr()->Sz()));
}

#endif // GPOS_DEBUG


//---------------------------------------------------------------------------
//	@function:
//		CName::OsPrint
//
//	@doc:
//		debug print
//
//---------------------------------------------------------------------------
IOstream &
CName::OsPrint
	(
	IOstream &os
	)
	const
{
	os << GPOPT_NAME_QUOTE_BEGIN << m_pstrName->Sz() << GPOPT_NAME_QUOTE_END;
	return os;
}

// EOF

