#include "pch.h"
#include <functional>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <thread>
#include <windows.h>

// New modular architecture headers
#include "../Source/Core/Configuration/EngineConfiguration.h"
#include "../Source/Engines/Il2Cpp/Il2CppEngine.h"
#include "../Source/Core/Output/CSharpFormatter.h"
#include "../Source/Core/Progress/ConsoleProgressReporter.h"
#include "../Source/App/DumpController.h"
#include "../Source/Core/Factory/EngineFactory.h"
#include "../Source/Core/Output/ScriptJsonGenerator.h"

using namespace EclipseKit::Core::Configuration;
using EclipseKit::Engines::Il2Cpp::Il2CppEngine;
using EclipseKit::Core::Output::CSharpFormatter;
using EclipseKit::Core::Progress::ConsoleProgressReporter;
using EclipseKit::App::DumpController;
using EclipseKit::Core::Output::ScriptJsonGenerator;

static DWORD WINAPI UnloadDll(LPVOID module) {
    std::cout << "Dll Unloaded\n";
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)module, 0);
    return 0;
}

static void EnableAnsiConsole() {
    // Enable Windows 10+ virtual terminal processing so ANSI color codes work
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE && hOut != nullptr) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, mode);
        }
    }
    HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
    if (hErr != INVALID_HANDLE_VALUE && hErr != nullptr) {
        DWORD mode = 0;
        if (GetConsoleMode(hErr, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hErr, mode);
        }
    }
}

static inline void PrintSection(const char* title, const char* color = "\x1b[96m") {
    std::cout << "\n" << color << "-> " << title << "\x1b[0m\n";
}

DWORD WINAPI MainThread(LPVOID module) {
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    FILE* console;
    freopen_s(&console, "CONOUT$", "w", stdout);
    freopen_s(&console, "CONIN$", "r", stdin);

    // Initial messages
    std::cout << R"(
███████╗ ██████╗██╗     ██╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗███╗   ███╗██████╗ ███████╗██████╗ 
██╔════╝██╔════╝██║     ██║██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║████╗ ████║██╔══██╗██╔════╝██╔══██╗
█████╗  ██║     ██║     ██║██████╔╝███████╗█████╗  ██║  ██║██║   ██║██╔████╔██║██████╔╝█████╗  ██████╔╝
██╔══╝  ██║     ██║     ██║██╔═══╝ ╚════██║██╔══╝  ██║  ██║██║   ██║██║╚██╔╝██║██╔═══╝ ██╔══╝  ██╔══██╗
███████╗╚██████╗███████╗██║██║     ███████║███████╗██████╔╝╚██████╔╝██║ ╚═╝ ██║██║     ███████╗██║  ██║
╚══════╝ ╚═════╝╚══════╝╚═╝╚═╝     ╚══════╝╚══════╝╚═════╝  ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝
                                                                                                       )" << "\n";
    EnableAnsiConsole();
    std::cout << "Waiting for il2cpp initialization... (Press Enter when ready)\n";
    while (!(GetAsyncKeyState(VK_RETURN) & 0x8000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    PrintSection("Starting dump", "\x1b[96m");

    // Load configurations and auto-detect
    ConfigurationManager::LoadConfiguration();
    const GameConfiguration* config = ConfigurationManager::AutoDetectConfiguration();
    if (!config) {
        std::cerr << "FATAL ERROR: Failed to detect game configuration." << std::endl;
        UnloadDll(module);
        return 1;
    }

    // Build output file name
    std::string outputFile = config->OutputFilePrefix + ".cs";

    // Create components
    auto engine = EclipseKit::Core::Factory::EngineFactory::Create(config);
    if (!engine) {
        std::cerr << "Unsupported engine id: " << config->EngineId << "\n";
        UnloadDll(module);
        return 1;
    }
    auto formatter = std::make_unique<CSharpFormatter>();
    auto progress = std::make_unique<ConsoleProgressReporter>();

    DumpController controller(config, std::move(engine), std::move(formatter), std::move(progress));
    if (!controller.Run(outputFile)) {
        std::cerr << "Dump failed." << std::endl;
        UnloadDll(module);
        return 1;
    }

    PrintSection("Dump complete", "\x1b[92m");
    std::cout << "Output: " << outputFile << "\n";

    // Also generate script.json for IDA naming support if enabled by configuration
    if (config->GenerateScriptJson) {
        try {
            PrintSection("Generating script.json", "\x1b[95m");
            std::ofstream jsonFile("script.json", std::ios::out | std::ios::trunc);
            if (jsonFile.is_open()) {
                jsonFile << ScriptJsonGenerator::GenerateJson();
                jsonFile.close();
                std::cout << "JSON metadata saved to script.json\n";
            } else {
                std::cerr << "Failed to create script.json\n";
            }
        } catch (...) {
            std::cerr << "Exception while generating script.json\n";
        }
    }

    PrintSection("All tasks finished", "\x1b[92m");

    UnloadDll(module);
    return 0;
}

BOOL WINAPI DllMain(const HMODULE instance, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(instance);
        CreateThread(nullptr, 0, MainThread, instance, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
