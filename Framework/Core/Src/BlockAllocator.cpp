#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace MAGE;
using namespace MAGE::Core;

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	: mData(nullptr)
	, mBlockSize(blockSize)
	, mCapacity(capacity)
{
	ASSERT(capacity > 0, "BlockAllocator -- Invalid capacity.");
	//Remove any elements or data preexisting
	mFreeSlots.clear();
	// Allocate a chunk of memory but leave it unitialized
	mFreeSlots.reserve(capacity);
	//Allocate a block of size mCapacity * mBlocksize (ie 16 * 1 = 16)
	//and returns that pointer to the beginning of the block in the form of void*
	mData = reinterpret_cast<uint8_t*>(malloc(mCapacity * mBlockSize));
	
	for (size_t i = 0; i < mCapacity; i++)
	{
		//Creates how many rooms are availabe
		//Ie if capacity is 10 then we create 10 slots of free rooms
		mFreeSlots.push_back(i);
	}
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
	mData = nullptr;
	//No more rooms, hotel doesn't exist anymore 
	mFreeSlots.clear();
}

// Check into the room
void* BlockAllocator::Allocate()
{
	//Check to see if there are any rooms availabe if not then just return a nullptr
	//because we are booked
	if (mFreeSlots.empty())
		return nullptr;

	int availableSlots = mFreeSlots.back();
	
	// Math: int* num = 1000; num += 1 == 1004; 
	// Ie AllBlock Test you are allocating 10 rooms
	// the address of mData is incremented by the number of available slots * the block size
	// |____|				|____|					|____| 
	//	&mData + 0 * 16								&mData + 2 * 16
	void* newData = reinterpret_cast<uint8_t*>(mData) + availableSlots * mBlockSize;
	//The free slots are the number of keys now that we have assigned a room
	//we are down one room so popback removes one "key" or "room" from the list.
	mFreeSlots.pop_back();
	return newData;

}

//Check out of the room
void BlockAllocator::Free(void* ptr)
{
	//ptr is already null so the room has been freed already return
	if (!ptr)
		return;

	// Math to free up the data from the ptr in memory then push back that memory
	//onto the mfreeslots vector, now it there is another room open. 
	auto Slot = (&ptr - mData) / mBlockSize;
	mFreeSlots.push_back(Slot);
}
