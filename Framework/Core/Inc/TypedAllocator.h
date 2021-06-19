#pragma once

#include "BlockAllocator.h"

namespace MAGE::Core
{
	template <class DataType>
	class TypedAllocator : private BlockAllocator
	{
	public:
		TypedAllocator(size_t capacity) : BlockAllocator(sizeof(DataType), capacity)
		{
		}
		
		//Variated Template: I don't what the type is and how many arguments.
		//Args&& is forwarding reference, it makes sure if it is a reference
		//it stays as a reference. 
		template <class... Args>
		DataType* New(Args&&... args)
		{

			// Get a new block from BlockAllocator
			DataType* dataType = (DataType*)Allocate();
			if (!dataType)
				return nullptr;
			// Use placement new on the returned block, like new but you pass in where you 
			// want the memory to be placed, doesn't allocate but only calls the constructor. 
			DataType* result = new (dataType) DataType(std::forward<Args>(args)...);
			return result;
		}

		void Delete(DataType* ptr)
		{
			//Check if the ptr is a null ptr if it is skip and don't do anything
			if (ptr == nullptr)
				return;


			// Call destructor explicitly
			ptr->~DataType();
	
			//return it to block allocator. 
			Free(mData);
			
	
		}
	};
}