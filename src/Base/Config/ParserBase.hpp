#ifndef PARSER_BASE_HPP_
#define PARSER_BASE_HPP_

#include "Game/Error.hpp"
#include <string>

namespace config {

class ParserBase {
public:
    virtual ~ParserBase() = default;
    virtual Error Load(const std::string& path) = 0;
    virtual bool IsLoaded() const = 0;
    virtual bool Exists(const std::string& section, const std::string& key) const = 0;
    virtual Error GetInt(const std::string& section, const std::string& key, int& value) const = 0;
    virtual Error GetFloat(const std::string& section, const std::string& key, float& value) const = 0;
    virtual Error GetBool(const std::string& section, const std::string& key, bool& value) const = 0;
    virtual Error GetString(const std::string& section, const std::string& key, std::string& value) const = 0;
};

} // namespace config

#endif // PARSER_BASE_HPP_
