#pragma once

#include <string>
#include <vector>
#include "../../Core/Interfaces/IOutputFormatter.h"

namespace EclipseKit::Core::Output {

    class CSharpFormatter final : public Interfaces::IOutputFormatter {
    public:
        ~CSharpFormatter() override = default;

        std::string FormatOutput(const std::vector<std::string>& dumpedTypes) const override;
        std::string GetFileExtension() const override { return ".cs"; }
        std::string GetFormatterName() const override { return "CSharp"; }
    };
}
