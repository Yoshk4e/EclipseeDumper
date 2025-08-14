#pragma once
#include "pch.h"

//APIs Offsets Should Be Changed Every Update!
namespace il2CppOffsets {
	constexpr int GetTypeInfoFromTypeDefinitionIndex = 0x50C180;
	//easily gotten by searching this string " because generic types cannot have explicit layout."
	namespace Class {
		constexpr int il2cpp_type_get_name = 0x49A080;
		constexpr int il2cpp_class_get_namespace = 0x4993A0;
		//string: " %s%s%s must be instantiated using the ScriptableObject.CreateInstance method instead of new %s."
		constexpr int il2cpp_class_get_name = 0x0BF40;
		//string: " %s%s%s must be instantiated using the ScriptableObject.CreateInstance method instead of new %s."
		constexpr int il2cpp_class_get_methods = 0x500020;
		constexpr int FromIl2CppType = 0x4FD010;
		constexpr int il2cpp_class_from_name = 0x504970;
		constexpr int il2cpp_class_get_method_from_name = 0x0;
		constexpr int il2cpp_class_get_fields = 0x4FFA40;

		constexpr int il2cpp_class_get_flags = 0x499440;
		constexpr int il2cpp_class_get_type = 0x4994B0;
		constexpr int il2cpp_class_get_parent = 0x4993E0;
		constexpr int il2cpp_class_is_enum = 0x499580;
	}
	namespace Methods {
		const int il2cpp_method_get_name = 0x499BC0;
		const int il2cpp_method_get_return_type = 0x516700;
		const int il2cpp_method_get_param_count = 0x499C10;
		const int il2cpp_method_get_param = 0x499C20;
		const int il2cpp_method_get_param_name = 0x499D30;

		constexpr int method_pointer = 0;
		constexpr int il2cpp_method_get_flags = 42;
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
		constexpr int il2cpp_field_get_name = 0x4996A0;
		constexpr int il2cpp_field_get_type = 0x4996F0;
		constexpr int il2cpp_field_get_flags = 0x4996B0;
		constexpr int il2cpp_field_get_offset = 0x4996E0;
		constexpr int il2cpp_field_static_get_value = 0x499890;
	}
	namespace RuntimeStuff {
		constexpr int il2cpp_runtime_class_init =;
		constexpr int il2cpp_vvm_class_init =;
	}
	namespace Type {
		constexpr int class_is_valuetype = 0x499430;
	}

}
