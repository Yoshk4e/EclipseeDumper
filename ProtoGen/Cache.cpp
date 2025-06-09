#include "pch.h"
#include "../Il2cpp-Data/il2cpp-api-types.h"
#include <map>

enum CachedType{
    Object,
    Boolean,
    Byte,
    SByte,
    UInt16,
    Int16,
    UInt32,
    Int32,
    UInt64,
    Int64,
    Single,
    Double,
    String,
    ByteString,
    Any,
    Enum,
};

struct TypeCache {
	std::map<std::size_t, CachedType> type_map;
};

/*namespace TypeCache {
    
}*/