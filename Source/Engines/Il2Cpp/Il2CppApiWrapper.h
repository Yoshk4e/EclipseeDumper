#pragma once

#include <cstdint>
#include <string>
#include <functional>
#include "../../../Il2cpp-Data/il2cpp-class.h"
#include "../../../Il2cpp-Data/il2cpp-metadata.h"
#include "../../../Il2cpp-Data/il2cpp-tabledefs.h"
#include "../../../Il2cpp-Data/il2cpp-api-types.h"
#include "../../Core/Configuration/EngineConfiguration.h"

namespace EclipseKit::Engines::Il2Cpp {

    /// Modern wrapper for Il2Cpp API calls with proper naming conventions
    class Il2CppApiWrapper {
    public:
        /// Initialize the API wrapper with configuration
        static bool Initialize(const Core::Configuration::GameConfiguration* config);

        /// Cleanup resources
        static void Shutdown();

        // Type System API
        static Il2CppClass* GetTypeInfoFromIndex(TypeDefinitionIndex index);
        static const char* GetClassName(Il2CppClass* classPtr);
        static const char* GetClassNamespace(Il2CppClass* classPtr);
        static bool IsClassEnum(const Il2CppClass* classPtr);
        static bool IsClassValueType(const Il2CppClass* classPtr);
        static int GetClassFlags(const Il2CppClass* classPtr);
        static Il2CppClass* GetClassParent(Il2CppClass* classPtr);
        static const Il2CppType* GetClassType(Il2CppClass* classPtr);
        static Il2CppClass* GetClassFromName(const Il2CppImage* image, const char* namespaceName, const char* className);
        static bool InitializeClass(Il2CppClass* classPtr);

        // Method API
        static const uint8_t* GetClassMethods(Il2CppClass* classPtr, void* iterator);
        static const char* GetMethodName(const uint8_t* methodPtr);
        static const Il2CppType* GetMethodReturnType(const uint8_t* methodPtr);
        static uint32_t GetMethodParameterCount(const uint8_t* methodPtr);
        static const Il2CppType* GetMethodParameter(const uint8_t* methodPtr, uint32_t index);
        static const char* GetMethodParameterName(const uint8_t* methodPtr, uint32_t index);
        static const uintptr_t* GetMethodFromName(Il2CppClass* classPtr, const char* methodName, int argumentCount);
        static std::string GetMethodModifiers(uint32_t flags);

        // Field API
        static uintptr_t* GetClassFields(Il2CppClass* classPtr, void** iterator);
        static int GetFieldFlags(uintptr_t* fieldPtr);
        static const Il2CppType* GetFieldType(uintptr_t* fieldPtr);
        static const char* GetFieldName(uintptr_t* fieldPtr);
        static void GetStaticFieldValue(uintptr_t* fieldPtr, void* value);
        static size_t GetStaticFieldValue(Il2CppField* fieldPtr);
        static bool IsFieldInstance(uintptr_t* fieldPtr);
        static size_t GetFieldOffset(uintptr_t* fieldPtr);

        // Generic Type API
        static Il2CppClass* GetClassInterfaces(Il2CppClass* classPtr, void** iterator);
        static const uint8_t* GetGenericClassContainer(const uint8_t* genericClass);
        static const uint8_t* GetClassGenericClass(const uint8_t* classPtr);
        static uint64_t GetGenericArgumentCount(const uint8_t* classPtr);
        static const uint8_t* GetGenericArgumentType(const uint8_t* classPtr, uint64_t index);
        static bool IsClassGeneric(const uint8_t* classPtr);

        // Type and Reflection API
        static void* GetTypeName(const Il2CppType* type);
        static Il2CppClass* GetClassFromType(const Il2CppType* type);

        // Runtime API
        static const uintptr_t* InitializeRuntimeClass();
        static Il2CppDomain* GetDomain();
        static Il2CppObject* InvokeMethod(const uintptr_t* methodPtr, void* objectPtr, void** parameters);
        static const Il2CppAssembly* OpenDomainAssembly(const char* assemblyName);
        static const Il2CppImage* GetAssemblyImage(const Il2CppAssembly* assembly);
        static Il2CppClass* GetObjectClass(Il2CppObject* objectPtr);
        static size_t GetImageClassCount(const Il2CppImage* image);
        static Il2CppThread* AttachThread(Il2CppDomain* domain);
        static Il2CppArray* CreateArray(Il2CppClass* elementType, il2cpp_array_size_t length);
        static Il2CppObject* CreateObject(Il2CppClass* classPtr);
        static void DisableGarbageCollection();
        static const Il2CppClass* GetImageType(const Il2CppImage* image, size_t index);

    private:
        static const Core::Configuration::GameConfiguration* currentConfig_;
        static HMODULE gameAssemblyModule_;

        /// Template for remote function calls with proper type safety
        template <typename ReturnType, typename... Args>
        static auto CreateRemoteFunction(uintptr_t offset) {
            using FunctionType = ReturnType(*)(Args...);
            return reinterpret_cast<FunctionType>(
                offset + reinterpret_cast<uintptr_t>(gameAssemblyModule_)
            );
        }
    };
}
