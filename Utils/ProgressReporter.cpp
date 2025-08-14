#include "pch.h"
#include "ProgressReporter.h"
#include <iostream>
#include <iomanip>

ProgressReporter::ProgressReporter(int totalItems, int barWidth)
    : m_total(totalItems), m_width(barWidth),
    m_lastDraw(std::chrono::steady_clock::now())
{
}

bool ProgressReporter::update(int currentIndex) {
    // Compute percent
    int percent = static_cast<int>(100.0 * currentIndex / (m_total - 1));
    if (percent == m_lastPercent) {
        // Throttle to ~30 FPS
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastDraw).count() < 33)
            return false;
    }
    m_lastPercent = percent;
    m_lastDraw = std::chrono::steady_clock::now();

    // Build bar
    int pos = m_width * percent / 100;
    std::cout << "\r\33[2K[";

    for (int i = 0; i < m_width; i++)
        std::cout << (i <= pos ? '#' : ' ');

    std::cout
        << "] " << std::fixed << std::setprecision(1)
        << percent << "% (" << currentIndex << "/" << (m_total - 1) << ")"
        << std::flush;

    return true;
}

void ProgressReporter::finish() {
    std::cout << "\n";
}
