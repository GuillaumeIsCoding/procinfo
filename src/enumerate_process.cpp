#include "include/enumerate_process.hpp"

#include "include/process.hpp"

#include "include/header.hpp"

#if __linux__

    #include <dirent.h>

#elif _WIN32

    #include <tlhelp32.h>
    #include <psapi.h>
    #pragma comment(lib, "psapi.lib")

#endif // __linux__

static std::shared_ptr<Process> create_process(PID pid)
{
    return Process::Create(pid);
}

std::unordered_map<std::size_t, std::shared_ptr<Process>> enumerate_process()
{
    std::unordered_map<std::size_t, std::shared_ptr<Process>> map;

    #if __linux__

    DIR* directory = opendir("/proc");

    if (!dir)
    {
        return map;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr)
    {
        bool is_numeric = true;

        for (int i = 0; entry->d_name[i] != '\0'; i++)
        {
            if (!isdigit(entry->d_name[i]))
            {
                is_numeric = false;
                break;
            }
        }

        if (!is_numeric) continue;

        auto process = create_process(std::stoul(entry->d_name));
        map[std::hash<Process>{}(*process)] = process;
    }

    closedir(directory);

    #elif _WIN32

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return map;
    }

    PROCESSENTRY32W process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &process_entry))
    {
        do
        {
            auto process = create_process(process_entry.th32ProcessID);
            map[std::hash<Process>{}(*process)] = process;
        } while (Process32NextW(snapshot, &process_entry));
    }

    #endif // __linux__

    return map;
}