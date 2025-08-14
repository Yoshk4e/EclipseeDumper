#include "pch.h"
#include "../Core/il2cpp/Offsets/Il2CppOffsets.h"
#include "Il2CppApi.h"
#include <libloaderapi.h>
#include <cstdio>
#include <cstdint>
#include <cstddef>  


// Type and reflection API implementations
char* Il2CppApi::TypeGetName(const Il2CppType* type) {
    auto func = RemoteFuncCall<char*, const Il2CppType*>(
        il2CppOffsets::Class::il2cpp_type_get_name
    );
    return func(type);
}




// Type system API implementations
Il2CppClass* Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(TypeDefinitionIndex index) {
    auto func = RemoteFuncCall<Il2CppClass*, TypeDefinitionIndex>(
        il2CppOffsets::GetTypeInfoFromTypeDefinitionIndex
    );
    return func(index);
}

const char* Il2CppApi::ClassGetNamespace(Il2CppClass* klass) {
    auto func = RemoteFuncCall<const char*, Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_namespace
    );
    return func(klass);
}

const char* Il2CppApi::ClassGetName(Il2CppClass* klass) {
    auto func = RemoteFuncCall<const char*, Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_name
    );
    return func(klass);
}

bool Il2CppApi::ClassIsEnum(const Il2CppClass* klass) {
    auto func = RemoteFuncCall<bool, const Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_is_enum
    );
    auto result = func(klass) != 0;

    return result;
}

bool Il2CppApi::ClassIsValueType(const Il2CppClass* klass) {
    auto func = RemoteFuncCall<bool, const Il2CppClass*>(
        il2CppOffsets::Type::class_is_valuetype
    );

    return func(klass);
}

int Il2CppApi::ClassGetFlags(const Il2CppClass* klass) {
    auto func = RemoteFuncCall<bool, const Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_flags
    );
    return func(klass);
}

Il2CppClass* Il2CppApi::ClassGetParent(Il2CppClass* klass) {
    auto func = RemoteFuncCall<Il2CppClass*, Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_parent
    );
    return func(klass);
}

const Il2CppType* Il2CppApi::ClassGetType(Il2CppClass* klass) {
    auto func = RemoteFuncCall<const Il2CppType*, Il2CppClass*>(
        il2CppOffsets::Class::il2cpp_class_get_type
    );
    return func(klass);
}

Il2CppClass* Il2CppApi::ClassFromName(const Il2CppImage* image, const char* namespaze, const char* name) {
    auto func = RemoteFuncCall<Il2CppClass*, const Il2CppImage*, const char*, const char*>(
        il2CppOffsets::Class::il2cpp_class_from_name
    );
    return func(image, namespaze, name);
}

bool Il2CppApi::ClassInit(Il2CppClass* klass) {
    auto func = RemoteFuncCall<bool, Il2CppClass*>(
        il2CppOffsets::RuntimeStuff::il2cpp_vm_class_init
    );
    return func(klass);
}

// Method API implementations
const uint8_t* Il2CppApi::ClassGetMethods(Il2CppClass* klass, void* iter) {
    auto func = RemoteFuncCall<const uint8_t*, Il2CppClass*, void*>(
        il2CppOffsets::Class::il2cpp_class_get_methods
    );
    return func(klass, iter);
}

const char* Il2CppApi::MethodGetName(const uint8_t* MethodPtr) {
    auto func = RemoteFuncCall<const char*, const uint8_t*>(
        il2CppOffsets::Methods::il2cpp_method_get_name
    );
    return func(MethodPtr);
}

const Il2CppType* Il2CppApi::MethodGetReturnType(const uint8_t* method) {
    auto func = RemoteFuncCall<const Il2CppType*, const uint8_t*>(
        il2CppOffsets::Methods::il2cpp_method_get_return_type
    ); 
    
    return func(method);
}

const uint32_t Il2CppApi::MethodGetParamCount(const uint8_t* method) {
    auto func = RemoteFuncCall<const uint32_t, const uint8_t*>(
        il2CppOffsets::Methods::il2cpp_method_get_param_count
    );
    return func(method);
}

const Il2CppType* Il2CppApi::MethodGetParam(const uint8_t* method, uint32_t index) {
    auto func = RemoteFuncCall<const Il2CppType*, const uint8_t*, uint32_t>(
        il2CppOffsets::Methods::il2cpp_method_get_param
    );

    return func(method, index);
}

const char* Il2CppApi::MethodGetParamName(const uint8_t* method, uint32_t index) {
    auto func = RemoteFuncCall<const char*, const uint8_t*, uint32_t>(
        il2CppOffsets::Methods::il2cpp_method_get_param_name
    );
    return func(method, index);
}

const uintptr_t* Il2CppApi::ClassGetMethodFromName(Il2CppClass* klass, const char* name, int argsCount) {
    auto func = RemoteFuncCall<uintptr_t*, Il2CppClass*, const char*, int>(
        il2CppOffsets::Class::il2cpp_class_get_method_from_name
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
        il2CppOffsets::Class::il2cpp_class_get_fields
    );
    return func(klass, iter);
}

int Il2CppApi::FieldGetFlags(uintptr_t* field) {
    auto func = RemoteFuncCall<int, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_flags
    );
    return func(field);
}

const Il2CppType* Il2CppApi::FieldGetType(uintptr_t* field) {
    auto func = RemoteFuncCall<const Il2CppType*, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_type
    );
    return func(field);
}

const char* Il2CppApi::FieldGetName(uintptr_t* field) {
    auto func = RemoteFuncCall<const char*, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_name
    );
    return func(field);
}

