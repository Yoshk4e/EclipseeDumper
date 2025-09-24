#include "pch.h"
#include "ConsoleProgressReporter.h"
#include <iomanip>

namespace EclipseKit::Core::Progress {

    ConsoleProgressReporter::ConsoleProgressReporter() {
        lastDraw_ = std::chrono::steady_clock::now();
    }

    void ConsoleProgressReporter::Initialize(uint64_t totalItems) {
        total_ = totalItems;
        lastPercent_ = -1;
        lastDraw_ = std::chrono::steady_clock::now();
    }

    void ConsoleProgressReporter::UpdateProgress(uint64_t currentIndex) {
        if (total_ == 0) return;
        int percent = static_cast<int>(100.0 * currentIndex / (total_ - 1));
        if (percent == lastPercent_) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDraw_).count() < 33) {
                return;
            }
        }
        lastPercent_ = percent;
        lastDraw_ = std::chrono::steady_clock::now();

        int pos = barWidth_ * percent / 100;
        // Clear line and start cyan bar
        std::cout << "\r\33[2K\033[36m[";
        for (int i = 0; i < barWidth_; i++) {
            std::cout << (i <= pos ? '#' : ' ');
        }
        // Close bar and reset color
        std::cout << "]\033[0m ";

        // Color percentage: green at 100%, yellow otherwise
        const char* pctColor = (percent >= 100) ? "\033[92m" : "\033[93m";
        std::cout << pctColor << std::fixed << std::setprecision(1)
                  << percent << "%\033[0m (" << currentIndex << "/" << (total_ - 1) << ")" << std::flush;

        if (callback_) {
            callback_(percent / 100.0);
        }
    }

    void ConsoleProgressReporter::Finalize() {
        std::cout << "\n";
    }

    void ConsoleProgressReporter::SetProgressCallback(std::function<void(double)> callback) {
        callback_ = std::move(callback);
    }
}
