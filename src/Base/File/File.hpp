#ifndef BASE_FILE_HPP_
#define BASE_FILE_HPP_

#include "Game/Error.hpp"
#include <string>

class File {
public:
    // Check if file/directory exists
    static bool Exists(const std::string& path);

    // Create a directory
    static Error CreateDir(const std::string& path);

    // Check if path is a directory
    static bool IsDir(const std::string& path);
};

#endif
