#include "pch.h"
#include "Il2CppMetadataRenderer.h"
#include <sstream>

namespace EclipseKit::Engines::Il2Cpp {

    static std::string BuildMethodSignature(const uint8_t* method, bool useFullNames) {
        std::stringstream ss;

        const uint32_t flags = *reinterpret_cast<const uint32_t*>(
            method + EclipseKit::Core::Configuration::ConfigurationManager::AutoDetectConfiguration()->Offsets.MethodGetFlags
        );
        ss << Il2CppApiWrapper::GetMethodModifiers(flags);

        const Il2CppType* returnType = Il2CppApiWrapper::GetMethodReturnType(method);
        const char* returnName = nullptr;
        if (useFullNames) {
            returnName = static_cast<const char*>(Il2CppApiWrapper::GetTypeName(returnType));
        } else {
            auto returnClass = Il2CppApiWrapper::GetClassFromType(returnType);
            returnName = Il2CppApiWrapper::GetClassName(returnClass);
        }
        ss << returnName << " " << Il2CppApiWrapper::GetMethodName(method) << "(";

        const uint32_t paramCount = Il2CppApiWrapper::GetMethodParameterCount(method);
        for (uint32_t i = 0; i < paramCount; ++i) {
            const Il2CppType* param = Il2CppApiWrapper::GetMethodParameter(method, i);
            const char* typeName = nullptr;
            if (useFullNames) {
                typeName = static_cast<const char*>(Il2CppApiWrapper::GetTypeName(param));
            } else {
                auto paramClass = Il2CppApiWrapper::GetClassFromType(param);
                typeName = Il2CppApiWrapper::GetClassName(paramClass);
            }
            ss << typeName << " " << Il2CppApiWrapper::GetMethodParameterName(method, i);
            if (i + 1 < paramCount) ss << ", ";
        }
        ss << ") { }";
        return ss.str();
    }

    std::string RenderClassCSharp(Il2CppClass* klass) {
        Il2CppApiWrapper::DisableGarbageCollection();
        std::stringstream out;

        // Get configuration for UseFullNames option
        const auto* config = EclipseKit::Core::Configuration::ConfigurationManager::AutoDetectConfiguration();
        bool useFullNames = config ? config->UseFullNames : false;

        const char* className = nullptr;
        if (useFullNames) {
            className = static_cast<const char*>(Il2CppApiWrapper::GetTypeName(Il2CppApiWrapper::GetClassType(klass)));
        } else {
            className = Il2CppApiWrapper::GetClassName(klass);
        }
        const char* nameSpace = Il2CppApiWrapper::GetClassNamespace(klass);

        out << "\n// Namespace: " << nameSpace << "\n";

        int flags = Il2CppApiWrapper::GetClassFlags(klass);
        bool isEnum = Il2CppApiWrapper::IsClassEnum(klass);
        bool isValueType = *(reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(klass) + 40)) >= 0;

        auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
        switch (visibility) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC: out << "public "; break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY: out << "internal "; break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE: out << "private "; break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY: out << "protected "; break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM: out << "protected internal "; break;
        }

        if ((flags & TYPE_ATTRIBUTE_ABSTRACT) && (flags & TYPE_ATTRIBUTE_SEALED)) out << "static ";
        else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && (flags & TYPE_ATTRIBUTE_ABSTRACT)) out << "abstract ";
        else if (!isValueType && !isEnum && (flags & TYPE_ATTRIBUTE_SEALED)) out << "sealed ";

        if (flags & TYPE_ATTRIBUTE_INTERFACE) out << "interface ";
        else if (isEnum) out << "enum ";
        else if (isValueType) out << "struct ";
        else out << "class ";

        out << className;

        std::vector<std::string> extends;
        auto parent = Il2CppApiWrapper::GetClassParent(klass);
        if (!isValueType && !isEnum && parent) {
            auto parentType = Il2CppApiWrapper::GetClassType(parent);
            if (parentType->type != IL2CPP_TYPE_OBJECT) {
                const char* parentName = nullptr;
                if (useFullNames) {
                    parentName = static_cast<const char*>(Il2CppApiWrapper::GetTypeName(parentType));
                } else {
                    parentName = Il2CppApiWrapper::GetClassName(parent);
                }
                extends.emplace_back(parentName);
            }
        }
        {
            void* iter = nullptr;
            Il2CppClass* interfaceClass;
            while ((interfaceClass = Il2CppApiWrapper::GetClassInterfaces(klass, &iter)) != nullptr) {
                auto interfaceType = Il2CppApiWrapper::GetClassType(interfaceClass);
                const char* interfaceName = nullptr;
                if (useFullNames) {
                    interfaceName = static_cast<const char*>(Il2CppApiWrapper::GetTypeName(interfaceType));
                } else {
                    interfaceName = Il2CppApiWrapper::GetClassName(interfaceClass);
                }
                extends.emplace_back(interfaceName);
            }
        }

        if (!extends.empty()) {
            out << " : " << extends[0];
            for (size_t i = 1; i < extends.size(); ++i) {
                out << ", " << extends[i];
            }
        }

        out << "\n{";

        // Fields
        out << "\n\t// Fields\n";
        void* fieldIter = nullptr;
        bool isEnumClass = Il2CppApiWrapper::IsClassEnum(klass);
        while (auto field = Il2CppApiWrapper::GetClassFields(klass, &fieldIter)) {
            out << "\t";
            auto attrs = Il2CppApiWrapper::GetFieldFlags(field);
            auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
            switch (access) {
            case FIELD_ATTRIBUTE_PRIVATE: out << "private "; break;
            case FIELD_ATTRIBUTE_PUBLIC: out << "public "; break;
            case FIELD_ATTRIBUTE_FAMILY: out << "protected "; break;
            case FIELD_ATTRIBUTE_ASSEMBLY:
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM: out << "internal "; break;
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM: out << "protected internal "; break;
            }
            if (attrs & FIELD_ATTRIBUTE_LITERAL) out << "const ";
            else {
                if (attrs & FIELD_ATTRIBUTE_STATIC) out << "static ";
                if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) out << "readonly ";
            }

            auto fieldType = Il2CppApiWrapper::GetFieldType(field);
            const char* fieldTypeName = nullptr;
            if (useFullNames) {
                fieldTypeName = static_cast<const char*>(Il2CppApiWrapper::GetTypeName(fieldType));
            } else {
                auto fieldClass = Il2CppApiWrapper::GetClassFromType(fieldType);
                fieldTypeName = Il2CppApiWrapper::GetClassName(fieldClass);
            }
            out << fieldTypeName << " " << Il2CppApiWrapper::GetFieldName(field);

            if ((attrs & FIELD_ATTRIBUTE_LITERAL) && isEnumClass) {
                uint64_t val = 0;
                Il2CppApiWrapper::GetStaticFieldValue(field, &val);
                out << " = " << std::dec << val;
            }

            out << "; // 0x" << std::hex << Il2CppApiWrapper::GetFieldOffset(field) << "\n";
        }

        // Methods
        out << "\n\t// Methods\n";
        void* iter = nullptr;
        while (const uint8_t* method = Il2CppApiWrapper::GetClassMethods(klass, &iter)) {
            const auto* cfgMod = EclipseKit::Core::Configuration::ConfigurationManager::AutoDetectConfiguration();
            HMODULE baseMod = (cfgMod && cfgMod->UseMainModule)
                ? GetModuleHandleA(nullptr)
                : GetModuleHandleA("GameAssembly.dll");
            uintptr_t moduleBase = reinterpret_cast<uintptr_t>(baseMod);
            int64_t VA = *(reinterpret_cast<const uint64_t*>(method + EclipseKit::Core::Configuration::ConfigurationManager::AutoDetectConfiguration()->Offsets.MethodPointer));
            int64_t RVA = VA - moduleBase;

            out << "\t// RVA: 0x" << std::hex << std::uppercase << RVA
                << " VA: 0x" << std::hex << std::uppercase << VA << "\n\t";

            out << BuildMethodSignature(method, useFullNames) << "\n";
        }

        out << "}\n";
        return out.str();
    }
}
