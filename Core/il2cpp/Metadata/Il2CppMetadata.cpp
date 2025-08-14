#include "pch.h"
#include "Il2CppMetadata.h"
#include "../Core/il2cpp/Api/Il2CppApi.h"


enum class Il2CppType_Flag {
    is_param,
    is_return,
    is_field,
    is_class,
};

std::string Il2CppType_Helper(const Il2CppType* type_ptr, Il2CppType_Flag flag) {
    std::string ret;
    const uint8_t type_flag = (reinterpret_cast<const uint8_t*>(type_ptr))[11];

    if (flag == Il2CppType_Flag::is_return
        || flag == Il2CppType_Flag::is_field) {

        if (type_flag & IL2CPP_TYPE_BYREF) {
            ret += "ref ";
        }
    }

    if (flag == Il2CppType_Flag::is_param) {
        if (type_flag & IL2CPP_TYPE_BYREF) {
            if (type_ptr->attrs & PARAM_ATTRIBUTE_OUT && !(type_ptr->attrs & PARAM_ATTRIBUTE_IN)) {
                ret += "out ";
            }
            else if (type_ptr->attrs & PARAM_ATTRIBUTE_IN && !(type_ptr->attrs & PARAM_ATTRIBUTE_OUT)) {
                ret += "in ";
            }
            else {
                ret += "ref ";
            }
        }
        else {
            if (type_ptr->attrs & PARAM_ATTRIBUTE_IN) {
                ret += "[In] ";
            }
            if (type_ptr->attrs & PARAM_ATTRIBUTE_OUT) {
                ret += "[Out] ";
            }
        }
    }

    ret += Il2CppApi::TypeGetName(type_ptr);

    return ret;
}

std::string Il2CppMetadata::DumpClass(Il2CppClass* klass) {

    uint8_t* byte_klass = reinterpret_cast<uint8_t*>(klass);
    std::stringstream outPut;

    /*Il2CppApi::ClassInit(klass); */

    auto QualName = Il2CppApi::TypeGetName(Il2CppApi::ClassGetType(klass));

    outPut << "\n// Namespace: " << Il2CppApi::ClassGetNamespace(klass) << "\n";

    auto flags = Il2CppApi::ClassGetFlags(klass);

    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        outPut << "[Serializable]\n";
    }
    //TODO attribute
    bool is_valuetype = Il2CppApi::ClassIsValueType(klass);

    bool is_enum = Il2CppApi::ClassIsEnum(klass);

    auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
    switch (visibility) {
    case TYPE_ATTRIBUTE_PUBLIC:
    case TYPE_ATTRIBUTE_NESTED_PUBLIC:
        outPut << "public ";
        break;
    case TYPE_ATTRIBUTE_NOT_PUBLIC:
    case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
    case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
        outPut << "internal ";
        break;
    case TYPE_ATTRIBUTE_NESTED_PRIVATE:
        outPut << "private ";
        break;
    case TYPE_ATTRIBUTE_NESTED_FAMILY:
        outPut << "protected ";
        break;
    case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
        outPut << "protected internal ";
        break;
    }
    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "static ";
    }
    else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
    }
    else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "sealed ";
    }
    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        outPut << "interface ";
    }
    else if (is_enum) {
        outPut << "enum ";
    }
    else if (is_valuetype) {
        outPut << "struct ";
    }
    else {
        outPut << "class ";
    }
    outPut << QualName;
    std::vector<std::string> extends;
    auto parent = Il2CppApi::ClassGetParent(klass);
    if (!is_valuetype && !is_enum && parent) {
        auto parent_type = Il2CppApi::ClassGetType(parent);
        if (parent_type->type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(Il2CppApi::ClassGetName(parent));
        }
    }
    {
        size_t count = 0;
        auto interfaces = Il2CppApi::ClassGetInterfaces(byte_klass, count);

        for (size_t i = 0; i < count; ++i) {
            Il2CppClass* itf = reinterpret_cast<Il2CppClass*>(interfaces[i]);
            if (itf) {
                const Il2CppType* ifaceType = Il2CppApi::ClassGetType(itf);
                const char* fullName = Il2CppApi::TypeGetName(ifaceType);
                extends.emplace_back(fullName);
            }
        }
    }


    if (!extends.empty()) {
        outPut << " : " << extends[0];
        for (int i = 1; i < extends.size(); ++i) {
            outPut << ", " << extends[i];
        }
    }

    outPut << "\n{";
    outPut << Il2CppMetadata::DumpClassFields(klass);
    outPut << Il2CppMetadata::DumpClassMethods(klass);
    outPut << "}\n";
    return outPut.str();
}

