//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2008 Greenplum, Inc.
//
//	@filename:
//		CStringDynamic.cpp
//
//	@doc:
//		Implementation of the wide character string class
//		with dynamic buffer allocation.
//---------------------------------------------------------------------------

#include "gpos/common/clibwrapper.h"
#include "gpos/string/CStringStatic.h"
#include "gpos/string/CStringDynamic.h"
#include "gpos/common/CAutoRg.h"
using namespace gpos;


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::CStringDynamic
//
//	@doc:
//		Constructs an empty string
//
//------------------------------------------------------------------------ ---
CStringDynamic::CStringDynamic
	(
	IMemoryPool *pmp
	)
	:
	CString
		(
		0 // ulLength
		),
	m_pmp(pmp),
	m_ulCapacity(0)
{
	Reset();
}

//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::CStringDynamic
//
//	@doc:
//		Constructs a new string and initializes it with the given buffer
//
//---------------------------------------------------------------------------
CStringDynamic::CStringDynamic
	(
	IMemoryPool *pmp,
	const CHAR *wszBuf
	)
	:
	CString
		(
		GPOS_SZ_LENGTH(wszBuf)
		),
	m_pmp(pmp),
	m_ulCapacity(0)
{
	GPOS_ASSERT(NULL != wszBuf);

	Reset();
	AppendBuffer(wszBuf);
}


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::~CStringDynamic
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CStringDynamic::~CStringDynamic()
{
	Reset();
}


//---------------------------------------------------------------------------
//	@function:
//		CString::Reset
//
//	@doc:
//		Resets string
//
//---------------------------------------------------------------------------
void
CStringDynamic::Reset()
{
	if (NULL != m_wszBuf && &m_wcEmpty != m_wszBuf)
	{
		GPOS_DELETE_ARRAY(m_wszBuf);
	}

	m_wszBuf = const_cast<CHAR *>(&m_wcEmpty);
	m_ulLength = 0;
	m_ulCapacity = 0;
}


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::AppendBuffer
//
//	@doc:
//		Appends the contents of a buffer to the current string
//
//---------------------------------------------------------------------------
void
CStringDynamic::AppendBuffer
	(
	const CHAR *wsz
	)
{
	GPOS_ASSERT(NULL != wsz);
	ULONG ulLength = GPOS_SZ_LENGTH(wsz);
	if (0 == ulLength)
	{
		return;
	}

	// expand buffer if needed
	ULONG ulNewLength = m_ulLength + ulLength;
	if (ulNewLength + 1 > m_ulCapacity)
	{
		IncreaseCapacity(ulNewLength);
	}

	clib::SzStrNCpy(m_wszBuf + m_ulLength, wsz, ulLength + 1);
	m_ulLength = ulNewLength;

	GPOS_ASSERT(FValid());
}

