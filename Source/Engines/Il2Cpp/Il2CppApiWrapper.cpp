#include "pch.h"
#include "Il2CppApiWrapper.h"
#include <libloaderapi.h>
#include <sstream>

namespace EclipseKit::Engines::Il2Cpp {

    const Core::Configuration::GameConfiguration* Il2CppApiWrapper::currentConfig_ = nullptr;
    HMODULE Il2CppApiWrapper::gameAssemblyModule_ = nullptr;

    bool Il2CppApiWrapper::Initialize(const Core::Configuration::GameConfiguration* config) {
        if (!config) {
            return false;
        }

        currentConfig_ = config;

        // Find the first loaded module from the configured list
        gameAssemblyModule_ = nullptr;
        for (const auto& name : config->ModuleNames) {
            if (HMODULE mod = GetModuleHandleA(name.c_str())) {
                gameAssemblyModule_ = mod;
                break;
            }
        }
        
        return gameAssemblyModule_ != nullptr;
    }

    void Il2CppApiWrapper::Shutdown() {
        currentConfig_ = nullptr;
        gameAssemblyModule_ = nullptr;
    }

    // Type System API Implementation
    Il2CppClass* Il2CppApiWrapper::GetTypeInfoFromIndex(TypeDefinitionIndex index) {
        auto function = CreateRemoteFunction<Il2CppClass*, TypeDefinitionIndex>(
            currentConfig_->Offsets.GetTypeInfoFromTypeDefinitionIndex
        );
        return function(index);
    }

    const char* Il2CppApiWrapper::GetClassName(Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<const char*, Il2CppClass*>(
            currentConfig_->Offsets.ClassGetName
        );
        return function(classPtr);
    }

    const char* Il2CppApiWrapper::GetClassNamespace(Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<const char*, Il2CppClass*>(
            currentConfig_->Offsets.ClassGetNamespace
        );
        return function(classPtr);
    }

    bool Il2CppApiWrapper::IsClassEnum(const Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<bool, const Il2CppClass*>(
            currentConfig_->Offsets.ClassIsEnum
        );
        return function(classPtr) != 0;
    }

    bool Il2CppApiWrapper::IsClassValueType(const Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<bool, const Il2CppClass*>(
            currentConfig_->Offsets.ClassIsValueType
        );
        return function(classPtr);
    }

    int Il2CppApiWrapper::GetClassFlags(const Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<int, const Il2CppClass*>(
            currentConfig_->Offsets.ClassGetFlags
        );
        return function(classPtr);
    }

    Il2CppClass* Il2CppApiWrapper::GetClassParent(Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<Il2CppClass*, Il2CppClass*>(
            currentConfig_->Offsets.ClassGetParent
        );
        return function(classPtr);
    }

    const Il2CppType* Il2CppApiWrapper::GetClassType(Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<const Il2CppType*, Il2CppClass*>(
            currentConfig_->Offsets.ClassGetType
        );
        return function(classPtr);
    }

    Il2CppClass* Il2CppApiWrapper::GetClassFromName(const Il2CppImage* image, const char* namespaceName, const char* className) {
        auto function = CreateRemoteFunction<Il2CppClass*, const Il2CppImage*, const char*, const char*>(
            currentConfig_->Offsets.ClassFromName
        );
        return function(image, namespaceName, className);
    }

    bool Il2CppApiWrapper::InitializeClass(Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<bool, Il2CppClass*>(
            currentConfig_->Offsets.ClassInit
        );
        return function(classPtr);
    }

    // Method API Implementation
    const uint8_t* Il2CppApiWrapper::GetClassMethods(Il2CppClass* classPtr, void* iterator) {
        auto function = CreateRemoteFunction<const uint8_t*, Il2CppClass*, void*>(
            currentConfig_->Offsets.ClassGetMethods
        );
        return function(classPtr, iterator);
    }

