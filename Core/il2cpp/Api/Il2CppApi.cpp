#include "pch.h"
#include "../Core/il2cpp/Offsets/Il2CppOffsets.h"
#include "Il2CppApi.h"
#include <libloaderapi.h>

// Initialize static member
uintptr_t Il2CppApi::gIBaseAddress = 0;

void Il2CppApi::Initialize() {
    gIBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA("GenshinImpact.exe"));
}

uintptr_t Il2CppApi::GetImageBase() {
    return gIBaseAddress;
}

// Type system API implementations
Il2CppClass* Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(TypeDefinitionIndex index) {
    auto func = RemoteFuncCall<Il2CppClass*, TypeDefinitionIndex>(
        il2CppOffsets::GetTypeInfoFromTypeDefinitionIndex + gIBaseAddress
    );
    return func(index);
}

const char* Il2CppApi::ClassGetNamespace(Il2CppClass* klass) {
    auto func = RemoteFuncCall<const char*, Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_namespace + gIBaseAddress
    );
    return func(klass);
}

const char* Il2CppApi::ClassGetName(Il2CppClass* klass) {
    auto func = RemoteFuncCall<const char*, Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_name + gIBaseAddress
    );
    return func(klass);
}

bool Il2CppApi::ClassIsEnum(const Il2CppClass* klass) {
    return (*reinterpret_cast<const uint8_t*>(
        reinterpret_cast<const char*>(klass) + 201) & 0x20) != 0;
}

Il2CppClass* Il2CppApi::ClassGetParent(Il2CppClass* klass) {
    return *reinterpret_cast<Il2CppClass**>(reinterpret_cast<char*>(klass) + 56);
}

const Il2CppType* Il2CppApi::ClassGetType(Il2CppClass* klass) {
    return reinterpret_cast<const Il2CppType*>(reinterpret_cast<char*>(klass) + 32);
}

Il2CppClass* Il2CppApi::ClassFromName(const Il2CppImage* image, const char* namespaze, const char* name) {
    auto func = RemoteFuncCall<Il2CppClass*, const Il2CppImage*, const char*, const char*>(
        il2CppOffsets::Class::il2cpp_class_from_name + gIBaseAddress
    );
    return func(image, namespaze, name);
}

bool Il2CppApi::ClassInit(Il2CppClass* klass) {
    auto func = RemoteFuncCall<bool, Il2CppClass*>(
        il2CppOffsets::RuntimeStuff::il2cpp_vvm_class_init + gIBaseAddress
    );
    return func(klass);
}

// Method API implementations
uintptr_t* Il2CppApi::ClassGetMethods(Il2CppClass* klass, void* iter) {
    auto func = RemoteFuncCall<uintptr_t*, Il2CppClass*, void*>(
        il2CppOffsets::Class::il2cpp_class_get_methods + gIBaseAddress
    );
    return func(klass, iter);
}

const char* Il2CppApi::MethodGetName(const uintptr_t* MethodPtr) {
    auto func = RemoteFuncCall<const char*, const uintptr_t*>(
        il2CppOffsets::Methods::il2cpp_method_get_name + gIBaseAddress
    );
    return func(MethodPtr);
}

const Il2CppType* Il2CppApi::MethodGetReturnType(const uintptr_t* method) {
    auto func = RemoteFuncCall<const Il2CppType*, const uintptr_t*>(
        il2CppOffsets::Methods::il2cpp_method_get_return_type + gIBaseAddress
    );
    return func(method);
}

uintptr_t* Il2CppApi::MethodGetParamCount(const uintptr_t* method) {
    auto func = RemoteFuncCall<uintptr_t*, const uintptr_t*>(
        il2CppOffsets::Methods::il2cpp_method_get_param_count + gIBaseAddress
    );
    return func(method);
}

const Il2CppType* Il2CppApi::MethodGetParam(const uintptr_t* method, uint32_t index) {
    auto func = RemoteFuncCall<const Il2CppType*, const uintptr_t*, uint32_t>(
        il2CppOffsets::Methods::il2cpp_method_get_param + gIBaseAddress
    );
    return func(method, index);
}

const char* Il2CppApi::MethodGetParamName(const uintptr_t* method, uint32_t index) {
    auto func = RemoteFuncCall<const char*, const uintptr_t*, uint32_t>(
        il2CppOffsets::Methods::il2cpp_method_get_param_name + gIBaseAddress
    );
    return func(method, index);
}

const uintptr_t* Il2CppApi::ClassGetMethodFromName(Il2CppClass* klass, const char* name, int argsCount) {
    auto func = RemoteFuncCall<uintptr_t*, Il2CppClass*, const char*, int>(
        il2CppOffsets::Class::il2cpp_class_get_method_from_name + gIBaseAddress
    );
    return func(klass, name, argsCount);
}

std::string Il2CppApi::GetMethodModifier(uint32_t flags) {
    std::stringstream Modifiers;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;

    switch (access) {
    case METHOD_ATTRIBUTE_PRIVATE: Modifiers << "private "; break;
    case METHOD_ATTRIBUTE_PUBLIC: Modifiers << "public "; break;
    case METHOD_ATTRIBUTE_FAMILY: Modifiers << "protected "; break;
    case METHOD_ATTRIBUTE_ASSEM:
    case METHOD_ATTRIBUTE_FAM_AND_ASSEM: Modifiers << "internal "; break;
    case METHOD_ATTRIBUTE_FAM_OR_ASSEM: Modifiers << "protected internal "; break;
    }

    if (flags & METHOD_ATTRIBUTE_STATIC) Modifiers << "static ";
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        Modifiers << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            Modifiers << "override ";
        }
    }
    else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            Modifiers << "sealed override ";
        }
    }
    else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            Modifiers << "virtual ";
        }
        else {
            Modifiers << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) Modifiers << "extern ";

    return Modifiers.str();
}

