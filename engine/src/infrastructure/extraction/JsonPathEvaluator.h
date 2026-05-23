#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace chainapi::engine {

class JsonPathEvaluator {
public:
    /// Returns the matched value as a stringified scalar; nullopt on miss.
    std::optional<std::string> evaluate(std::string_view json,
                                         std::string_view jsonpath) const;
};

}  // namespace chainapi::engine
