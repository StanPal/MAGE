#include "Precompiled.h"
#include "MetaPointer.h"

using namespace MAGE::Core::Meta;


MAGE::Core::Meta::MetaPointer::MetaPointer(const MetaType* pointerType)
	: MetaType(MetaType::Category::Pointer, "Pointer", sizeof(nullptr)), mPointerType(pointerType)
{
}

