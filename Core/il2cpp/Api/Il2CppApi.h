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
    static const uint8_t* ClassGetMethods(Il2CppClass* klass, void* iter);
    static const char* MethodGetName(const uint8_t* MethodPtr);
    static const Il2CppType* MethodGetReturnType(const uint8_t* method);
    static const uint32_t MethodGetParamCount(const uint8_t* method);
    static const Il2CppType* MethodGetParam(const uint8_t* method, uint32_t index);
    static const char* MethodGetParamName(const uint8_t* method, uint32_t index);
    static const uintptr_t* ClassGetMethodFromName(Il2CppClass* klass, const char* name, int argsCount);
    static std::string GetMethodModifier(uint32_t flags);

    // Field API
    static uintptr_t* ClassGetFields(Il2CppClass* klass, void** iter);
    static int FieldGetFlags(uintptr_t* field);
    static const Il2CppType* FieldGetType(uintptr_t* field);
    static const char* FieldGetName(uintptr_t* field);
    static void FieldStaticGetValue(uintptr_t* field, void* value);
    static size_t FieldStaticGetValue(Il2CppField* field);
    static bool FieldIsInstance(uintptr_t* field);
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
    static Il2CppObject* MethodInvoke(const uintptr_t* method, void* obj, void** params);
    static const Il2CppAssembly* DomainAssemblyOpen(const char* name);
    static const Il2CppImage* AssemblyGetImage(const Il2CppAssembly* assembly);
    static Il2CppClass* ObjectGetClass(Il2CppObject* obj);
    static size_t ImageGetClassCount(const Il2CppImage* image);
    static Il2CppThread* ThreadAttach(Il2CppDomain* domain);
    static Il2CppArray* ArrayNew(Il2CppClass* elementTypeInfo, il2cpp_array_size_t length);
    static Il2CppObject* ObjectNew(Il2CppClass* klass);
    static void GCDisable();
    static const Il2CppClass* ImageGetType(const Il2CppImage* image, size_t index);


private:
    template <typename ReturnType, typename... Args>
    static auto RemoteFuncCall(uintptr_t offset) {
        using FuncType = ReturnType(*)(Args...);
        return reinterpret_cast<FuncType>(offset + il2CppOffsets::gIBaseAddress);
    }
};