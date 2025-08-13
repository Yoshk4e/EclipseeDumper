#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <map>
#include "../Il2cpp-Data/il2cpp-class.h"
#include "../Core/il2cpp/Offsets/Il2CppOffsets.h"
#include "../Il2cpp-Data/il2cpp-metadata.h"
#include "../Il2cpp-Data/il2cpp-tabledefs.h"
#include "../Il2cpp-Data/il2cpp-api-types.h"


class Il2CppApi {
public:
    static void Initialize();
    static uintptr_t GetImageBase();

    // Type system API
    static Il2CppClass* GetTypeInfoFromTypeDefinitionIndex(TypeDefinitionIndex index);
    static const char* ClassGetNamespace(Il2CppClass* klass);
    static const char* ClassGetName(Il2CppClass* klass);
    static bool ClassIsEnum(const Il2CppClass* klass);
    static bool ClassIsValueType(const Il2CppClass* klass);
    static int ClassGetFlags(const Il2CppClass* klass);
    static Il2CppClass* ClassGetParent(Il2CppClass* klass);
    static const Il2CppType* ClassGetType(Il2CppClass* klass);
    static Il2CppClass* ClassFromName(const Il2CppImage* image, const char* namespaze, const char* name);
    static bool ClassInit(Il2CppClass* klass);

    // Method API
    static uintptr_t* ClassGetMethods(Il2CppClass* klass, void* iter);
    static const char* MethodGetName(const uintptr_t* MethodPtr);
    static const Il2CppType* MethodGetReturnType(const uintptr_t* method);
    static uintptr_t* MethodGetParamCount(const uintptr_t* method);
    static const Il2CppType* MethodGetParam(const uintptr_t* method, uint32_t index);
    static const char* MethodGetParamName(const uintptr_t* method, uint32_t index);
    static const uintptr_t* ClassGetMethodFromName(Il2CppClass* klass, const char* name, int argsCount);
    static std::string GetMethodModifier(uint32_t flags);

    // Field API
    static uintptr_t* ClassGetFields(Il2CppClass* klass, void** iter);
    static int FieldGetFlags(uintptr_t* field);
    static const Il2CppType* FieldGetType(uintptr_t* field);
    static const char* FieldGetName(uintptr_t* field);
    static void FieldStaticGetValue(uintptr_t* field, void* value);
    static size_t FieldGetOffset(uintptr_t* field);

    // Generic type API
    static const size_t* ClassGetInterfaces(const uint8_t* klass, size_t& count);
    static const uint8_t* GenericClassGetGenericContainer(const uint8_t* generic_class);
    static const uint8_t* ClassGetGenericClass(const uint8_t* klass);
    static uint64_t ClassGetGenericArgCount(const uint8_t* klass);
    static const uint8_t* ClassGetGenericArgType(const uint8_t* klass, uint64_t index);
    static bool ClassIsGeneric(const uint8_t* klass);

    // Type and reflection API
    static char* TypeGetName(const Il2CppType* type);
    static Il2CppClass* FromIl2CppType(const Il2CppType* type);

    // Runtime API
    static const uintptr_t* RuntimeClassInit();
    static Il2CppDomain* DomainGet();
    static const Il2CppAssembly* DomainAssemblyOpen(Il2CppDomain* domain, const char* name);

private:
    static uintptr_t gIBaseAddress;

    template <typename ReturnType, typename... Args>
    static auto RemoteFuncCall(uintptr_t offset) {
        using FuncType = ReturnType(*)(Args...);
        return reinterpret_cast<FuncType>(offset);
    }
};
