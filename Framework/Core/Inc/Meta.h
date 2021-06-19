#pragma once

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaRegistry.h"
#include "MetaPointer.h"
#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template <> const MAGE::Core::Meta::MetaType* MAGE::Core::Meta::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template <> const MAGE::Core::Meta::MetaType* MAGE::Core::Meta::GetMetaType<Type>()\
	{\
		static const MAGE::Core::Meta::MetaType sMetaType(\
			MAGE::Core::Meta::MetaType::Category::Primitive, #Name, sizeof(Type),\
			MAGE::Core::Meta::Deserialize<Type>);\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const MAGE::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const MAGE::Core::Meta::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }


#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const MAGE::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_DERIVED_BEGIN(ClassType,ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const MAGE::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const MAGE::Core::Meta::MetaType* MAGE::Core::Meta::GetMetaType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const MAGE::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;

#define META_FIELD_BEGIN\
		static const std::initializer_list<MAGE::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ MAGE::Core::Meta::GetFieldType(&Class::Var), Name, MAGE::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<MAGE::Core::Meta::MetaField> fields;

#define META_CLASS_END\
		static const MAGE::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			parentMetaClass, fields,\
			[]{return new Class();});\
		return &sMetaClass;\
	}
