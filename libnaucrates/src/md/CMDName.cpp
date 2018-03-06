//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 Greenplum, Inc.
//
//	@filename:
//		CMDName.cpp
//
//	@doc:
//		Metadata name of objects
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/string/CStringDynamic.h"
#include "naucrates/md/CMDName.h"

using namespace gpmd;


//---------------------------------------------------------------------------
//	@function:
//		CMDName::CMDName
//
//	@doc:
//		Constructor
//		Creates a deep copy of the provided string
//
//---------------------------------------------------------------------------
CMDName::CMDName
	(
	IMemoryPool *pmp,
	const CStringBase *pstr
	)
	:
	m_psc(NULL),
	m_fDeepCopy(true)
{
	m_psc = GPOS_NEW(pmp) CStringConst(pmp, pstr->Sz());
}

//---------------------------------------------------------------------------
//	@function:
//		CMDName::CMDName
//
//	@doc:
//		ctor
//		Depending on the value of the the fOwnsMemory argument, the string object
//		can become property of the CMDName object
//
//---------------------------------------------------------------------------
CMDName::CMDName
	(
	const CStringConst *pstr,
	BOOL fOwnsMemory
	)
{
    if (fOwnsMemory) // this is stupid, see if we can get rid of fOwnsMemory
        GPOS_ASSERT(NULL != m_psc);
	m_psc = pstr;
//	GPOS_ASSERT(m_psc->FValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CMDName::CMDName
//
//	@doc:
//		Shallow copy constructor
//
//---------------------------------------------------------------------------
CMDName::CMDName
	(
	const CMDName &name
	)

{
	GPOS_ASSERT(NULL != m_psc->Sz());
	m_psc = name.Pstr();
//	GPOS_ASSERT(m_psc->FValid());
}


//---------------------------------------------------------------------------
//	@function:
//		CMDName::~CMDName
//
//	@doc:
//		dtor
//
//---------------------------------------------------------------------------
CMDName::~CMDName()
{
	//GPOS_ASSERT(m_psc->FValid());

	if (m_fDeepCopy)
	{
		GPOS_DELETE(m_psc);
	}
}

// EOF

