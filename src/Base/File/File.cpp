#include "File.hpp"
#include <SDL3/SDL_log.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#endif
#include <string>

bool File::Exists(const std::string& path) {
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
}

Error File::CreateDir(const std::string& path) {
    if (File::IsDir(path)) {
        return Error::SUCCESS;
    }

#ifdef _WIN32
    std::string fullPath;
    for (size_t i = 0; i < path.size(); ++i) {
        fullPath += path[i];
        if (path[i] == '/' || path[i] == '\\' || i == path.size() - 1) {
            if (!fullPath.empty() && fullPath.back() != ':') {
                if (!File::Exists(fullPath)) {
                    if (_mkdir(fullPath.c_str()) != 0) {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File: failed to create directory %s", fullPath.c_str());
                        return Error::ERR;
                    }
                }
            }
        }
    }
    SDL_Log("File: created directory %s", path.c_str());
    return Error::SUCCESS;
#else
    if (mkdir(path.c_str(), 0755) == 0) {
        SDL_Log("File: created directory %s", path.c_str());
        return Error::SUCCESS;
    }
    if (errno == EEXIST) {
        return Error::SUCCESS;
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File: failed to create directory %s", path.c_str());
    return Error::ERROR_;
#endif
}

bool File::IsDir(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) == 0) {
        return (buffer.st_mode & S_IFDIR) != 0;
    }
    return false;
}