//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::AppendCharArray
//
//	@doc:
//		Appends a a null terminated character array
//
//---------------------------------------------------------------------------
void
CStringDynamic::AppendCharArray
	(
	const CHAR *sz
	)
{
	GPOS_ASSERT(NULL != sz);

	// expand buffer if needed
	const ULONG ulLength = GPOS_SZ_LENGTH(sz);
	ULONG ulNewLength = m_ulLength + ulLength;
	if (ulNewLength + 1 > m_ulCapacity)
	{
		IncreaseCapacity(ulNewLength);
	}

	// append input string to current end of buffer
	(void) clib::PvMemCpy(m_wszBuf + m_ulLength, sz, ulLength + 1);

	m_wszBuf[ulNewLength] = CHAR_EOS;
	m_ulLength = ulNewLength;

	GPOS_ASSERT(FValid());
}


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::AppendFormat
//
//	@doc:
//		Appends a formatted string
//
//---------------------------------------------------------------------------
void
CStringDynamic::AppendFormat
	(
	const CHAR *wszFormat,
	...
	)
{
	GPOS_ASSERT(NULL != wszFormat);
	using clib::IVsnPrintf;

	VA_LIST	vaArgs;

	// determine length of format string after expansion
	INT iRes = -1;

	// attempt to fit the formatted string in a static array
	CHAR wszBufStatic[GPOS_STR_DYNAMIC_STATIC_BUFFER];

	// get arguments
	VA_START(vaArgs, wszFormat);

	// try expanding the formatted string in the buffer
	iRes = IVsnPrintf(wszBufStatic, GPOS_ARRAY_SIZE(wszBufStatic), wszFormat, vaArgs);

	// reset arguments
	VA_END(vaArgs);
	GPOS_ASSERT(-1 <= iRes);

	// estimated number of characters in expanded format string
	ULONG ulSize = std::max(GPOS_SZ_LENGTH(wszFormat), GPOS_ARRAY_SIZE(wszBufStatic));

	// if the static buffer is too small, find the formatted string
	// length by trying to store it in a buffer of increasing size
	while (-1 == iRes)
	{
		// try with a bigger buffer this time
		ulSize *= 2;
		CAutoRg<CHAR> a_wszBuf;
		a_wszBuf = GPOS_NEW_ARRAY(m_pmp, CHAR, ulSize + 1);

		// get arguments
		VA_START(vaArgs, wszFormat);

		// try expanding the formatted string in the buffer
		iRes = IVsnPrintf(a_wszBuf.Rgt(), ulSize, wszFormat, vaArgs);

		// reset arguments
		VA_END(vaArgs);

		GPOS_ASSERT(-1 <= iRes);
	}
	// verify required buffer was not bigger than allowed
	GPOS_ASSERT(iRes >= 0);

	// expand buffer if needed
	ULONG ulNewLength = m_ulLength + ULONG(iRes);
	if (ulNewLength + 1 > m_ulCapacity)
	{
		IncreaseCapacity(ulNewLength);
	}

	// get arguments
	VA_START(vaArgs, wszFormat);

	// print vaArgs to string
	IVsnPrintf(m_wszBuf + m_ulLength, iRes + 1, wszFormat, vaArgs);

	// reset arguments
	VA_END(vaArgs);

	m_ulLength = ulNewLength;
	GPOS_ASSERT(FValid());
}


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::AppendEscape
//
//	@doc:
//		Appends a string and replaces character with string
//
//---------------------------------------------------------------------------
void
CStringDynamic::AppendEscape
	(
	const CStringBase *pstr,
	CHAR wc,
	const CHAR *wszReplace
	)
{
	GPOS_ASSERT(NULL != pstr);

	if (pstr->FEmpty())
	{
		return;
	}

	// count how many times the character to be escaped appears in the string
	ULONG ulOccurences = pstr->UlOccurences(wc);
	if (0 == ulOccurences)
	{
		Append(pstr);
		return;
	}

	ULONG ulLength = pstr->UlLength();
	const CHAR * wsz = pstr->Sz();

	ULONG ulLengthReplace =  GPOS_SZ_LENGTH(wszReplace);
	ULONG ulNewLength = m_ulLength + ulLength + (ulLengthReplace - 1) * ulOccurences;
	if (ulNewLength + 1 > m_ulCapacity)
	{
		IncreaseCapacity(ulNewLength);
	}

	// append new contents while replacing character with escaping string
	for (ULONG i = 0, j = m_ulLength; i < ulLength; i++)
	{
		if (wc == wsz[i] && !pstr->FEscaped(i))
		{
			clib::SzStrNCpy(m_wszBuf + j, wszReplace, ulLengthReplace);
			j += ulLengthReplace;
		}
		else
		{
			m_wszBuf[j++] = wsz[i];
		}
	}

	// terminate string
	m_wszBuf[ulNewLength] = CHAR_EOS;
	m_ulLength = ulNewLength;

	GPOS_ASSERT(FValid());
}


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::IncreaseCapacity
//
//	@doc:
//		Increase string capacity
//
//---------------------------------------------------------------------------
void
CStringDynamic::IncreaseCapacity
	(
	ULONG ulRequested
	)
{
	GPOS_ASSERT(ulRequested + 1 > m_ulCapacity);

	ULONG ulCapacity = UlCapacity(ulRequested + 1);
	GPOS_ASSERT(ulCapacity > ulRequested + 1);
	GPOS_ASSERT(ulCapacity >= (m_ulCapacity << 1));

	CAutoRg<CHAR> a_wszNewBuf;
	a_wszNewBuf = GPOS_NEW_ARRAY(m_pmp, CHAR, ulCapacity);
	if (0 < m_ulLength)
	{
		// current string is not empty: copy it to the resulting string
		a_wszNewBuf = clib::SzStrNCpy(a_wszNewBuf.Rgt(), m_wszBuf, m_ulLength);
	}

	// release old buffer
	if (m_wszBuf != &m_wcEmpty)
	{
		GPOS_DELETE_ARRAY(m_wszBuf);
	}
	m_wszBuf = a_wszNewBuf.RgtReset();
	m_ulCapacity = ulCapacity;
}


//---------------------------------------------------------------------------
//	@function:
//		CStringDynamic::UlCapacity
//
//	@doc:
//		Find capacity that fits requested string size
//
//---------------------------------------------------------------------------
ULONG
CStringDynamic::UlCapacity
	(
	ULONG ulRequested
	)
{
	ULONG ulCapacity = GPOS_STR_DYNAMIC_CAPACITY_INIT;
	while (ulCapacity <= ulRequested + 1)
	{
		ulCapacity = ulCapacity << 1;
	}

	return ulCapacity;
}


// EOF

