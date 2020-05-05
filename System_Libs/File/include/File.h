//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FILE_H
#define FILE_H

// Make the assumption of c-char strings, not UNICODE

class File
{
public:

	typedef void *Handle;

	enum class Mode
	{
		READ = 0x7A000000,
		WRITE,
		READ_WRITE
	};

	enum class Location
	{
		BEGIN = 0x7B000000,
		CURRENT,
		END
	};

	enum class Error
	{
		SUCCESS = 0x7C000000,
		OPEN_FAIL,
		CLOSE_FAIL,
		WRITE_FAIL,
		READ_FAIL,
		SEEK_FAIL,
		TELL_FAIL,
		FLUSH_FAIL,
		UNDEFINED
	};

public:
   static File::Error Open( File::Handle &fh, const char * const fileName, File::Mode mode ) noexcept;
   static File::Error Close( File::Handle fh ) noexcept;
   static File::Error Write( File::Handle fh, const void * const buffer, const DWORD inSize ) noexcept;
   static File::Error Read( File::Handle fh, void * const _buffer, const DWORD _size ) noexcept;
   static File::Error Seek( File::Handle fh, File::Location location, int offset ) noexcept;
   static File::Error Tell( File::Handle fh, DWORD &offset ) noexcept;
   static File::Error Flush( File::Handle fh ) noexcept;
   static bool IsHandleValid(File::Handle fh) noexcept;

private:
   static DWORD privGetFileDesiredAccess( File::Mode mode ) noexcept;
   static DWORD privGetSeek( File::Location location ) noexcept;
};

#endif

// ---  End of File ---------------
