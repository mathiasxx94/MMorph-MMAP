#include "pch.h"

#include "Tools.h"

bool FileExists(const wchar_t * szFile)
{
	return (GetFileAttributesW(szFile) != INVALID_FILE_ATTRIBUTES);
}