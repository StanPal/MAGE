#pragma once

#include "MetaType.h"

namespace MAGE::Core::Meta
{
	class MetaPointer : public MetaType
	{
	public:
		MetaPointer(const MetaType* elementType);

		const MetaType* GetPointerType() const { return mPointerType; }

	private:
		const MetaType* const mPointerType;
	};
}