//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2010 Greenplum, Inc.
//
//	@filename:
//		CStringBase.cpp
//
//	@doc:
//		Implementation of the base abstract wide character string class
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpos/common/clibwrapper.h"
#include "gpos/string/CStringBase.h"
#include "gpos/string/CStringConst.h"

using namespace gpos;

const CHAR CStringBase::m_wcEmpty = '\0';

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::PStrCopy
//
//	@doc:
//		Creates a deep copy of the string
//
//---------------------------------------------------------------------------
CStringConst *CStringBase::PStrCopy
	(
	IMemoryPool *pmp
	) const
{
	return GPOS_NEW(pmp) CStringConst(pmp, Sz());
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::FValid
//
//	@doc:
//		Checks if the string is properly NULL-terminated
//
//---------------------------------------------------------------------------
bool
CStringBase::FValid() const
{
	return (UlLength() == GPOS_SZ_LENGTH(Sz()));
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::operator == 
//
//	@doc:
//		Equality operator on strings
//
//---------------------------------------------------------------------------
BOOL
CStringBase::operator ==
	(
	const CStringBase &str
	)
	const
{
	return FEquals(&str);
}


//---------------------------------------------------------------------------
//	@function:
//		CStringBase::UlLength()
//
//	@doc:
//		Returns the length of the string in number of wide characters,
//		not counting the terminating '\0'
//
//---------------------------------------------------------------------------
ULONG
CStringBase::UlLength() const
{
	return m_ulLength;
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::FEquals
//
//	@doc:
//		Checks whether the string is byte-wise equal to another string
//
//---------------------------------------------------------------------------
BOOL
CStringBase::FEquals
	(
	const CStringBase *pStr
	)
	const
{
	GPOS_ASSERT(NULL != pStr);
	return FEquals(pStr->Sz());
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::FEquals
//
//	@doc:
//		Checks whether the string is byte-wise equal to a string literal
//
//---------------------------------------------------------------------------
BOOL
CStringBase::FEquals
	(
	const CHAR *SzBuf
	)
	const
{
	GPOS_ASSERT(NULL != SzBuf);
	ULONG ulLength = GPOS_SZ_LENGTH(SzBuf);
	if (UlLength() == ulLength &&
		0 == clib::IStrNCmp(Sz(), SzBuf, ulLength))
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::FEmpty
//
//	@doc:
//		Checks whether the string is empty
//
//---------------------------------------------------------------------------
BOOL
CStringBase::FEmpty() const
{
	return (0 == UlLength());
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::IFind
//
//	@doc:
//		Returns the index of the first occurrence of a character, -1 if not found
//
//---------------------------------------------------------------------------
INT
CStringBase::IFind
	(
	CHAR wc
	)
	const
{
	const CHAR *sz = Sz();
	const ULONG ulLength = UlLength();

	for (ULONG i = 0; i < ulLength; i++)
	{
		if (wc == sz[i])
		{
			return i;
		}
	}

	return -1;
}


//---------------------------------------------------------------------------
//	@function:
//		CStringBase::FEscaped
//
//	@doc:
//		Checks if a character is escaped
//
//---------------------------------------------------------------------------
BOOL
CStringBase::FEscaped
	(
	ULONG ulOfst
	)
	const
{
	GPOS_ASSERT(!FEmpty());
	GPOS_ASSERT(UlLength() > ulOfst);

	const CHAR *SzBuf = Sz();

	for (ULONG i = ulOfst; i > 0; i--)
	{
		// check for escape character
		if ('\\' != SzBuf[i - 1])
		{
			if (0 == ((ulOfst - i) & ULONG(1)))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	// reached beginning of string
	if (0 == (ulOfst & ULONG(1)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//---------------------------------------------------------------------------
//	@function:
//		CStringBase::UlOccurences
//
//	@doc:
//		Count how many times the character appears in string
//
//---------------------------------------------------------------------------
ULONG
CStringBase::UlOccurences
	(
	const CHAR wc
	) const
{
	ULONG ulOccurences = 0;
	ULONG ulLength = UlLength();
	const CHAR *sz = Sz();

	for (ULONG i = 0; i < ulLength; i++)
	{
		if (wc == sz[i] && !FEscaped(i))
		{
			ulOccurences++;
		}
	}
	return ulOccurences;
}
// EOF

