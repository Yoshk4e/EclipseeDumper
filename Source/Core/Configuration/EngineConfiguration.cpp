#include "pch.h"
extern "C" IMAGE_DOS_HEADER __ImageBase;
#include "EngineConfiguration.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>

namespace EclipseKit::Core::Configuration {

    std::unordered_map<std::string, GameConfiguration> ConfigurationManager::configurations_;
    std::vector<std::string> ConfigurationManager::detectionOrder_;
    const GameConfiguration* ConfigurationManager::selected_ = nullptr;

    bool ConfigurationManager::LoadConfiguration(const std::string& configPath) {
        InitializeBuiltInConfigurations();
        
        // Determine configs directory
        std::filesystem::path baseDir;
        if (!configPath.empty()) {
            baseDir = std::filesystem::path(configPath);
        } else {
            // Resolve the directory of our DLL and use a sibling Configs folder
            char modulePath[MAX_PATH] = {0};
            if (GetModuleFileNameA(reinterpret_cast<HMODULE>(&__ImageBase), modulePath, MAX_PATH)) {
                baseDir = std::filesystem::path(modulePath).parent_path() / "Configs";
            }
        }

        if (!baseDir.empty() && std::filesystem::exists(baseDir) && std::filesystem::is_directory(baseDir)) {
            for (auto& entry : std::filesystem::directory_iterator(baseDir)) {
                if (!entry.is_regular_file()) continue;
                auto path = entry.path();
                if (path.extension() != ".json") continue;

                try {
                    std::ifstream in(path, std::ios::in);
                    if (!in.is_open()) continue;
                    std::string text{ std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
                    in.close();

                    // Very lightweight parsing using regexes; expects reasonably formatted JSON
                    auto getString = [&](const char* key) -> std::string {
                        std::regex rx(std::string("\\\"") + key + "\\\"\\s*:\\s*\\\"([^\\\"]*)\\\"");
                        std::smatch m; if (std::regex_search(text, m, rx)) return m[1].str();
                        return {};
                    };
                    auto getBool = [&](const char* key, bool def=false) -> bool {
                        std::regex rx(std::string("\\\"") + key + "\\\"\\s*:\\s*(true|false)");
                        std::smatch m; if (std::regex_search(text, m, rx)) return m[1].str()=="true";
                        return def;
                    };
                    auto getUint64 = [&](const char* key, uint64_t def=0) -> uint64_t {
                        std::regex rxHex(std::string("\\\"") + key + "\\\"\\s*:\\s*\\\"0x([0-9A-Fa-f]+)\\\"");
                        std::regex rxNum(std::string("\\\"") + key + "\\\"\\s*:\\s*([0-9]+)");
                        std::smatch m; 
                        if (std::regex_search(text, m, rxHex)) return std::stoull(m[1].str(), nullptr, 16);
                        if (std::regex_search(text, m, rxNum)) return std::stoull(m[1].str(), nullptr, 10);
                        return def;
                    };
                    auto getArrayStrings = [&](const char* key) -> std::vector<std::string> {
                        std::vector<std::string> out; 
                        std::regex rx(std::string("\\\"") + key + "\\\"\\s*:\\s*\\[(.*?)\\]");
                        std::smatch m; if (std::regex_search(text, m, rx)) {
                            std::string inner = m[1].str();
                            std::regex qx("\\\"([^\\\"]*)\\\"");
                            auto it = std::sregex_iterator(inner.begin(), inner.end(), qx);
                            for (; it != std::sregex_iterator(); ++it) out.push_back((*it)[1].str());
                        }
                        return out;
                    };

                    GameConfiguration cfg;
                    cfg.GameName = getString("gameName");
                    cfg.Version = getString("version");
                    cfg.EngineId = getString("engineId");
                    cfg.ModuleNames = getArrayStrings("moduleNames");
                    cfg.OutputFilePrefix = getString("outputFilePrefix");
                    cfg.GenerateScriptJson = getBool("generateScriptJson", true);
                    cfg.UseFullNames = getBool("useFullNames", false);
                    cfg.UseMainModule = getBool("useMainModule", false);
                    cfg.MaxTypeProbeCount = getUint64("maxTypeProbeCount", 2000000);

                    auto& o = cfg.Offsets;
                    o.GetTypeInfoFromTypeDefinitionIndex = getUint64("GetTypeInfoFromTypeDefinitionIndex");
                    o.ClassGetName = getUint64("ClassGetName");
                    o.ClassGetNamespace = getUint64("ClassGetNamespace");
                    o.ClassGetMethods = getUint64("ClassGetMethods");
                    o.ClassGetFields = getUint64("ClassGetFields");
                    o.ClassGetFlags = getUint64("ClassGetFlags");
                    o.ClassGetType = getUint64("ClassGetType");
                    o.ClassGetParent = getUint64("ClassGetParent");
                    o.ClassIsEnum = getUint64("ClassIsEnum");
                    o.ClassGetInterfaces = getUint64("ClassGetInterfaces");
                    o.ClassFromName = getUint64("ClassFromName");
                    o.ClassGetMethodFromName = getUint64("ClassGetMethodFromName", 0);
                    o.ClassInit = getUint64("ClassInit");
                    o.FromIl2CppType = getUint64("FromIl2CppType");

                    o.MethodGetName = getUint64("MethodGetName");
                    o.MethodGetReturnType = getUint64("MethodGetReturnType");
                    o.MethodGetParamCount = getUint64("MethodGetParamCount");
                    o.MethodGetParam = getUint64("MethodGetParam");
                    o.MethodGetParamName = getUint64("MethodGetParamName");
                    // MethodGetFlags may be provided as quoted decimal (e.g., "54"). Handle that case explicitly.
                    o.MethodGetFlags = getUint64("MethodGetFlags");
                    if (o.MethodGetFlags == 0) {
                        std::regex rxQuotedDec("\\\"MethodGetFlags\\\"\\s*:\\s*\\\"([0-9]+)\\\"");
                        std::smatch m2; if (std::regex_search(text, m2, rxQuotedDec)) {
                            try { o.MethodGetFlags = std::stoull(m2[1].str(), nullptr, 10); } catch (...) {}
                        }
                    }
                    o.MethodPointer = getUint64("MethodPointer");

                    o.FieldGetName = getUint64("FieldGetName");
                    o.FieldGetType = getUint64("FieldGetType");
                    o.FieldGetFlags = getUint64("FieldGetFlags");
                    o.FieldGetOffset = getUint64("FieldGetOffset");
                    o.FieldStaticGetValue = getUint64("FieldStaticGetValue");

                    o.TypeGetName = getUint64("TypeGetName");
                    o.ClassIsValueType = getUint64("ClassIsValueType");

                    o.RuntimeClassInit = getUint64("RuntimeClassInit");
                    o.VmClassInit = getUint64("VmClassInit");
                    o.ThreadAttach = getUint64("ThreadAttach");
                    o.VmArrayNew = getUint64("VmArrayNew");
                    o.ObjectNew = getUint64("ObjectNew");
                    o.RuntimeInvoke = getUint64("RuntimeInvoke");

                    o.DomainGet = getUint64("DomainGet");
                    o.DomainGetAssemblies = getUint64("DomainGetAssemblies");
                    o.DomainAssemblyOpen = getUint64("DomainAssemblyOpen");
                    o.AssemblyGetImage = getUint64("AssemblyGetImage");

                    o.GcDisable = getUint64("GcDisable");

                    if (!cfg.GameName.empty() && !cfg.Version.empty() && !cfg.EngineId.empty() && !cfg.ModuleNames.empty()) {
                        RegisterConfiguration(cfg);
                        std::cout << "Loaded configuration: " << cfg.GameName << " " << cfg.Version << " from " << path.filename().string() << "\n";
                    }
                } catch (...) {
                    // Skip malformed config files gracefully
                }
            }
        }
        
        return true;
    }

    const GameConfiguration* ConfigurationManager::GetConfiguration(const std::string& gameName, const std::string& version) {
        std::string key = gameName + "_" + version;
        auto it = configurations_.find(key);
        return (it != configurations_.end()) ? &it->second : nullptr;
    }

    const GameConfiguration* ConfigurationManager::AutoDetectConfiguration() {
        // Return cached selection if already computed
        if (selected_ != nullptr) return selected_;

        // Env override: ECLIPSEKIT_CONFIG="Game_Version"
        char envBuf[256] = {0};
        DWORD got = GetEnvironmentVariableA("ECLIPSEKIT_CONFIG", envBuf, sizeof(envBuf));
        if (got > 0 && got < sizeof(envBuf)) {
            std::string key(envBuf);
            auto it = configurations_.find(key);
            if (it != configurations_.end()) {
                const auto& cfg = it->second;
                for (const auto& moduleName : cfg.ModuleNames) {
                    if (GetModuleHandleA(moduleName.c_str()) != nullptr) {
                        std::cout << "[Config] Using configuration from ECLIPSEKIT_CONFIG: " << key << "\n";
                        selected_ = &cfg;
                        return selected_;
                    }
                }
            }
        }

        // Build candidate list by module presence, prefer most recently registered
        std::vector<const GameConfiguration*> candidates;
        for (const auto& key : detectionOrder_) {
            auto it = configurations_.find(key);
            if (it == configurations_.end()) continue;
            const auto& cfg = it->second;
            for (const auto& moduleName : cfg.ModuleNames) {
                if (GetModuleHandleA(moduleName.c_str()) != nullptr) {
                    candidates.push_back(&cfg);
                    break;
                }
            }
        }
        if (candidates.empty()) return nullptr;

        // Prefer process name matches if provided
        char exePath[MAX_PATH] = {0};
        std::string exeName;
        if (GetModuleFileNameA(nullptr, exePath, MAX_PATH)) {
            exeName = std::filesystem::path(exePath).filename().string();
            std::transform(exeName.begin(), exeName.end(), exeName.begin(), ::tolower);
        }
        if (!exeName.empty()) {
            for (auto it = candidates.rbegin(); it != candidates.rend(); ++it) {
                const auto* cfg = *it;
                for (auto name : cfg->ProcessNames) {
                    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                    if (name == exeName) {
                        std::cout << "[Config] Selected by process match: " << cfg->GameName << " " << cfg->Version << "\n";
                        selected_ = cfg;
                        return selected_;
                    }
                }
            }
        }

        // Fallback to most recently registered candidate
        selected_ = candidates.back();
        std::cout << "[Config] Selected by module presence: " << selected_->GameName << " " << selected_->Version << "\n";
        return selected_;
    }

    void ConfigurationManager::RegisterConfiguration(const GameConfiguration& config) {
        std::string key = config.GameName + "_" + config.Version;
        configurations_[key] = config;
        auto it = std::find(detectionOrder_.begin(), detectionOrder_.end(), key);
        if (it != detectionOrder_.end()) detectionOrder_.erase(it);
        detectionOrder_.push_back(key);
    }

    void ConfigurationManager::InitializeBuiltInConfigurations() {
        if (!configurations_.empty()) {
            return; // Already initialized
        }

        
        GameConfiguration eclipseConfig;
        eclipseConfig.GameName = "Eclipse";
        eclipseConfig.Version = "OSREL5.6";
        eclipseConfig.EngineId = "il2cpp";
        eclipseConfig.ModuleNames = { "GameAssembly.dll" };
        eclipseConfig.OutputFilePrefix = "OSREL5.6_dump";
        
        auto& offsets = eclipseConfig.Offsets;
        offsets.GetTypeInfoFromTypeDefinitionIndex = 0x15E3300;
        
        // Class offsets
        offsets.TypeGetName = 0x15496D0;
        offsets.ClassGetNamespace = 0x15478F0;
        offsets.ClassGetName = 0x15478D0;
        offsets.ClassGetMethods = 0x15478C0;
        offsets.FromIl2CppType = 0x15C5840;
        offsets.ClassFromName = 0x504970;
        offsets.ClassGetFields = 0x15C6B30;
        offsets.ClassGetFlags = 0x12F00E0;
        offsets.ClassGetType = 0x12A6710;
        offsets.ClassGetParent = 0x1547920;
        offsets.ClassIsEnum = 0x15479F0;
        offsets.ClassGetInterfaces = 0x1547910;
        
        // Method offsets
        offsets.MethodGetName = 0x1548DD0;
        offsets.MethodGetReturnType = 0x1548E40;
        offsets.MethodGetParamCount = 0x1548E10;
        offsets.MethodGetParam = 0x1548E00;
        offsets.MethodGetParamName = 0x1548E20;
        offsets.MethodPointer = 0;
        offsets.MethodGetFlags = 54;
        
        // Field offsets
        offsets.FieldGetName = 0x1547BD0;
        offsets.FieldGetType = 0x15E31F0;
        offsets.FieldGetFlags = 0x15E0F50;
        offsets.FieldGetOffset = 0x1547BF0;
        offsets.FieldStaticGetValue = 0x1547C90;
        
        // Type offsets
        offsets.ClassIsValueType = 0x499430;
        
        // Runtime offsets
        offsets.RuntimeClassInit = 0x482CC0;
        offsets.VmClassInit = 0x479930;
        offsets.ThreadAttach = 0x49F8B0;
        offsets.VmArrayNew = 0x468FC0;
        offsets.ObjectNew = 0x48FAD0;
        offsets.RuntimeInvoke = 0x492AE0;
        
        // Domain offsets
        offsets.DomainGet = 0x49C0E0;
        offsets.DomainGetAssemblies = 0x4459EA8;
        offsets.DomainAssemblyOpen = 0x412970;
        offsets.AssemblyGetImage = 0x4125A0;
        
        // GC offsets
        offsets.GcDisable = 0x1547E30;
        
        RegisterConfiguration(eclipseConfig);
    }
}
