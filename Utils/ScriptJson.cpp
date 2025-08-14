// https://github.com/thexeondev/GracefulDumper/blob/master/idapy-gen/src/lib.rs

#include "pch.h"
#include "scriptjson.h"
#include "../Core/il2cpp/Api/Il2CppApi.h"
#include <sstream>
#include <vector>

using namespace std;

void ScriptJsonGenerator::WriteEscapedString(stringstream& ss, const string& value) {
    for (char c : value) {
        switch (c) {
        case '"':  ss << "\\\""; break;
        case '\\': ss << "\\\\"; break;
        case '\n': ss << "\\n";  break;
        case '\r': ss << "\\r";  break;
        case '\t': ss << "\\t";  break;
        default:   ss << c;      break;
        }
    }
}



string ScriptJsonGenerator::GenerateJson() {
    stringstream ss;
    ss << "{";

    // ScriptString (empty array)
    ss << "\"ScriptString\":[],";

    // ScriptMetadata
    ss << "\"ScriptMetadata\":";
    WriteTypeMetadata(ss);
    ss << ",";

    // ScriptMethod
    ss << "\"ScriptMethod\":";
    WriteMethods(ss);
    ss << ",";

    // ScriptMetadataMethod
    ss << "\"ScriptMetadataMethod\":";
    WriteMethodRefs(ss);

    ss << "}";
    return ss.str();
}


void ScriptJsonGenerator::WriteMethods(stringstream& ss) {
    vector<string> entries;
    ss << "[";

    for (int index = 0; index <= 56983; ++index) {
        if (Il2CppClass* klass = Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(index)) {
            void* methodIter = nullptr;
            while (auto method = Il2CppApi::ClassGetMethods(klass, &methodIter)) {
                stringstream entry;
                entry << "{"
                    << "\"Address\":" << ((*(uint64_t*)((char*)method + 8)) - il2CppOffsets::gIBaseAddress) << ","
                    << "\"Name\":\"" << Il2CppApi::ClassGetName(klass) << "$$" << Il2CppApi::MethodGetName(method) << "\","
                    << "\"Signature\":\"\","
                    << "\"TypeSignature\":\"\""
                    << "}";
                entries.push_back(entry.str());
            }
        }
    }

    for (size_t i = 0; i < entries.size(); ++i) {
        if (i > 0) ss << ",";
        ss << entries[i];
    }
    ss << "]";
}

void ScriptJsonGenerator::WriteTypeMetadata(stringstream& ss) {
    vector<string> entries;
    ss << "[";

    for (int index = 0; index <= 56983; ++index) {
        if (Il2CppClass* klass = Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(index)) {
            stringstream entry;
            entry << "{"
                << "\"Address\":" << (reinterpret_cast<uintptr_t>(klass) - il2CppOffsets::gIBaseAddress) << ","
                << "\"Name\":\"" << Il2CppApi::ClassGetName(klass) << "_TypeInfo\","
                << "\"Signature\":\"" << Il2CppApi::ClassGetName(klass) << "_c*\""
                << "}";
            entries.push_back(entry.str());
        }
    }

    for (size_t i = 0; i < entries.size(); ++i) {
        if (i > 0) ss << ",";
        ss << entries[i];
    }
    ss << "]";
}

void ScriptJsonGenerator::WriteMethodRefs(stringstream& ss) {
    vector<string> entries;
    ss << "[";

    for (int index = 0; index <= 56983; ++index) {
        if (Il2CppClass* klass = Il2CppApi::GetTypeInfoFromTypeDefinitionIndex(index)) {
            void* methodIter = nullptr;
            while (auto method = Il2CppApi::ClassGetMethods(klass, &methodIter)) {
                stringstream entry;
                entry << "{"
                    << "\"Address\":" << (reinterpret_cast<uintptr_t>(method) - il2CppOffsets::gIBaseAddress) << ","
                    << "\"Name\":\"" << Il2CppApi::ClassGetName(klass) << "_" << Il2CppApi::MethodGetName(method) << "\","
                    << "\"MethodAddress\":" << (*(uint64_t*)((char*)method + 8) - il2CppOffsets::gIBaseAddress)
                    << "}";
                entries.push_back(entry.str());
            }
        }
    }

    for (size_t i = 0; i < entries.size(); ++i) {
        if (i > 0) ss << ",";
        ss << entries[i];
    }
    ss << "]";
}