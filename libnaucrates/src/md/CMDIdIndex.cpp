//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2013 EMC Corp.
//
//	@filename:
//		CMDIdIndex.cpp
//
//	@doc:
//		Implementation of mdids for cast functions
//---------------------------------------------------------------------------


#include "naucrates/md/CMDIdIndex.h"
#include "naucrates/dxl/xml/CXMLSerializer.h"

using namespace gpos;
using namespace gpmd;

//---------------------------------------------------------------------------
//	@function:
//		CMDIdIndex::CMDIdIndex
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CMDIdIndex::CMDIdIndex
	(
	 BOOL fAggregateIndex
	)
	:
	m_fAggregateIndex(fAggregateIndex),
	m_str(m_wszBuffer, GPOS_ARRAY_SIZE(m_wszBuffer))
{
	// serialize mdid into static string 
	Serialize();
}

//---------------------------------------------------------------------------
//	@function:
//		CMDIdIndex::~CMDIdIndex
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CMDIdIndex::~CMDIdIndex()
{
}

//---------------------------------------------------------------------------
//	@function:
//		CMDIdIndex::Serialize
//
//	@doc:
//		Serialize mdid into static string
//
//---------------------------------------------------------------------------
void
CMDIdIndex::Serialize()
{
	// serialize mdid as SystemType.mdidSrc.mdidDest
	m_str.AppendFormat
			(
			GPOS_WSZ_LIT("%d.%d"), 
			Emdidt(),
			m_fAggregateIndex
			);
}

//---------------------------------------------------------------------------
//	@function:
//		CMDIdIndex::Wsz
//
//	@doc:
//		Returns the string representation of the mdid
//
//---------------------------------------------------------------------------
const WCHAR *
CMDIdIndex::Wsz() const
{
	return m_str.Wsz();
}

//---------------------------------------------------------------------------
//	@function:
//		CMDIdIndex::Serialize
//
//	@doc:
//		Serializes the mdid as the value of the given attribute
//
//---------------------------------------------------------------------------
void
CMDIdIndex::Serialize
	(
	CXMLSerializer * pxmlser,
	const CWStringConst *pstrAttribute
	)
	const
{
	pxmlser->AddAttribute(pstrAttribute, &m_str);
}

//---------------------------------------------------------------------------
//	@function:
//		CMDIdIndex::OsPrint
//
//	@doc:
//		Debug print of the id in the provided stream
//
//---------------------------------------------------------------------------
IOstream &
CMDIdIndex::OsPrint
	(
	IOstream &os
	) 
	const
{
	os << "(" << m_str.Wsz() << ")";
	return os;
}

// EOF
