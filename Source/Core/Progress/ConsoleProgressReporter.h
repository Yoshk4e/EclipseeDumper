#pragma once

#include <functional>
#include <chrono>
#include <iostream>
#include "../Interfaces/IProgressReporter.h"

namespace EclipseKit::Core::Progress {

    class ConsoleProgressReporter final : public Interfaces::IProgressReporter {
    public:
        ConsoleProgressReporter();
        ~ConsoleProgressReporter() override = default;

        void Initialize(uint64_t totalItems) override;
        void UpdateProgress(uint64_t currentIndex) override;
        void Finalize() override;
        void SetProgressCallback(std::function<void(double)> callback) override;

    private:
        uint64_t total_{0};
        int lastPercent_{-1};
        int barWidth_{50};
        std::chrono::steady_clock::time_point lastDraw_{};
        std::function<void(double)> callback_{};
    };
}
