#pragma once
#include "pch.h"

//APIs Offsets Should Be Changed Every Update!
namespace il2CppOffsets {
	constexpr int GetTypeInfoFromTypeDefinitionIndex =;
	//easily gotten by searching this string " because generic types cannot have explicit layout."
	namespace Class {
		constexpr int il2cpp_type_get_name =;
		constexpr int il2cpp_class_get_namespace =;
		//string: " %s%s%s must be instantiated using the ScriptableObject.CreateInstance method instead of new %s."
		constexpr int il2cpp_class_get_name =;
		//string: " %s%s%s must be instantiated using the ScriptableObject.CreateInstance method instead of new %s."
		constexpr int il2cpp_class_get_methods =;
		constexpr int FromIl2CppType =;
		constexpr int il2cpp_class_from_name =;
		constexpr int il2cpp_class_get_method_from_name =;
		constexpr int il2cpp_class_get_fields =;

		constexpr int il2cpp_class_get_flags =;
	}
	namespace Methods {
		constexpr int il2cpp_method_get_name =;
		constexpr int il2cpp_method_get_return_type = ;
		constexpr int il2cpp_method_get_param_count = ;
		constexpr int il2cpp_method_get_param = ;
		constexpr int il2cpp_method_get_param_name = ;

		constexpr int method_pointer = ;
		constexpr int il2cpp_method_get_flags = ;
	}
	namespace GC {
		constexpr int il2cpp_gc_disable =;
	}
	namespace Domain {
		constexpr int il2cpp_domain_get =;
		constexpr int il2cpp_domain_get_assemblies = ;
		constexpr int il2cpp_domain_assembly_open = ;
		constexpr int il2cpp_assembly_get_image = ;

	}
	namespace Field {
		constexpr int il2cpp_field_get_name = ;
		constexpr int il2cpp_field_get_type = ;
		constexpr int il2cpp_field_get_flags = ;
		constexpr int il2cpp_field_get_offset = ;
		constexpr int il2cpp_field_static_get_value =;
	}
	namespace RuntimeStuff {
		constexpr int il2cpp_runtime_class_init =;
		constexpr int il2cpp_vvm_class_init =;
	}
	namespace Type {
		constexpr int is_valuetype =;
	}

}
