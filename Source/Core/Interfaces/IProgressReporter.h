#pragma once

#include <functional>

namespace EclipseKit::Core::Interfaces {

    /// Interface for progress reporting during dump operations
    class IProgressReporter {
    public:
        virtual ~IProgressReporter() = default;

        /// Initialize progress tracking
        virtual void Initialize(uint64_t totalItems) = 0;

        /// Update progress with current item index
        virtual void UpdateProgress(uint64_t currentIndex) = 0;

        /// Finalize progress reporting
        virtual void Finalize() = 0;

        /// Set custom progress callback
        virtual void SetProgressCallback(std::function<void(double)> callback) = 0;
    };

    using ProgressReporterPtr = std::unique_ptr<IProgressReporter>;
}