// Field API implementations
uintptr_t* Il2CppApi::ClassGetFields(Il2CppClass* klass, void** iter) {
    auto func = RemoteFuncCall<uintptr_t*, Il2CppClass*, void**>(
        il2CppOffsets::Class::il2cpp_class_get_fields + gIBaseAddress
    );
    return func(klass, iter);
}

int Il2CppApi::FieldGetFlags(uintptr_t* field) {
    auto func = RemoteFuncCall<int, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_flags + gIBaseAddress
    );
    return func(field);
}

const Il2CppType* Il2CppApi::FieldGetType(uintptr_t* field) {
    auto func = RemoteFuncCall<const Il2CppType*, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_type + gIBaseAddress
    );
    return func(field);
}

const char* Il2CppApi::FieldGetName(uintptr_t* field) {
    auto func = RemoteFuncCall<const char*, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_name + gIBaseAddress
    );
    return func(field);
}

void Il2CppApi::FieldStaticGetValue(uintptr_t* field, void* value) {
    auto func = RemoteFuncCall<void, uintptr_t*, void*>(
        il2CppOffsets::Field::il2cpp_field_static_get_value + gIBaseAddress
    );
    func(field, value);
}

size_t Il2CppApi::FieldGetOffset(uintptr_t* field) {
    auto func = RemoteFuncCall<size_t, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_offset + gIBaseAddress
    );
    return func(field);
}

// Generic type API implementations
const size_t* Il2CppApi::ClassGetInterfaces(const uint8_t* klass, size_t& count) {
    const uint64_t count_ptr = *reinterpret_cast<const uint64_t*>(klass + 120);
    count = (*reinterpret_cast<const uint8_t*>(count_ptr + 65)) ^ 0x56ULL;
    const size_t* interfaces = *reinterpret_cast<const size_t* const*>(klass + 112);
    return interfaces ? interfaces : nullptr;
}

const uint8_t* Il2CppApi::GenericClassGetGenericContainer(const uint8_t* generic_class) {
    auto raw = *reinterpret_cast<const uint64_t*>(generic_class + 8);
    return reinterpret_cast<const uint8_t*>(raw);
}

const uint8_t* Il2CppApi::ClassGetGenericClass(const uint8_t* klass) {
    auto raw = *reinterpret_cast<const uint64_t*>(klass + 96);
    return reinterpret_cast<const uint8_t*>(raw);
}

uint64_t Il2CppApi::ClassGetGenericArgCount(const uint8_t* klass) {
    const uint8_t* generic_class = ClassGetGenericClass(klass);
    if (generic_class) {
        const uint8_t* generic_container = GenericClassGetGenericContainer(generic_class);
        if (generic_container) {
            return *reinterpret_cast<const uint32_t*>(generic_container);
        }
    }
    return 0;
}

const uint8_t* Il2CppApi::ClassGetGenericArgType(const uint8_t* klass, uint64_t index) {
    const uint8_t* generic_class = ClassGetGenericClass(klass);
    if (!generic_class) return nullptr;

    const uint8_t* generic_container = GenericClassGetGenericContainer(generic_class);
    if (!generic_container) return nullptr;

    uint64_t argv_ptr = *reinterpret_cast<const uint64_t*>(generic_container + 8);
    if (!argv_ptr) return nullptr;

    const uint64_t* argv = reinterpret_cast<const uint64_t*>(argv_ptr);
    return reinterpret_cast<const uint8_t*>(argv[index]);
}

bool Il2CppApi::ClassIsGeneric(const uint8_t* klass) {
    const uint8_t* generic_class = ClassGetGenericClass(klass);
    if (!generic_class) return false;
    return GenericClassGetGenericContainer(generic_class) != nullptr;
}

// Type and reflection API implementations
char* Il2CppApi::TypeGetName(const Il2CppType* type) {
    auto func = RemoteFuncCall<char*, const Il2CppType*>(
        il2CppOffsets::Class::il2cpp_type_get_name + gIBaseAddress
    );
    return func(type);
}

Il2CppClass* Il2CppApi::FromIl2CppType(const Il2CppType* type) {
    auto func = RemoteFuncCall<Il2CppClass*, const Il2CppType*>(
        il2CppOffsets::Class::FromIl2CppType + gIBaseAddress
    );
    return func(type);
}

// Runtime API implementations
const uintptr_t* Il2CppApi::RuntimeClassInit() {
    auto func = RemoteFuncCall<const uintptr_t*>(
        il2CppOffsets::RuntimeStuff::il2cpp_runtime_class_init + gIBaseAddress
    );
    return func();
}

Il2CppDomain* Il2CppApi::DomainGet() {
    auto func = RemoteFuncCall<Il2CppDomain*>(
        il2CppOffsets::Domain::il2cpp_domain_get + gIBaseAddress
    );
    return func();
}

const Il2CppAssembly* Il2CppApi::DomainAssemblyOpen(Il2CppDomain* domain, const char* name) {
    auto func = RemoteFuncCall<const Il2CppAssembly*, Il2CppDomain*, const char*>(
        il2CppOffsets::Domain::il2cpp_domain_assembly_open + gIBaseAddress
    );
    return func(domain, name);
}