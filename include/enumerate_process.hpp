#pragma once
#ifndef PROC_INFO_ENUMERATE_PROCESS_HPP
#define PROC_INFO_ENUMERATE_PROCESS_HPP

#include <memory>

#include <unordered_map>

// forward declaration 
class Process;

std::unordered_map<std::size_t, std::shared_ptr<Process>> enumerate_process();

#endif // PROC_INFO_ENUMERATE_PROCESS_HPP