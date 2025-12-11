#pragma once
#ifndef PROC_INFO_PROCESS_HPP
#define PROC_INFO_PROCESS_HPP

#include "header.hpp"

#include <memory>
#include <vector>
#include <string>

#include <ostream>

#include <functional>

typedef struct Information
{
    PID pid;                    ///< Process id
    PID ppid;                   ///< Parent process id
    THREAD_ID thread_id;        ///< Thread id
    std::string name;           ///< Name of the process
    std::string path;           ///< Path to process executable
    std::string folder;  ///< Path to parent folder
    FLAGS flags;                ///< Creation flags
    PROCESS_HANDLE handle;      ///< Process handle

    std::string to_string() const;
} PROCESS_INFORMATION;

class Process
{
    public:
        Process(PID pid);
        ~Process();

        Process& ppid();
        Process& name();
        Process& path();
        Process& folder();
        Process& flags();
        Process& handle();

        PROCESS_INFORMATION get_information() const;

        std::string to_string() const;
        
        friend std::ostream& operator<<(std::ostream& os, const Process& p);

        static std::shared_ptr<Process> Create(PID pid);

    private:
        PROCESS_INFORMATION m_information;
};

namespace std
{
    template <>
    struct hash<Information>
    {
        std::size_t operator()(const Information& i) const noexcept
        {
            // pid is unique so lets use the process pid
            return i.pid;
        }
    };

    template <>
    struct hash<Process> 
    {
        std::size_t operator()(const Process& p) const noexcept
        {
            return std::hash<Information>{}(p.get_information());
        }
    };
}

#endif // PROC_INFO_PROCESS_HPP