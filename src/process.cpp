#include "process.hpp"

#if __linux__

    #include <dirent.h>

#elif _WIN32

    #include <tlhelp32.h>
    #include <tchar.h>
    #include <psapi.h>
    #include <winternl.h>
    #pragma comment(lib, "psapi.lib")
    #pragma comment(lib, "kernel32.lib")

#endif // __linux__

#include <sstream>

#if __linux__

#elif _WIN32

typedef NTSTATUS (NTAPI *PFN_NtQueryInformationProcess)(
    HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG
);

#endif // __linux__

Process::Process(PID pid)
{
    m_information.pid = pid;
}

Process::~Process() 
{
    #if __linux__

    #elif _WIN32

    if (m_information.handle != nullptr)
    {
        CloseHandle(m_information.handle);
    }

    #endif // __linux__
}

Process& Process::ppid()
{
    #if __linux__

    #elif _WIN32

    if (m_information.handle)
    {
        static PFN_NtQueryInformationProcess nt_query = nullptr;

        if (!nt_query)
        {
            HMODULE ntdll = GetModuleHandleA("ntdll.dll");
            nt_query = (PFN_NtQueryInformationProcess)GetProcAddress(ntdll, "NtQueryInformationProcess");
        }

        PROCESS_BASIC_INFORMATION pbi = {};
        PID ppid = 0;

        if (nt_query && nt_query(m_information.handle, ProcessBasicInformation, &pbi, sizeof(pbi), nullptr) == 0)
        {
            ppid = (DWORD)(ULONG_PTR)pbi.Reserved3;
        }
    }

    #endif // __linux__

    return *this;
}

Process& Process::name()
{
    #if __linux__

    #elif _WIN32

    TCHAR process_name[MAX_PATH] = TEXT("<unknown>");

    if (m_information.handle)
    {
        HMODULE module;
        DWORD cb_needed;

        if (EnumProcessModules(m_information.handle, &module, sizeof(HMODULE), &cb_needed))
        {
            GetModuleBaseName(m_information.handle, module, process_name, sizeof(process_name) / sizeof(TCHAR));
        }
    }

    #ifdef UNICODE

    int buffer_size = WideCharToMultiByte(CP_ACP, 0, process_name, -1, NULL, 0, NULL, NULL);

    if (buffer_size > 0)
    {
        char* char_buffer = new char[buffer_size];
        WideCharToMultiByte(CP_ACP, 0, process_name, -1, char_buffer, buffer_size, NULL, NULL);
        m_information.name = std::string(char_buffer);
        delete[] char_buffer;
    }
    
    #else

    m_information.name = std::string(process_name);
    
    #endif // UNICODE

    #endif // __linux__

    return *this;
}

Process& Process::path()
{
    #if __linux__

    #elif _WIN32

    TCHAR process_path[MAX_PATH] = TEXT("<unknown>");

    if (m_information.handle)
    {
        GetModuleFileNameEx(m_information.handle, NULL, process_path, MAX_PATH);
    }

    #ifdef UNICODE 

    int buffer_size = WideCharToMultiByte(CP_ACP, 0, process_path, -1, NULL, 0, NULL, NULL);

    if (buffer_size > 0)
    {
        char* char_buffer = new char[buffer_size];
        WideCharToMultiByte(CP_ACP, 0, process_path, -1, char_buffer, buffer_size, NULL, NULL);
        m_information.path = std::string(char_buffer);
        delete[] char_buffer;
    }

    #else 

    m_information.path = std::string(process_path);

    #endif // UNICODE
    

    #endif // __linux__

    return *this;
}

Process& Process::folder()
{
    #if __linux__

    #elif _WIN32

    std::vector<TCHAR> process_folder(MAX_PATH);
    DWORD size = process_folder.size();
    bool error = true;


    if (m_information.handle)
    {
        error = false;

        while (!QueryFullProcessImageName(m_information.handle, 0, process_folder.data(), &size))
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                process_folder.resize(process_folder.size() * 2);
                size = process_folder.size();
            }
            else
            {
                error = true;
                break;
            }
        }
    }

    if (error)
    {
        const TCHAR* unknown =TEXT("<unknown>");
        process_folder.clear();
        process_folder.insert(process_folder.end(), unknown, unknown + _tcslen(unknown) + 1);
    }

    #ifdef UNICODE

    int buffer_size = WideCharToMultiByte(CP_ACP, 0, process_folder.data(), -1, NULL, 0, NULL, NULL);

    if (buffer_size > 0)
    {
        char* char_buffer = new char[buffer_size];
        WideCharToMultiByte(CP_ACP, 0, process_folder.data(), -1, char_buffer, buffer_size, NULL, NULL);
        m_information.path = std::string(char_buffer);
        delete[] char_buffer;
    }

    #else

    m_information.path = std::string(process_folder.data());

    #endif // UNICODE

    #endif // __linux__

    return *this;
}

Process& Process::flags()
{
    #if __linux__

    #elif _WIN32

    #endif // __linux__

    return *this;
}

Process& Process::handle()
{
    #if __linux__

    #elif _WIN32

    m_information.handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION |
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        m_information.pid
    );

    #endif // __linux__

    return *this;
}

std::shared_ptr<Process> Process::Create(PID pid)
{
    auto process = Process(pid).handle()
                               .name()
                               .folder()
                               .path()
                               .ppid()
                               .flags();

    return std::make_shared<Process>(process);
}

std::string Information::to_string() const
{
    std::stringstream ss;
    
    ss << "Process name : " << name << "\n"
       << "\tProcess id (pid)           : " << pid      << "\n"
       << "\tParent process id (ppid)   : " << ppid     << "\n"
       << "\tProcess executable path    : " << path     << "\n"
       << "\tProcess folder             : " << folder   << "\n"
       << "\tProcess flags              : " << flags    << "\n"
       << "\tProcess handle             : " << handle   << "\n";
       
    return ss.str();
}

std::string Process::to_string() const
{
    return m_information.to_string();
}

std::ostream& operator<<(std::ostream& os, const Process& p)
{
    return os << p.to_string();
}