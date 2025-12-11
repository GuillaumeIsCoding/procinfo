#pragma once
#ifndef PROC_INFO_HEADER_HPP
#define PROC_INFO_HEADER_HPP

#ifndef __linux__
#define __linux__ 0
#endif // _WIN32

#ifndef _WIN32
#define _WIN32 0
#endif // _WIN32

#if __linux__

    #include <sys/types.h>

#elif _WIN32

    #include <windows.h>

#endif // __linux__

typedef unsigned int FLAGS;

#if __linux__
    typedef pid_t PID, THREAD_ID;
    typedef void* PROCESS_HANDLE;
#elif _WIN32
    typedef DWORD PID, THREAD_ID;
    typedef HANDLE PROCESS_HANDLE;
#endif // __linux__

#endif // PROC_INFO_HEADER_HPP