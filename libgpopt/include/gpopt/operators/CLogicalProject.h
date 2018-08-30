//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CLogicalProject.h
//
//	@doc:
//		Project operator
//---------------------------------------------------------------------------
#ifndef GPOS_CLogicalProject_H
#define GPOS_CLogicalProject_H

#include "gpos/base.h"

#include "gpopt/operators/CExpressionHandle.h"
#include "gpopt/operators/CLogicalUnary.h"

namespace gpopt
{
	// fwd declaration
	class CColRefSet;
	
	//---------------------------------------------------------------------------
	//	@class:
	//		CLogicalProject
	//
	//	@doc:
	//		Project operator
	//
	//---------------------------------------------------------------------------
	class CLogicalProject : public CLogicalUnary
	{
		private:

			// private copy ctor
			CLogicalProject(const CLogicalProject &);

			// return equivalence class from scalar ident project element
			static
			CColRefSetArray *PdrgpcrsEquivClassFromScIdent
								(
								IMemoryPool *mp,
								CExpression *pexprPrEl,
								CColRefSet *not_null_columns
								);

			// extract constraint from scalar constant project element
			static
			void ExtractConstraintFromScConst
					(
					IMemoryPool *mp,
					CExpression *pexprPrEl,
					CConstraintArray *pdrgpcnstr,
					CColRefSetArray *pdrgpcrs
					);

		public:

			// ctor
			explicit
			CLogicalProject(IMemoryPool *mp);

			// dtor
			virtual
			~CLogicalProject()
			{}

			// ident accessors
			virtual 
			EOperatorId Eopid() const
			{
				return EopLogicalProject;
			}
			
			virtual 
			const CHAR *SzId() const
			{
				return "CLogicalProject";
			}

			//-------------------------------------------------------------------------------------
			// Derived Relational Properties
			//-------------------------------------------------------------------------------------

			// derive output columns
			virtual
			CColRefSet *PcrsDeriveOutput(IMemoryPool *mp, CExpressionHandle &exprhdl);
			
			// dervive keys
			virtual 
			CKeyCollection *PkcDeriveKeys(IMemoryPool *mp, CExpressionHandle &exprhdl) const;
					
			// derive max card
			virtual
			CMaxCard Maxcard(IMemoryPool *mp, CExpressionHandle &exprhdl) const;

			// derive constraint property
			virtual
			CPropConstraint *PpcDeriveConstraint(IMemoryPool *mp, CExpressionHandle &exprhdl) const;

			//-------------------------------------------------------------------------------------
			// Transformations
			//-------------------------------------------------------------------------------------

			// candidate set of xforms
			virtual
			CXformSet *PxfsCandidates(IMemoryPool *mp) const;

			// derive statistics
			virtual
			IStatistics *PstatsDerive
						(
						IMemoryPool *mp,
						CExpressionHandle &exprhdl,
						IStatisticsArray *stats_ctxt
						)
						const;

			//-------------------------------------------------------------------------------------
			//-------------------------------------------------------------------------------------
			//-------------------------------------------------------------------------------------

			// conversion function
			static
			CLogicalProject *PopConvert
				(
				COperator *pop
				)
			{
				GPOS_ASSERT(NULL != pop);
				GPOS_ASSERT(EopLogicalProject == pop->Eopid());
				
				return dynamic_cast<CLogicalProject*>(pop);
			}

	}; // class CLogicalProject

}

#endif // !GPOS_CLogicalProject_H

// EOF
