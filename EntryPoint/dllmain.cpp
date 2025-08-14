// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <functional>
#include "../Core/il2cpp/Api/Il2CppApi.h"
#include "../Core/il2cpp/Metadata/Il2CppMetadata.h"
#include "../Utils/ScriptJson.h"
#include "../Utils/ProgressReporter.h"
#include <chrono>
#include <iomanip>
#include <fstream>
#define DEBUG_VER
//#define RELEASE_VER


BOOL WINAPI DllMain(const HMODULE instance, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(instance);
        CreateThread(nullptr, 0, MainThread, instance, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:

        break;
    }


    return true;

}

DWORD WINAPI MainThread(LPVOID lpReserved) {
    Il2CppApi::Initialize();
    auto gIBaseAddress = Il2CppApi::GetImageBase();

    // Setup console for UTF-8 output
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
    std::cout << "Waiting for il2cpp initialization... (Press Enter when ready)\n";
    while (!(GetAsyncKeyState(VK_RETURN) & 0x8000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << "\n=== Starting Dump Process ===\n";
    constexpr int totalTypes = 56983;
    std::cout << "Scanning 0-" << (totalTypes - 1) << " TypeDefinition indices...\n\n";

    std::ofstream outFile("OSREL5.6Dump.cs");
    if (!outFile.is_open()) {
        std::cerr << "FATAL ERROR: Failed to create output file!\n";
        return 1;
    }

    // Instantiate progress reporter
    ProgressReporter reporter(totalTypes, 50);

    int validClasses = 0;
    int totalMethods = 0;
    int totalFields = 0;
    int nullClasses = 0;
    int errorClasses = 0;

    Il2CppApi::RuntimeClassInit();
    for (int index = 0; index < totalTypes; ++index) {
        // Ensure class init each iteration
        Il2CppApi::RuntimeClassInit();

        // Dump logic
        Il2CppClass* klass = Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(index);
        if (!klass) {
            nullClasses++;
        }
        else {
            try {
                std::string classDump = Il2CppMetadata::DumpClass(klass);
                totalMethods += std::count(classDump.begin(), classDump.end(), '\n');
                totalFields += std::count(classDump.begin(), classDump.end(), ';');
                validClasses++;
                outFile << "// TypeDefIndex: " << index << "\n" << classDump << "\n\n";
            }
            catch (const std::exception& e) {
                errorClasses++;
            }
        }

        // Update progress bar
        reporter.update(index);
    }
    reporter.finish();

    // Final report
    std::cout << "\n=== Dump Complete ===\n"
        << "Processed " << totalTypes << " type definitions\n"
        << "Valid: " << validClasses << " | Null: " << nullClasses
        << " | Errors: " << errorClasses << "\n"
        << "Methods: " << totalMethods << " | Fields: " << totalFields << "\n"
        << "Output: OSREL5.6Dump.cs\n";

    outFile.close();
    std::cout << "\n=== Generating Script JSON ===\n";
    std::ofstream jsonFile("script.json");
    if (jsonFile.is_open()) {
        jsonFile << ScriptJsonGenerator::GenerateJson();
        jsonFile.close();
        std::cout << "JSON metadata saved to script.json\n";
    }
    else {
        std::cerr << "Failed to create script.json\n";
    }

	std::cout << "\nDumping completed\n";
    return 0;
}

