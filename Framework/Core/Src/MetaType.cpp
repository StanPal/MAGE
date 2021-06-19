#include "Precompiled.h"
#include "MetaType.h"

#include "DebugUtil.h"
#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaPointer.h"

using namespace MAGE::Core::Meta;

MetaType::MetaType(Category category, const char* name, size_t size, DeserializeFunc deserialize)
	: mCategory(category), mName(name), mSize(size), mDeserialize(std::move(deserialize))
{}

const MetaArray* MAGE::Core::Meta::MetaType::AsMetaArray() const
{
	ASSERT(mCategory == Category::Array, "MetaType -- MetaType is not a Array type!");
	return static_cast<const MetaArray*>(this);
}

const MetaClass* MAGE::Core::Meta::MetaType::AsMetaClass() const
{
	ASSERT(mCategory == Category::Class, "MetaType -- MetaType is not a Class type!");
	return static_cast<const MetaClass*>(this);
}

const MetaPointer* MAGE::Core::Meta::MetaType::AsMetaPointer() const
{
	ASSERT(mCategory == Category::Pointer, "MetaType -- MetaType is not a Pointer type!");
	return static_cast<const MetaPointer*>(this);

	return nullptr;
}

void MAGE::Core::Meta::MetaType::Deserialize(void* instance, const rapidjson::Value& jsonValue) const
{
	ASSERT(mDeserialize, "MetaType -- no deserialize callabe registered for '%s'", GetName());
	mDeserialize(instance, jsonValue);
}
