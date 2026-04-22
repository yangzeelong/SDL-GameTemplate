#ifndef CONFIG_PARSER_HPP_
#define CONFIG_PARSER_HPP_

#include "ParserBase.hpp"
#include "ParserIni.hpp"
#include <memory>
#include <string>

namespace config {

class ConfigParser {
public:
    static ConfigParser& GetInstance() {
        static ConfigParser instance;
        return instance;
    }

    Error Init(const std::string& filePath);
    bool IsInit() const { return m_parser && m_parser->IsLoaded(); }

    Error GetValue(const std::string& section, const std::string& key, std::string& value) const;
    Error GetValue(const std::string& section, const std::string& key, float& value) const;
    Error GetValue(const std::string& section, const std::string& key, int& value) const;
    Error GetValue(const std::string& section, const std::string& key, unsigned int& value) const;
    Error GetValue(const std::string& section, const std::string& key, bool& value) const;

    template <typename T>
    T GetValueOrDefault(const std::string& section, const std::string& key, const T& defaultValue) const {
        T value;
        if (GetValue(section, key, value) != Error::SUCCESS) {
            value = defaultValue;
        }
        return value;
    }

private:
    ConfigParser() = default;
    ConfigParser(const ConfigParser&) = delete;
    ConfigParser& operator=(const ConfigParser&) = delete;

    std::unique_ptr<ParserBase> m_parser;
};

} // namespace config

// clang-format off

#define CONFIG_INIT(path) \
    config::ConfigParser::GetInstance().Init(path)

#define CONFIG_IS_INIT() \
    config::ConfigParser::GetInstance().IsInit()

#define CONFIG_GET_VALUE(section, key, outValue) \
    config::ConfigParser::GetInstance().GetValue(section, key, outValue)

#define CONFIG_GET_VALUE_OR_DEFAULT(section, key, defaultValue) \
    config::ConfigParser::GetInstance().GetValueOrDefault(section, key, defaultValue)

// clang-format on

#endif // CONFIG_PARSER_HPP_
