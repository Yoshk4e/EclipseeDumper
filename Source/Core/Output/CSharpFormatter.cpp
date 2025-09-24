#include "pch.h"
#include "CSharpFormatter.h"
#include <sstream>

namespace EclipseKit::Core::Output {

    std::string CSharpFormatter::FormatOutput(const std::vector<std::string>& dumpedTypes) const {
        std::stringstream ss;
        for (const auto& typeStr : dumpedTypes) {
            ss << typeStr << "\n\n";
        }
        return ss.str();
    }
}
