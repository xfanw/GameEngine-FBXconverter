//-----------------------------------------------------------------------------
// Copyright 2020, Ed  all rights reserved.
//----------------------------------------------------------------------------- 
//
// Align16
//      Needed a clean way to allocate aligned data for Matrix, Quat, and Vector
//      Data is align by inheriting this class
//      
//
//      Used to be stand alone files... now all one include
//
//----------------------------------------------------------------------------- 

#ifndef ALIGN_16_H
#define ALIGN_16_H

#include <stdlib.h>  
#include <crtdbg.h>  

		#define ALIGN_UNUSED_VAR(x) (void(x))

		#pragma push_macro("new")
		#undef new

		class Align16
		{
		public:

	// ------------------------------------------------------
	// Operators new/delete with memory tracking off
	// ------------------------------------------------------

			// Placement new for align16
			void* operator new(size_t, void *p) noexcept
			{
				ALIGN_UNUSED_VAR(p);
				return p;
			}

			void *operator new(size_t size) noexcept
			{
				void *p = _aligned_malloc(size, 16);
				return p;
			}

			void operator delete(void *p)
			{
				_aligned_free(p);
			}

			void *operator new[](size_t size) noexcept
			{
				void *p = _aligned_malloc(size, 16);
				return p;
			}

			void operator delete[](void *p)
			{
				_aligned_free(p);
			}

	// ------------------------------------------------------
	// Operators new/delete with memory tracking on
	// ------------------------------------------------------

			void *operator new(size_t      size,
								int         _BlockUse,
								char const* _FileName,
								int         _LineNumber) noexcept
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);

				void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
				return p;
			}

			void operator delete(void        *p,
									int         _BlockUse,
									char const* _FileName,
									int         _LineNumber)
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);
				_aligned_free_dbg(p);
			}

			void *operator new[](size_t        size,
									int         _BlockUse,
									char const* _FileName,
									int         _LineNumber)  noexcept
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);

				void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
				return p;
			}

			void operator delete[](void      *p,
									int         _BlockUse,
									char const* _FileName,
									int         _LineNumber)
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);
				_aligned_free_dbg(p);
			}

		};

		#pragma pop_macro("new")

#endif ALIGN_16_H

// ---  End of File ---------------
