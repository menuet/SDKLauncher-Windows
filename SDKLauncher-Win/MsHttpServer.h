// 
//  SDKLauncherWin.h : main header file for the PROJECT_NAME application
//
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met :
//  1. Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation and / or
//  other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may
//  be used to endorse or promote products derived from this software without
//  specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//  THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <http.h>
#include <stdio.h>
#include <string>
#include <vector>

#pragma comment(lib, "httpapi.lib")

// Macros
#define INITIALIZE_HTTP_RESPONSE( resp, status, reason )	\
do															\
    {                                                       \
    RtlZeroMemory((resp), sizeof(*(resp)));					\
    (resp)->StatusCode = (status);							\
    (resp)->pReason = (reason);								\
    (resp)->ReasonLength = (USHORT)strlen(reason);			\
    } while (FALSE)
#define ADD_KNOWN_HEADER(Response, HeaderId, RawValue)      \
do                                                          \
    {                                                       \
    (Response).Headers.KnownHeaders[(HeaderId)].pRawValue = \
    (RawValue);												\
    (Response).Headers.KnownHeaders[(HeaderId)].RawValueLength = \
    (USHORT)strlen(RawValue);                               \
    } while (FALSE)
#define ALLOC_MEM(cb) HeapAlloc(GetProcessHeap(), 0, (cb))
#define FREE_MEM(ptr) HeapFree(GetProcessHeap(), 0, (ptr))

extern bool getResponseStringAndMime(PCSTR rawURL, BYTE** bytes, ULONGLONG* pSize, std::string& mimeTxt);
bool getResponseSizeAndMime(PCSTR rawURL, ULONGLONG* pSize, std::string& mimeTxt);
bool getByteRespBegEnd(PCSTR rawURL, ULONGLONG begin, ULONGLONG end, BYTE** bytes, ULONGLONG* pSize, std::string& mimeTxt);
// TODO: THIS IS the place to create response

class MSHttpAPIWrapper
{
private:
    std::vector<std::wstring> arrOfURLs;

    ULONG			retCode;
    bool			bHTTPInitialized;
    HANDLE          hReqQueue;		// = NULL;
    HTTPAPI_VERSION HttpApiVersion;	// = HTTPAPI_VERSION_1;
    PCHAR           pRequestBuffer; // HTTP memory buffer
    ULONG           RequestBufferLength; // HTTP memory buffer length
    CWinThread		*pHTTPServerThread;
    bool			bStop;


    void Cleanup();
    ULONG DoReceiveRequests();
    DWORD SendHttpResponse(IN PHTTP_REQUEST pRequest, IN USHORT StatusCode, IN PSTR pReason, IN PSTR pEntityString);
    static UINT MSHTTPServerThread(void* pThis_);
    void stopMsHTTPServerThread();
    void allocBufferMem(int iMemSize);
    void freeBufferMem();

public:
    MSHttpAPIWrapper();
    ~MSHttpAPIWrapper();
    void Init(std::vector<std::wstring>& rArrOfURLs);
    void startMsHTTPServerThread();
};