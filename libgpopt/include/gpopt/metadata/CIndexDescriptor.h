//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CIndexDescriptor.h
//
//	@doc:
//		Base class for index descriptor
//---------------------------------------------------------------------------
#ifndef GPOPT_CIndexDescriptor_H
#define GPOPT_CIndexDescriptor_H

#include "gpos/base.h"
#include "gpos/common/CDynamicPtrArray.h"

#include "naucrates/md/IMDId.h"
#include "naucrates/md/IMDIndex.h"

#include "gpopt/base/CColRef.h"
#include "gpopt/metadata/CTableDescriptor.h"
#include "gpopt/metadata/CColumnDescriptor.h"

namespace gpopt
{
	using namespace gpos;
	using namespace gpmd;

	//---------------------------------------------------------------------------
	//	@class:
	//		CIndexDescriptor
	//
	//	@doc:
	//		Base class for index descriptor
	//
	//---------------------------------------------------------------------------
	class CIndexDescriptor : public CRefCount
	{
	private:
		// mdid of the index
		IMDId *m_pmdidIndex;

		// name of index
		CName m_name;

		// array of index key columns
		CColumnDescriptorArray *m_pdrgpcoldescKeyCols;

		// array of index included columns
		CColumnDescriptorArray *m_pdrgpcoldescIncludedCols;

		// clustered index
		BOOL m_clustered;

		// index type
		IMDIndex::EmdindexType m_emdindt;

		// private copy ctor
		CIndexDescriptor(const CIndexDescriptor &);

	public:
		// ctor
		CIndexDescriptor(IMemoryPool *mp,
						 IMDId *pmdidIndex,
						 const CName &name,
						 CColumnDescriptorArray *pdrgcoldescKeyCols,
						 CColumnDescriptorArray *pdrgcoldescIncludedCols,
						 BOOL is_clustered,
						 IMDIndex::EmdindexType emdindt);

		// dtor
		virtual ~CIndexDescriptor();

		// number of key columns
		ULONG Keys() const;

		// number of included columns
		ULONG UlIncludedColumns() const;

		// index mdid accessor
		IMDId *
		MDId() const
		{
			return m_pmdidIndex;
		}

		// index name
		const CName &
		Name() const
		{
			return m_name;
		}

		// key column descriptors
		CColumnDescriptorArray *
		PdrgpcoldescKey() const
		{
			return m_pdrgpcoldescKeyCols;
		}

		// included column descriptors
		CColumnDescriptorArray *
		PdrgpcoldescIncluded() const
		{
			return m_pdrgpcoldescIncludedCols;
		}

		// is index clustered
		BOOL
		IsClustered() const
		{
			return m_clustered;
		}

		IMDIndex::EmdindexType
		IndexType() const
		{
			return m_emdindt;
		}

		// create an index descriptor
		static CIndexDescriptor *Pindexdesc(IMemoryPool *mp,
											const CTableDescriptor *ptabdesc,
											const IMDIndex *pmdindex);

#ifdef GPOS_DEBUG
		IOstream &OsPrint(IOstream &) const;
#endif  // GPOS_DEBUG

	};  // class CIndexDescriptor
}  // namespace gpopt

#endif  // !GPOPT_CIndexDescriptor_H

// EOF
