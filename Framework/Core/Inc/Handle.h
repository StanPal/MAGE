#pragma once

#include "Common.h"

namespace MAGE::Core
{
	template<class DataType>
	class HandlePool;

	template <class DataType>
	class Handle
	{
	public:
		Handle() 
			: mIndex(0), // slot 0 will be unused, in other words handle at 0 is an invalidate handle
			mGeneration(0)
		{}
		bool IsValid() const
		{
			return sPool && sPool->IsValid(*this);
		}
		void Invalidate()
		{
			//Making yourself null
			*this = Handle();
		}

		DataType* Get() const
		{
			return sPool ? sPool->Get(*this) : nullptr;
		}

		DataType* operator->() const
		{
			return sPool ? sPool->Get(*this) : nullptr; 
		}

		DataType& operator*() const
		{
			return *(sPool->Get(*this));
		}

		bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
		bool operator!=(Handle rhs) const { return !(*this == rhs); }

	private:
		friend class HandlePool<DataType>; 

		//(*Trick*)Internal Singleton, the handle will talk to the corresponding HandlePool
		static HandlePool<DataType>* sPool;

		//Index of ticket to the handle pool slot
		//Ie Movie Ticket Seat 3
		uint32_t mIndex : 16; // bit packing (Tell the compiler that when you use 
		//the number use it as a 32bit number, when you store in memory store it as a
		//16 bit number) If this number is only 16 bit, you can pack them into one row.
		//Another condition of the handle aka Movie Ticket
		//Ie What time is the Ticket valid for
		uint32_t mGeneration : 16; 

		//First row is for mIndex, Second Row is mGeneration so when you pull it out
		//It will be a 32 bit integer
		//sizeof(Handle) == 4 bytes == sizeof(void*)
	};

	//This is how you initialize a static
	template <class DataType>
	HandlePool<DataType>* Handle<DataType>::sPool = nullptr; 

}