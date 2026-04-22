#include "ParserIni.hpp"
#include <SDL3/SDL_log.h>
#include <cctype>
#include <fstream>

namespace config {

Error ParserIni::Load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Config: failed to open %s", path.c_str());
        return Error::ERROR_INIT;
    }

    m_data.clear();
    std::string currentSection;
    std::string line;
    while (std::getline(file, line)) {
        line = Trim(line);
        if (line.empty()) continue;
        if (line[0] == '#' || line[0] == ';') continue;
        if (line[0] == '[') {
            size_t pos = line.find(']');
            if (pos != std::string::npos) {
                currentSection = Trim(line.substr(1, pos - 1));
            }
            continue;
        }
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = Trim(line.substr(0, eqPos));
            std::string val = Trim(line.substr(eqPos + 1));
            if (!currentSection.empty() && !key.empty()) {
                m_data[currentSection][key] = val;
            }
        }
    }

    m_loaded = true;
    SDL_Log("Config: loaded %zu sections from %s", m_data.size(), path.c_str());
    return Error::SUCCESS;
}

bool ParserIni::Exists(const std::string& section, const std::string& key) const {
    auto secIt = m_data.find(section);
    if (secIt == m_data.end()) return false;
    return secIt->second.find(key) != secIt->second.end();
}

std::string ParserIni::Trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

Error ParserIni::GetInt(const std::string& section, const std::string& key, int& value) const {
    auto secIt = m_data.find(section);
    if (secIt == m_data.end()) return Error::ERR;
    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end()) return Error::ERR;
    try {
        value = std::stoi(keyIt->second);
        return Error::SUCCESS;
    } catch (...) {
        return Error::ERR;
    }
}

Error ParserIni::GetFloat(const std::string& section, const std::string& key, float& value) const {
    auto secIt = m_data.find(section);
    if (secIt == m_data.end()) return Error::ERR;
    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end()) return Error::ERR;
    try {
        value = std::stof(keyIt->second);
        return Error::SUCCESS;
    } catch (...) {
        return Error::ERR;
    }
}

Error ParserIni::GetBool(const std::string& section, const std::string& key, bool& value) const {
    auto secIt = m_data.find(section);
    if (secIt == m_data.end()) return Error::ERR;
    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end()) return Error::ERR;
    std::string v = keyIt->second;
    for (auto& c : v) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    value = (v == "true" || v == "1" || v == "yes" || v == "on");
    return Error::SUCCESS;
}

Error ParserIni::GetString(const std::string& section, const std::string& key, std::string& value) const {
    auto secIt = m_data.find(section);
    if (secIt == m_data.end()) return Error::ERR;
    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end()) return Error::ERR;
    value = keyIt->second;
    return Error::SUCCESS;
}

} // namespace config
