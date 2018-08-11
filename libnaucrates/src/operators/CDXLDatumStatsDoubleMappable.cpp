//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2014 Pivotal Inc.
//
//	@filename:
//		CDXLDatumStatsDoubleMappable.cpp
//
//	@doc:
//		Implementation of DXL datum of types having double mapping
//
//	@owner:
//
//
//	@test:
//
//---------------------------------------------------------------------------

#include "naucrates/dxl/operators/CDXLDatumStatsDoubleMappable.h"
#include "naucrates/dxl/CDXLUtils.h"
#include "naucrates/dxl/xml/CXMLSerializer.h"

using namespace gpos;
using namespace gpdxl;

//---------------------------------------------------------------------------
//	@function:
//		CDXLDatumStatsDoubleMappable::CDXLDatumStatsDoubleMappable
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CDXLDatumStatsDoubleMappable::CDXLDatumStatsDoubleMappable(IMemoryPool *mp,
														   IMDId *mdid_type,
														   INT type_modifier,
														   BOOL is_passed_by_value,
														   BOOL is_null,
														   BYTE *data,
														   ULONG length,
														   CDouble val)
	: CDXLDatumGeneric(mp, mdid_type, type_modifier, is_passed_by_value, is_null, data, length),
	  m_val(val)
{
}
//---------------------------------------------------------------------------
//	@function:
//		CDXLDatumStatsDoubleMappable::Serialize
//
//	@doc:
//		Serialize datum in DXL format
//
//---------------------------------------------------------------------------
void
CDXLDatumStatsDoubleMappable::Serialize(CXMLSerializer *xml_serializer)
{
	m_mdid_type->Serialize(xml_serializer, CDXLTokens::GetDXLTokenStr(EdxltokenTypeId));
	if (default_type_modifier != TypeModifier())
	{
		xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenTypeMod), TypeModifier());
	}
	xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenIsNull), m_is_null);
	xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenIsByValue),
								 m_is_passed_by_value);
	xml_serializer->AddAttribute(
		CDXLTokens::GetDXLTokenStr(EdxltokenValue), m_is_null, GetByteArray(), Length());
	xml_serializer->AddAttribute(CDXLTokens::GetDXLTokenStr(EdxltokenDoubleValue),
								 GetDoubleMapping());
}

// EOF
