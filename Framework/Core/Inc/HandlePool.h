#pragma once

#include "Common.h"
#include "DebugUtil.h"

namespace MAGE::Core
{
	template <class DataType>
	class Handle; //forward declaration

	template <class DataType>
	class HandlePool
	{
	public:
		//Convience to make sure we are handling the specified datatype ie cat pointing to cat not dog. 
		using HandleType = Handle<DataType>;

		HandlePool(size_t capacity)
		{

			// allocate capacity + 1 entries (we don't use slot 0, thats why we add + 1 to capcity) 
			// add freeslots to mFreeSlots (exlcuding 0)
			mEntries.resize(capacity + 1);
			mFreeSlots.reserve(capacity);

			for (size_t i = 1; i <= capacity; ++i)
			{
				mFreeSlots.push_back(i);
			}

			ASSERT(HandleType::sPool == nullptr, "HandlePool -- Cannot have more than one pool of this type!");
			HandleType::sPool = this;
		}
		~HandlePool()
		{
			ASSERT(HandleType::sPool == this, "HandlePool -- Something is wrong...");
			HandleType::sPool = nullptr;
		}

		HandleType Register(DataType* instance)
		{
			ASSERT(instance != nullptr, "HandlePool -- Invalid instance.");
			ASSERT(!mFreeSlots.empty(), "HandlePool -- No more free slots available");

			// Get the next free slot that is open from the back
			uint32_t nextFreeslot = mFreeSlots.back();
			//Remove a Free slot
			mFreeSlots.pop_back();
			
			// Set entry instance pointer
			//Give all the ownership of your data to another instance
			mEntries[nextFreeslot].instance = std::move(instance);
			 
			//Return a handle to this entry (set index and generation)
			HandleType h;
			h.mIndex = nextFreeslot;
			h.mGeneration = mEntries[nextFreeslot].generation;

			return h;
		}

		void Unregister(HandleType handle)
		{		
			// Skip if handle is invalid
			if (!handle.IsValid())
				return;

			// Find the entry and increment the generation (this invalidates all existing to this slot) 
			mEntries[handle.mIndex].generation++;
			mFreeSlots.push_back(handle.mIndex);			
			// Recycle the slot (add the index back to mFreeSlots) 
		}

		//Go through all the generations and increment them so none of the handles in the previous
		//generations aren't valid
		void Flush()
		{
			// Loop thru all entries and increment generation (invalidates all existing handles) 
			for (size_t i = 0; i < mEntries.size(); ++i)
			{
				mEntries[i].generation++;
				mFreeSlots.push_back(i + 1);
			}
			
		}

		bool IsValid(HandleType handle) const
		{
			//The handle cannot be invalid 0,0
			return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
		}
		DataType* Get(HandleType handle)
		{
			return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
		}

	private:
		struct Entry
		{
			DataType* instance = nullptr; 
			uint32_t generation = 0; 
		};

		std::vector<Entry> mEntries; 
		std::vector<size_t> mFreeSlots; 
	};
}