//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2010 Greenplum, Inc.
//
//	@filename:
//		CStringBase.h
//
//	@doc:
//		Abstract wide character string class
//---------------------------------------------------------------------------
#ifndef GPOS_CStringBase_H
#define GPOS_CStringBase_H

#include "gpos/types.h"
#include "gpos/common/clibwrapper.h"

#define GPOS_SZ_LENGTH(x) gpos::clib::UlStrLen(x)
#define GPOS_SZ_STR_LENGTH(x) (x), GPOS_SZ_LENGTH((x))

#define CHAR_EOS '\0'


namespace gpos
{
	class CStringConst;
	class IMemoryPool;
	
	//---------------------------------------------------------------------------
	//	@class:
	//		CStringBase
	//
	//	@doc:
	//		Abstract wide character string class.
	//		Currently, the class has two derived classes: CStringConst and CWString.
	//		CWString is used to represent constant strings that once initialized are never
	//		modified. This class is not responsible for any memory management, rather
	//		its users are in charge for allocating and releasing the necessary memory.
	//		In contrast, CWString can be used to store strings that are modified after
	//		they are created. CWString is in charge of dynamically allocating and deallocating
	//		memory for storing the characters of the string.
	//
	//---------------------------------------------------------------------------
	class CStringBase
	{
		private:
			// private copy ctor
			CStringBase(const CStringBase&);
			
		protected:
			// represents end-of-wide-string character
			static const CHAR m_wcEmpty;
			
			// size of the string in number of CHAR units (not counting the terminating '\0')
			ULONG m_ulLength;
			
			// whether string owns its memory and should take care of deallocating it at destruction time
			BOOL m_fOwnsMemory;
			
			// checks whether the string is byte-wise equal to a given string literal
			virtual BOOL FEquals(const CHAR *SZBuf) const;
			
		public:
			
			// ctor
			CStringBase(ULONG ulLength, BOOL fOwnsMemory)
				:
				m_ulLength(ulLength),
				m_fOwnsMemory(fOwnsMemory)
			{}
			
			// dtor
			virtual ~CStringBase(){}
			
			// deep copy of the string
			virtual CStringConst *PStrCopy(IMemoryPool *pmp) const;

			// accessors
			virtual ULONG UlLength() const;
			
			// checks whether the string is byte-wise equal to another string
			virtual BOOL FEquals(const CStringBase *pStr) const;			
			
			// checks whether the string contains any characters
			virtual BOOL FEmpty() const;
			
			// checks whether a string is properly null-terminated
			bool FValid() const;
			
			// equality operator 
			BOOL operator == (const CStringBase &str) const;

			// returns the wide character buffer storing the string
			virtual const CHAR* Sz() const = 0;

			// returns the index of the first occurrence of a character, -1 if not found
			INT IFind(CHAR wc) const;

			// checks if a character is escaped
			BOOL FEscaped(ULONG ulOfst) const;

			// count how many times the character appears in string
			ULONG UlOccurences(const CHAR wc) const;
	};
	
}

#endif // !GPOS_CStringBase_H

// EOF

