#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>

namespace EclipseKit::Core::Configuration {

    /// Configuration structure for different engine versions and games
    struct EngineOffsets {
        // Type system offsets
        uintptr_t GetTypeInfoFromTypeDefinitionIndex;
        
        // Class-related offsets
        uintptr_t ClassGetName;
        uintptr_t ClassGetNamespace;
        uintptr_t ClassGetMethods;
        uintptr_t ClassGetFields;
        uintptr_t ClassGetFlags;
        uintptr_t ClassGetType;
        uintptr_t ClassGetParent;
        uintptr_t ClassIsEnum;
        uintptr_t ClassGetInterfaces;
        uintptr_t ClassFromName;
        uintptr_t ClassGetMethodFromName; // may be 0 if unknown
        uintptr_t ClassInit;
        uintptr_t FromIl2CppType;
        
        // Method-related offsets
        uintptr_t MethodGetName;
        uintptr_t MethodGetReturnType;
        uintptr_t MethodGetParamCount;
        uintptr_t MethodGetParam;
        uintptr_t MethodGetParamName;
        // Offsets inside Il2CppMethod struct (byte offsets)
        uintptr_t MethodGetFlags;  // byte offset to flags within method struct
        uintptr_t MethodPointer;   // byte offset to function pointer within method struct
        
        // Field-related offsets
        uintptr_t FieldGetName;
        uintptr_t FieldGetType;
        uintptr_t FieldGetFlags;
        uintptr_t FieldGetOffset;
        uintptr_t FieldStaticGetValue;
        
        // Type-related offsets
        uintptr_t TypeGetName;
        uintptr_t ClassIsValueType;
        
        // Runtime offsets
        uintptr_t RuntimeClassInit;
        uintptr_t VmClassInit;
        uintptr_t ThreadAttach;
        uintptr_t VmArrayNew;
        uintptr_t ObjectNew;
        uintptr_t RuntimeInvoke;
        
        // Domain offsets
        uintptr_t DomainGet;
        uintptr_t DomainGetAssemblies;
        uintptr_t DomainAssemblyOpen;
        uintptr_t AssemblyGetImage;
        
        // GC offsets
        uintptr_t GcDisable;
    };

    /// Configuration for a specific game/engine version
    struct GameConfiguration {
        std::string GameName;         // Human-friendly name
        std::string Version;          // Version tag
        std::string EngineId;         // e.g., "il2cpp"
        std::vector<std::string> ModuleNames; // One or more module names to detect (e.g., GameAssembly.dll)
        std::vector<std::string> ProcessNames; // Optional exe names to disambiguate detection
        EngineOffsets Offsets;
        
        // Additional configuration options
        bool RequiresInitialization = true;
        uint64_t MaxTypeProbeCount = 2000000;
        std::string OutputFilePrefix;
        bool GenerateScriptJson = true; // whether to produce script.json alongside output
        bool UseFullNames = false;      // whether to use full type names (e.g., Namespace.Class) instead of short names
        bool UseMainModule = false;     // if true, treat main executable module as IL2CPP image (static-linked)
    };

    /// Configuration manager for different games and versions
    class ConfigurationManager {
    public:
        /// Load configuration from file or use built-in configurations
        static bool LoadConfiguration(const std::string& configPath = "");
        
        /// Get configuration for a specific game version
        static const GameConfiguration* GetConfiguration(const std::string& gameName, const std::string& version);
        
        /// Auto-detect configuration based on current process
        static const GameConfiguration* AutoDetectConfiguration();
        
        /// Register a new configuration
        static void RegisterConfiguration(const GameConfiguration& config);
        
    private:
        static std::unordered_map<std::string, GameConfiguration> configurations_;
        static std::vector<std::string> detectionOrder_;
        static const GameConfiguration* selected_;
        static void InitializeBuiltInConfigurations();
    };
}