    const char* Il2CppApiWrapper::GetMethodName(const uint8_t* methodPtr) {
        auto function = CreateRemoteFunction<const char*, const uint8_t*>(
            currentConfig_->Offsets.MethodGetName
        );
        return function(methodPtr);
    }

    const Il2CppType* Il2CppApiWrapper::GetMethodReturnType(const uint8_t* methodPtr) {
        auto function = CreateRemoteFunction<const Il2CppType*, const uint8_t*>(
            currentConfig_->Offsets.MethodGetReturnType
        );
        return function(methodPtr);
    }

    uint32_t Il2CppApiWrapper::GetMethodParameterCount(const uint8_t* methodPtr) {
        auto function = CreateRemoteFunction<uint32_t, const uint8_t*>(
            currentConfig_->Offsets.MethodGetParamCount
        );
        return function(methodPtr);
    }

    const Il2CppType* Il2CppApiWrapper::GetMethodParameter(const uint8_t* methodPtr, uint32_t index) {
        auto function = CreateRemoteFunction<const Il2CppType*, const uint8_t*, uint32_t>(
            currentConfig_->Offsets.MethodGetParam
        );
        return function(methodPtr, index);
    }

    const char* Il2CppApiWrapper::GetMethodParameterName(const uint8_t* methodPtr, uint32_t index) {
        auto function = CreateRemoteFunction<const char*, const uint8_t*, uint32_t>(
            currentConfig_->Offsets.MethodGetParamName
        );
        return function(methodPtr, index);
    }

    const uintptr_t* Il2CppApiWrapper::GetMethodFromName(Il2CppClass* classPtr, const char* methodName, int argumentCount) {
        // Note: This offset might be 0 if unknown in the configuration
        if (currentConfig_->Offsets.ClassGetMethodFromName == 0) {
            return nullptr;
        }

        auto function = CreateRemoteFunction<uintptr_t*, Il2CppClass*, const char*, int>(
            currentConfig_->Offsets.ClassGetMethodFromName
        );
        return function(classPtr, methodName, argumentCount);
    }

    std::string Il2CppApiWrapper::GetMethodModifiers(uint32_t flags) {
        std::stringstream modifiers;
        auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;

        switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE: 
            modifiers << "private "; 
            break;
        case METHOD_ATTRIBUTE_PUBLIC: 
            modifiers << "public "; 
            break;
        case METHOD_ATTRIBUTE_FAMILY: 
            modifiers << "protected "; 
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM: 
            modifiers << "internal "; 
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM: 
            modifiers << "protected internal "; 
            break;
        }

        if (flags & METHOD_ATTRIBUTE_STATIC) {
            modifiers << "static ";
        }
        
