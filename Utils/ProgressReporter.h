#pragma once
#include <string>
#include <chrono>

class ProgressReporter {
public:
    ProgressReporter(int totalItems, int barWidth = 50);

    /// Call every iteration with the current index (0-based).
    /// Returns false if you should “throttle” (i.e. skip redraw).
    bool update(int currentIndex);

    /// Call once at the end to clear or finalize the line.
    void finish();

private:
    int   m_total;
    int   m_width;
    int   m_lastPercent{ -1 };
    std::chrono::steady_clock::time_point m_lastDraw;
};