std::string Il2CppMetadata::DumpClassMethods(Il2CppClass* klass) {
    std::stringstream outPut;

    outPut << "\n\t// Methods\n";
    void* iter = nullptr;
    while (const uint8_t* method = Il2CppApi::ClassGetMethods(klass, &iter)) {
        //TODO attribute
        int64_t VA = *(reinterpret_cast<const uint64_t*>(method + il2CppOffsets::Methods::method_pointer));
        int64_t RVA = VA - il2CppOffsets::gIBaseAddress;
        if (method) {
            outPut << "\t// RVA: 0x"
                << std::hex << RVA
                << " VA: 0x"
                << std::hex << VA;
        }
        else {
            outPut << "\t// RVA: 0x VA: 0x0";
        }
        outPut << "\n\t";
        const uint32_t flags = *reinterpret_cast<const uint32_t*>(method + il2CppOffsets::Methods::il2cpp_method_get_flags);
        outPut << Il2CppApi::GetMethodModifier(flags);
        //TODO genericContainerIndex

		const Il2CppType* return_type = Il2CppApi::MethodGetReturnType(method);

        outPut << Il2CppType_Helper(return_type, Il2CppType_Flag::is_return) << " "
            
            << Il2CppApi::MethodGetName(method) << "(";


        const uint32_t param_count = Il2CppApi::MethodGetParamCount(method);
        for (uint32_t i = 0; i < param_count; ++i) {

			const Il2CppType* param_type = Il2CppApi::MethodGetParam(method, i);

            outPut << Il2CppType_Helper(param_type, Il2CppType_Flag::is_param) << " "

                << Il2CppApi::MethodGetParamName(method, i);
            outPut << ", ";
        }

        if (param_count != 0) {
            outPut.seekp(-2, std::ios_base::cur);
        }
        outPut << ") { }\n";
        //TODO GenericInstMethod
    }

    return outPut.str();
}

std::string Il2CppMetadata::DumpClassFields(Il2CppClass* klass) {
    std::stringstream outPut;
    outPut << "\n\t// Fields\n";
    auto is_enum = Il2CppApi::ClassIsEnum(klass);
    void* iter = nullptr;
    while (auto field = Il2CppApi::ClassGetFields(klass, &iter)) {
        //TODO attribute
        outPut << "\t";
        auto attrs = Il2CppApi::FieldGetFlags(field);
        auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
        switch (access) {
        case FIELD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case FIELD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case FIELD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case FIELD_ATTRIBUTE_ASSEMBLY:
        case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL) {
            outPut << "const ";
        }
        else {
            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                outPut << "static ";
            }
            if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                outPut << "readonly ";
            }
        }
        const Il2CppType* field_type = Il2CppApi::FieldGetType(field);

        outPut << Il2CppType_Helper(field_type, Il2CppType_Flag::is_field) <<

            " " << Il2CppApi::FieldGetName(field);

        if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum) {
            uint64_t val = 0;
            Il2CppApi::FieldStaticGetValue(field, &val);
            outPut << " = " << std::dec << val;
        }
        outPut << "; // 0x" << std::hex << Il2CppApi::FieldGetOffset(field) << "\n";
    }
    return outPut.str();
}
