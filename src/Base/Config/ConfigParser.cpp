#include "ConfigParser.hpp"
#include <SDL3/SDL_log.h>

namespace config {

Error ConfigParser::Init(const std::string& filePath) {
    m_parser = std::make_unique<ParserIni>();
    return m_parser->Load(filePath);
}

Error ConfigParser::GetValue(const std::string& section, const std::string& key, std::string& value) const {
    if (!m_parser || !m_parser->IsLoaded()) return Error::ERROR_INIT;
    return m_parser->GetString(section, key, value);
}

Error ConfigParser::GetValue(const std::string& section, const std::string& key, float& value) const {
    if (!m_parser || !m_parser->IsLoaded()) return Error::ERROR_INIT;
    return m_parser->GetFloat(section, key, value);
}

Error ConfigParser::GetValue(const std::string& section, const std::string& key, int& value) const {
    if (!m_parser || !m_parser->IsLoaded()) return Error::ERROR_INIT;
    return m_parser->GetInt(section, key, value);
}

Error ConfigParser::GetValue(const std::string& section, const std::string& key, unsigned int& value) const {
    if (!m_parser || !m_parser->IsLoaded()) return Error::ERROR_INIT;
    int v = 0;
    Error err = m_parser->GetInt(section, key, v);
    if (err == Error::SUCCESS) {
        value = static_cast<unsigned int>(v);
    }
    return err;
}

Error ConfigParser::GetValue(const std::string& section, const std::string& key, bool& value) const {
    if (!m_parser || !m_parser->IsLoaded()) return Error::ERROR_INIT;
    return m_parser->GetBool(section, key, value);
}

} // namespace config