        if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
            modifiers << "abstract ";
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
                modifiers << "override ";
            }
        }
        else if (flags & METHOD_ATTRIBUTE_FINAL) {
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
                modifiers << "sealed override ";
            }
        }
        else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
                modifiers << "virtual ";
            }
            else {
                modifiers << "override ";
            }
        }
        
        if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
            modifiers << "extern ";
        }

        return modifiers.str();
    }

    // Field API Implementation
    uintptr_t* Il2CppApiWrapper::GetClassFields(Il2CppClass* classPtr, void** iterator) {
        auto function = CreateRemoteFunction<uintptr_t*, Il2CppClass*, void**>(
            currentConfig_->Offsets.ClassGetFields
        );
        return function(classPtr, iterator);
    }

    int Il2CppApiWrapper::GetFieldFlags(uintptr_t* fieldPtr) {
        auto function = CreateRemoteFunction<int, uintptr_t*>(
            currentConfig_->Offsets.FieldGetFlags
        );
        return function(fieldPtr);
    }

    const Il2CppType* Il2CppApiWrapper::GetFieldType(uintptr_t* fieldPtr) {
        auto function = CreateRemoteFunction<const Il2CppType*, uintptr_t*>(
            currentConfig_->Offsets.FieldGetType
        );
        return function(fieldPtr);
    }

    const char* Il2CppApiWrapper::GetFieldName(uintptr_t* fieldPtr) {
        auto function = CreateRemoteFunction<const char*, uintptr_t*>(
            currentConfig_->Offsets.FieldGetName
        );
        return function(fieldPtr);
    }

    void Il2CppApiWrapper::GetStaticFieldValue(uintptr_t* fieldPtr, void* value) {
        auto function = CreateRemoteFunction<void, uintptr_t*, void*>(
            currentConfig_->Offsets.FieldStaticGetValue
        );
        function(fieldPtr, value);
    }

    size_t Il2CppApiWrapper::GetStaticFieldValue(Il2CppField* fieldPtr) {
        size_t output = 0;
        GetStaticFieldValue(reinterpret_cast<uintptr_t*>(fieldPtr), &output);
        return output;
    }

    bool Il2CppApiWrapper::IsFieldInstance(uintptr_t* fieldPtr) {
        int flags = GetFieldFlags(fieldPtr);
        return (flags & FIELD_ATTRIBUTE_STATIC) == 0;
    }

    size_t Il2CppApiWrapper::GetFieldOffset(uintptr_t* fieldPtr) {
        auto function = CreateRemoteFunction<size_t, uintptr_t*>(
            currentConfig_->Offsets.FieldGetOffset
        );
        return function(fieldPtr);
    }

    // Generic Type API Implementation
    Il2CppClass* Il2CppApiWrapper::GetClassInterfaces(Il2CppClass* classPtr, void** iterator) {
        auto function = CreateRemoteFunction<Il2CppClass*, Il2CppClass*, void**>(
            currentConfig_->Offsets.ClassGetInterfaces
        );
        return function(classPtr, iterator);
    }

    const uint8_t* Il2CppApiWrapper::GetGenericClassContainer(const uint8_t* genericClass) {
        auto raw = *reinterpret_cast<const uint64_t*>(genericClass + 8);
        return reinterpret_cast<const uint8_t*>(raw);
    }

    const uint8_t* Il2CppApiWrapper::GetClassGenericClass(const uint8_t* classPtr) {
        auto raw = *reinterpret_cast<const uint64_t*>(classPtr + 96);
        return reinterpret_cast<const uint8_t*>(raw);
    }

    uint64_t Il2CppApiWrapper::GetGenericArgumentCount(const uint8_t* classPtr) {
        const uint8_t* genericClass = GetClassGenericClass(classPtr);
        if (genericClass) {
            const uint8_t* genericContainer = GetGenericClassContainer(genericClass);
            if (genericContainer) {
                return *reinterpret_cast<const uint32_t*>(genericContainer);
            }
        }
        return 0;
    }

    const uint8_t* Il2CppApiWrapper::GetGenericArgumentType(const uint8_t* classPtr, uint64_t index) {
        const uint8_t* genericClass = GetClassGenericClass(classPtr);
        if (!genericClass) return nullptr;

        const uint8_t* genericContainer = GetGenericClassContainer(genericClass);
        if (!genericContainer) return nullptr;

        uint64_t argumentsPtr = *reinterpret_cast<const uint64_t*>(genericContainer + 8);
        if (!argumentsPtr) return nullptr;

        const uint64_t* arguments = reinterpret_cast<const uint64_t*>(argumentsPtr);
        return reinterpret_cast<const uint8_t*>(arguments[index]);
    }

    bool Il2CppApiWrapper::IsClassGeneric(const uint8_t* classPtr) {
        const uint8_t* genericClass = GetClassGenericClass(classPtr);
        if (!genericClass) return false;
        return GetGenericClassContainer(genericClass) != nullptr;
    }

    // Type and Reflection API Implementation
    void* Il2CppApiWrapper::GetTypeName(const Il2CppType* type) {
        auto function = CreateRemoteFunction<void*, const Il2CppType*>(
            currentConfig_->Offsets.TypeGetName
        );
        return function(type);
    }

    Il2CppClass* Il2CppApiWrapper::GetClassFromType(const Il2CppType* type) {
        auto function = CreateRemoteFunction<Il2CppClass*, const Il2CppType*>(
            currentConfig_->Offsets.FromIl2CppType
        );
        return function(type);
    }

    // Runtime API Implementation
    const uintptr_t* Il2CppApiWrapper::InitializeRuntimeClass() {
        auto function = CreateRemoteFunction<const uintptr_t*>(
            currentConfig_->Offsets.RuntimeClassInit
        );
        return function();
    }

    Il2CppDomain* Il2CppApiWrapper::GetDomain() {
        auto function = CreateRemoteFunction<Il2CppDomain*>(
            currentConfig_->Offsets.DomainGet
        );
        return function();
    }

    Il2CppObject* Il2CppApiWrapper::InvokeMethod(const uintptr_t* methodPtr, void* objectPtr, void** parameters) {
        Il2CppException* exception = nullptr;
        auto function = CreateRemoteFunction<Il2CppObject*, const uintptr_t*, void*, void**, Il2CppException**>(
            currentConfig_->Offsets.RuntimeInvoke
        );
        return function(methodPtr, objectPtr, parameters, &exception);
    }

    const Il2CppAssembly* Il2CppApiWrapper::OpenDomainAssembly(const char* assemblyName) {
        auto function = CreateRemoteFunction<const Il2CppAssembly*, Il2CppDomain*, const char*>(
            currentConfig_->Offsets.DomainAssemblyOpen
        );
        Il2CppDomain* domain = GetDomain();
        return function(domain, assemblyName);
    }

    const Il2CppImage* Il2CppApiWrapper::GetAssemblyImage(const Il2CppAssembly* assembly) {
        auto function = CreateRemoteFunction<const Il2CppImage*, const Il2CppAssembly*>(
            currentConfig_->Offsets.AssemblyGetImage
        );
        return function(assembly);
    }

    Il2CppClass* Il2CppApiWrapper::GetObjectClass(Il2CppObject* objectPtr) {
        return objectPtr->klass;
    }

    size_t Il2CppApiWrapper::GetImageClassCount(const Il2CppImage* image) {
        return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const char*>(image) + 24);
    }

    Il2CppThread* Il2CppApiWrapper::AttachThread(Il2CppDomain* domain) {
        auto function = CreateRemoteFunction<Il2CppThread*, Il2CppDomain*>(
            currentConfig_->Offsets.ThreadAttach
        );
        return function(domain);
    }

    Il2CppArray* Il2CppApiWrapper::CreateArray(Il2CppClass* elementType, il2cpp_array_size_t length) {
        auto function = CreateRemoteFunction<Il2CppArray*, Il2CppClass*, il2cpp_array_size_t>(
            currentConfig_->Offsets.VmArrayNew
        );
        return function(elementType, length);
    }

    Il2CppObject* Il2CppApiWrapper::CreateObject(Il2CppClass* classPtr) {
        auto function = CreateRemoteFunction<Il2CppObject*, Il2CppClass*>(
            currentConfig_->Offsets.ObjectNew
        );
        return function(classPtr);
    }

    void Il2CppApiWrapper::DisableGarbageCollection() {
        auto function = CreateRemoteFunction<void>(
            currentConfig_->Offsets.GcDisable
        );
        function();
    }

    const Il2CppClass* Il2CppApiWrapper::GetImageType(const Il2CppImage* image, size_t index) {
        uintptr_t imagePtr = reinterpret_cast<uintptr_t>(image);
        int32_t offsetValue = *reinterpret_cast<const int32_t*>(imagePtr + 0x0C);
        TypeDefinitionIndex typeIndex = static_cast<int32_t>(index) + offsetValue + 0xC417DD23;
        return GetTypeInfoFromIndex(typeIndex);
    }
}
