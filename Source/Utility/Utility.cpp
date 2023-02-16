//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "Utility.h"

namespace fw {

void OutputMessage(const char* message, ...)
{
#define MAX_MESSAGE 1024
    char szBuff[MAX_MESSAGE];
    va_list arg;
    va_start(arg, message);
    vsnprintf_s( szBuff, sizeof(szBuff), _TRUNCATE, message, arg );
    va_end(arg);

    szBuff[MAX_MESSAGE-1] = 0; // vsnprintf_s might do this, but docs are unclear.
    OutputDebugString( szBuff );
}

char* LoadCompleteFile(const char* filename, uint32* length)
{
    char* fileContents = 0;

    FILE* fileHandle;
    errno_t error = fopen_s( &fileHandle, filename, "rb" );

    if( fileHandle )
    {
        fseek( fileHandle, 0, SEEK_END );
        uint32 size = ftell( fileHandle );
        rewind( fileHandle );

        fileContents = new char[size+1];
        fread( fileContents, size, 1, fileHandle );
        fileContents[size] = 0;

        if( length )
            *length = size;

        fclose( fileHandle );
    }

    return fileContents;
}

void SaveCompleteFile(const char* filename, const char* fileContents, uint32 length)
{
    FILE* fileHandle;
    errno_t error = fopen_s( &fileHandle, filename, "wb" );

    if( fileHandle )
    {
        fwrite( fileContents, length, 1, fileHandle );
        fclose( fileHandle );
    }
}

double GetSystemTime()
{
    uint64 freq;
    uint64 time;

    QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );
    QueryPerformanceCounter( (LARGE_INTEGER*)&time );

    double timeseconds = (double)time / freq;

    return timeseconds;
}

double GetSystemTimeSinceGameStart()
{
    static double starttime = -1;

    if( starttime == -1 )
        starttime = GetSystemTime();

    return GetSystemTime() - starttime;
}

std::string GetFileNameFromPath(const char* path)
{
    std::string filename = path;

    size_t lastSlash = filename.find_last_of( "\\/" );
    if( lastSlash != std::string::npos )
        filename = filename.substr( lastSlash+1 );

    return filename;    
}

} // namespace fw
