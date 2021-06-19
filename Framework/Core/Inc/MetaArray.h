#pragma once

#include "MetaType.h"

namespace MAGE::Core::Meta
{
	class MetaArray : public MetaType
	{
	public:
		MetaArray(const MetaType* elementType);

		const MetaType* GetElementType() const { return mElementType; }

	private:
		const MetaType* mElementType;
	};
}