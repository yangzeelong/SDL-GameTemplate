#ifndef PARSER_INI_HPP_
#define PARSER_INI_HPP_

#include "ParserBase.hpp"
#include <map>
#include <string>

namespace config {

class ParserIni : public ParserBase {
public:
    Error Load(const std::string& path) override;
    bool IsLoaded() const override { return m_loaded; }
    bool Exists(const std::string& section, const std::string& key) const override;

    Error GetInt(const std::string& section, const std::string& key, int& value) const override;
    Error GetFloat(const std::string& section, const std::string& key, float& value) const override;
    Error GetBool(const std::string& section, const std::string& key, bool& value) const override;
    Error GetString(const std::string& section, const std::string& key, std::string& value) const override;

private:
    static std::string Trim(const std::string& s);

    bool m_loaded = false;
    std::map<std::string, std::map<std::string, std::string>> m_data;
};

} // namespace config

#endif // PARSER_INI_HPP_
