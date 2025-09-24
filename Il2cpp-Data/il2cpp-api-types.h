#include "pch.h"
#pragma once
#include <cstdint>
#include "il2cpp-metadata.h"
// il2cpp-types.h

// Add forward declarations for all referenced types
struct Il2CppImage;
struct Il2CppType;
struct Il2CppGenericClass;
struct Il2CppTypeDefinition;
struct Il2CppInteropData;
struct FieldInfo;
struct EventInfo;
struct PropertyInfo;
struct Il2CppRuntimeInterfaceOffsetPair;
struct Il2CppRGCTXData;
struct VirtualInvokeData;
struct Il2CppMethodDefinition;
struct Il2CppGenericMethod;
struct Il2CppGenericContainer;
struct ParameterInfo;

// Define function pointer types
typedef void (*Il2CppMethodPointer)();
typedef void* (*InvokerMethod)(Il2CppMethodPointer, const struct MethodInfo*, void*, void**);

// Add this before Il2CppClass definition
#ifdef _MSC_VER
#define ALIGN_TYPE(n) __declspec(align(n))
#else
#define ALIGN_TYPE(n) __attribute__((aligned(n)))
#endif

// Add VirtualInvokeData definition
struct VirtualInvokeData {
    void* methodPtr;
    const struct MethodInfo* method;
};

// Define IL2CPP_ZERO_LEN_ARRAY if missing
#ifndef IL2CPP_ZERO_LEN_ARRAY
#define IL2CPP_ZERO_LEN_ARRAY 1
#endif

struct Il2CppClass
{
    // The following fields are always valid for a Il2CppClass structure
    const Il2CppImage* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
    const Il2CppType* byval_arg;
    const Il2CppType* this_arg;
    Il2CppClass* element_class;
    Il2CppClass* castClass;
    Il2CppClass* declaringType;
    Il2CppClass* parent;
    Il2CppGenericClass* generic_class;
    const Il2CppTypeDefinition* typeDefinition; // non-NULL for Il2CppClass's constructed from type defintions
    const Il2CppInteropData* interopData;
    // End always valid fields

    // The following fields need initialized before access. This can be done per field or as an aggregate via a call to Class::Init
    FieldInfo* fields; // Initialized in SetupFields
    const EventInfo* events; // Initialized in SetupEvents
    const PropertyInfo* properties; // Initialized in SetupProperties
    const MethodInfo** methods; // Initialized in SetupMethods
    Il2CppClass** nestedTypes; // Initialized in SetupNestedTypes
    Il2CppClass** implementedInterfaces; // Initialized in SetupInterfaces
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets; // Initialized in Init
    void* static_fields; // Initialized in Init
    const Il2CppRGCTXData* rgctx_data; // Initialized in Init
    // used for fast parent checks
    Il2CppClass** typeHierarchy; // Initialized in SetupTypeHierachy
    // End initialization required fields

    uint32_t cctor_started;
    uint32_t cctor_finished;
    ALIGN_TYPE(8) uint64_t cctor_thread;

    // Remaining fields are always valid except where noted
    GenericContainerIndex genericContainerIndex;
    CustomAttributeIndex customAttributeIndex;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;

    uint16_t method_count; // lazily calculated for arrays, i.e. when rank > 0
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count; // lazily calculated for arrays, i.e. when rank > 0
    uint16_t interfaces_count;
    uint16_t interface_offsets_count; // lazily calculated for arrays, i.e. when rank > 0

    uint8_t typeHierarchyDepth; // Initialized in SetupTypeHierachy
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t packingSize;

    uint8_t valuetype : 1;
    uint8_t initialized : 1;
    uint8_t enumtype : 1;
    uint8_t is_generic : 1;
    uint8_t has_references : 1;
    uint8_t init_pending : 1;
    uint8_t size_inited : 1;
    uint8_t has_finalize : 1;
    uint8_t has_cctor : 1;
    uint8_t is_blittable : 1;
    uint8_t is_import_or_windows_runtime : 1;
    uint8_t is_vtable_initialized : 1;
    VirtualInvokeData vtable[IL2CPP_ZERO_LEN_ARRAY];
};

typedef struct MethodInfo
{
    void* methodPointer;
    char _padding[8]; // Added 8 bytes of padding for methodPointer

    char invoker_method;
    const char* name;
    void* klass;
    Il2CppType* return_type;
    const void* parameters;

    union
    {
        const void* rgctx_data; /* is_inflated is true and is_generic is false, i.e. a generic instance method */
        const void* methodDefinition;
    };

    /* note, when is_generic == true and is_inflated == true the method represents an uninflated generic method on an inflated type. */
    union
    {
        const void* genericMethod; /* is_inflated is true */
        const void* genericContainer; /* is_inflated is false and is_generic is true */
    };

    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1;           /* true if method is a generic method definition */
    uint8_t is_inflated : 1;          /* true if declaring_type is a generic instance or if method is a generic instance*/
    uint8_t wrapper_type : 1;         /* always zero (MONO_WRAPPER_NONE) needed for the debugger */
    uint8_t is_marshaled_from_native : 1; /* a fake MethodInfo wrapping a native function pointer */
} MethodInfo;