void Il2CppApi::FieldStaticGetValue(uintptr_t* field, void* value) {
    auto func = RemoteFuncCall<void, uintptr_t*, void*>(
        il2CppOffsets::Field::il2cpp_field_static_get_value
    );
    func(field, value);
}

size_t Il2CppApi::FieldStaticGetValue(Il2CppField* field) {
    size_t out = 0;
    FieldStaticGetValue(
        reinterpret_cast<uintptr_t*>(field),
        &out
    );
    return out;
}


bool Il2CppApi::FieldIsInstance(uintptr_t* field) {

    int flags = Il2CppApi::FieldGetFlags(field);
    return (flags & FIELD_ATTRIBUTE_STATIC) == 0;
}

size_t Il2CppApi::FieldGetOffset(uintptr_t* field) {
    auto func = RemoteFuncCall<size_t, uintptr_t*>(
        il2CppOffsets::Field::il2cpp_field_get_offset
    );
    return func(field);
}

// Generic type API implementations
const size_t* Il2CppApi::ClassGetInterfaces(const uint8_t* klass, size_t& count) {
    const uint64_t count_ptr = *reinterpret_cast<const uint64_t*>(klass + 24);
    count = static_cast<size_t>(static_cast<unsigned char>(
        *reinterpret_cast<const uint8_t*>(count_ptr + 65) - 50));
    const size_t* interfaces = *reinterpret_cast<const size_t* const*>(klass + 56);
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




Il2CppClass* Il2CppApi::FromIl2CppType(const Il2CppType* type) {
    auto func = RemoteFuncCall<Il2CppClass*, const Il2CppType*>(
        il2CppOffsets::Class::FromIl2CppType
    );
    return func(type);
}

// Runtime API implementations
const uintptr_t* Il2CppApi::RuntimeClassInit() {
    auto func = RemoteFuncCall<const uintptr_t*>(
        il2CppOffsets::RuntimeStuff::il2cpp_runtime_class_init
    );
    return func();
}

Il2CppObject* Il2CppApi::MethodInvoke(const uintptr_t* method, void* obj, void** params) { //under the hood this is il2cpp_runtime_invoke
    Il2CppException* exception = nullptr;

    auto func = RemoteFuncCall<Il2CppObject*, const uintptr_t*, void*, void**, Il2CppException**>(
        il2CppOffsets::RuntimeStuff::il2cpp_runtime_invoke
    );

    return func(method, obj, params, &exception);
}

Il2CppDomain* Il2CppApi::DomainGet() {
    auto func = RemoteFuncCall<Il2CppDomain*>(
        il2CppOffsets::Domain::il2cpp_domain_get
    );
    auto dom = func();
    printf("[DomainGet] returned %p\n", dom);
    return func();
}

const Il2CppAssembly* Il2CppApi::DomainAssemblyOpen(const char* name)
{
    // RCX = Il2CppDomain*, RDX = const char*
    auto func = RemoteFuncCall<const Il2CppAssembly*,
        Il2CppDomain*,
        const char*>(
            il2CppOffsets::Domain::il2cpp_domain_assembly_open
        );

    Il2CppDomain* domain = DomainGet();
    return func(domain, name);
}


const Il2CppImage* Il2CppApi::AssemblyGetImage(const Il2CppAssembly* assembly) {
    auto func = RemoteFuncCall<const Il2CppImage*, const Il2CppAssembly*>(
        il2CppOffsets::Domain::il2cpp_assembly_get_image
    );
    return func(assembly);
}

Il2CppClass* Il2CppApi::ObjectGetClass(Il2CppObject* obj)
{
    return obj->klass;
}

Il2CppArray* Il2CppApi::ArrayNew(Il2CppClass* elementTypeInfo, il2cpp_array_size_t length) {
    auto func = RemoteFuncCall<Il2CppArray*, Il2CppClass*, il2cpp_array_size_t>(
        il2CppOffsets::RuntimeStuff::il2cpp_vm_array_new
    );
    return func(elementTypeInfo, length);
}

Il2CppObject* Il2CppApi::ObjectNew(Il2CppClass* klass) {
    auto func = RemoteFuncCall<Il2CppObject*, Il2CppClass*>(
        il2CppOffsets::RuntimeStuff::il2cpp_object_new
    );
    return func(klass);
}

void Il2CppApi::GCDisable() {
    auto func = RemoteFuncCall<void>(
        il2CppOffsets::GC::il2cpp_gc_disable
    );
    return func();
}

Il2CppThread* Il2CppApi::ThreadAttach(Il2CppDomain* domain) {
    auto func = RemoteFuncCall<Il2CppThread*, Il2CppDomain*>(
        il2CppOffsets::RuntimeStuff::il2cpp_thread_attach
    );
    return func(domain);
}



size_t Il2CppApi::ImageGetClassCount(const Il2CppImage* image)
{
    return *reinterpret_cast<const uint32_t*>(reinterpret_cast<const char*>(image) + 24);
}


const Il2CppClass* Il2CppApi::ImageGetType(const Il2CppImage* image, size_t index) {
    uintptr_t v1 = reinterpret_cast<uintptr_t>(image);

    //
    int32_t offsetValue = *reinterpret_cast<const int32_t*>(v1 + 0x0C);

    //
    TypeDefinitionIndex typeDefinitionIndex = static_cast<int32_t>(index) + offsetValue + 0xC417DD23;

    return Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(typeDefinitionIndex);
}

