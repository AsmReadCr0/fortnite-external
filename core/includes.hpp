#pragma once
#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#define null NULL
#define m_pi 3.14159265358979323846

//winapi
#include <windows.h>

//c++ standard
#include <numbers>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <bit>
#include <memory>

//cipher
#include <dependencies/cipher/hash.hpp>
#include <dependencies/cipher/imports.hpp>

//vars
#include <dependencies/vars/vars.hpp>

//sdk
#include <core/sdk/offsets.hpp>
#include <core/sdk/math/primitives.hpp>

inline std::string get_relative_file_path(const char* file)
{
    const char* relative_path = strstr(file, "[FN]");
    return relative_path ? std::string(relative_path) : std::string(file);
}

#define log_normal(message) \
    std::cout << "[\033[97mapel\033[0m.\033[94m\033[0m] -> \033[90m" << message << "\033[0m : " << get_relative_file_path(__FILE__) << ":" << __LINE__ << "\033[0m" << std::endl;

#define log_event() \
    std::cout << "[\033[97mapel\033[0m.\033[94m\033[0m] -> \033[90m" << get_relative_file_path(__FILE__) << ":" << __LINE__ << "\033[0m" << std::endl;

#define log_error() \
    std::cout << "[\033[91mexception\033[0m] -> \033[90m" << get_relative_file_path(__FILE__) << ":" << __LINE__ << "\033[0m" << std::endl;

#endif