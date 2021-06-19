#pragma once

namespace MAGE::Core::Meta
{
	class MetaType;

	template <class DataType>
	const MetaType* GetMetaType();

	template <class DataType>
	void Deserialize(void* instance, const rapidjson::Value& value)
	{
		static_assert(sizeof(DataType) == -1, "No specialization found for deerializing this type.");
	}
	namespace Detail
	{
		template<class DataType>
		inline const MetaType* GetMetaTypeTmpl(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "No MetaType found for this type.");
		}

		template<class DataType>
		inline const MetaType* GetMetaTypeTmpl(DataType**)
		{
			static const MetaPointer sMetaPointer(GetMetaType<DataType>());
			return &sMetaPointer;
		}

		//Recursively call the type within the metaArray(vector) until we find a type that matches
		template<class DataType>
		inline const MetaType* GetMetaTypeTmpl(std::vector<DataType>*)
		{
			static const MetaArray sMetaArray(GetMetaType<DataType>());
			return &sMetaArray;
		}


	}

	// SFINAE - Substitution Failue Is Not An Error 

	template <class DataType>
	inline const MetaType* GetMetaType()
	{
		return Detail::GetMetaTypeTmpl(static_cast<DataType*>(nullptr));
	}

	template <class ClassType, class DataType>
	inline const MetaType* GetFieldType(DataType ClassType::*)
	{
		return GetMetaType<DataType>();
	}

	template <class ClassType, class DataType>
	inline size_t GetFieldOffset(DataType ClassType::* field)
	{
		return (size_t)(void*)&(((ClassType*)nullptr)->*field);
	}

}