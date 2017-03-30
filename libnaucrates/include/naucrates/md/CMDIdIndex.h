//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2013 EMC Corp.
//
//	@filename:
//		CMDIdIndex.h
//
//	@doc:
//		Class for representing mdids of index
//---------------------------------------------------------------------------

#ifndef GPMD_CMDIdIndexFunc_H
#define GPMD_CMDIdIndexFunc_H

#include "gpos/base.h"
#include "gpos/common/CDynamicPtrArray.h"
#include "gpos/string/CWStringConst.h"

#include "naucrates/dxl/gpdb_types.h"

#include "naucrates/md/CMDIdGPDB.h"
#include "naucrates/md/CSystemId.h"

namespace gpmd
{
	using namespace gpos;


	//---------------------------------------------------------------------------
	//	@class:
	//		CMDIdIndex
	//
	//	@doc:
	//		Class for representing ids of cast objects
	//
	//---------------------------------------------------------------------------
	class CMDIdIndex : public IMDId
	{
		private:
			// aggregate index or not
			BOOL m_fAggregateIndex;
	
			// buffer for the serialized mdid
			WCHAR m_wszBuffer[GPDXL_MDID_LENGTH];
			
			// string representation of the mdid
			CWStringStatic m_str;
			
			// private copy ctor
			CMDIdIndex(const CMDIdIndex &);
			
			// serialize mdid
			void Serialize();
			
		public:
			// ctor
			CMDIdIndex(BOOL fAggregateIndex);
			
			// dtor
			virtual
			~CMDIdIndex();
			
			// string representation of mdid
			virtual
			const WCHAR *Wsz() const;
			
			// serialize mdid in DXL as the value of the specified attribute 
			virtual
			void Serialize(CXMLSerializer *pxmlser, const CWStringConst *pstrAttribute) const;
						
			// debug print of the metadata id
			virtual
			IOstream &OsPrint(IOstream &os) const;
			
			// const converter
			static
			const CMDIdIndex *PmdidConvert(const IMDId *pmdid)
			{
				GPOS_ASSERT(NULL != pmdid);

				return dynamic_cast<const CMDIdIndex *>(pmdid);
			}
			
			// non-const converter
			static
			CMDIdIndex *PmdidConvert(IMDId *pmdid)
			{
				GPOS_ASSERT(NULL != pmdid);

				return dynamic_cast<CMDIdIndex *>(pmdid);
			}
	};
}

#endif // !GPMD_CMDIdIndexFunc_H

// EOF
