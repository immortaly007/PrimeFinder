#include "BoolArray.h"
#include "BitHelper.h"
#include <assert.h>

BoolArray::BoolArray(size_t size, bool value) :
	mData(std::vector<unsigned8>(BitHelper::RoundToBytes(size) / 8, value ? 255 : 0)),
	mSize(size)
{}

BoolArray::~BoolArray() {}

void BoolArray::Set(const size_t& i, bool value)
{
	size_t byteIndex = GetByteIndex(i);
	unsigned8 bitIndex = GetBitIndex(i);
	unsigned8 bitMask = BitHelper::GetHSSingleBitMask<unsigned8>(bitIndex);
	mData[byteIndex] &= ~bitMask; // Clear the bit
	if (value) mData[byteIndex] |= bitMask; // Set it if needed
}

void BoolArray::SetRange(const size_t& start, const size_t& end, bool value)
{
	if (end < start) return;
	if (end > mSize) Resize(end);

	size_t startByteIndex = GetByteIndex(start);
	size_t endByteIndex = GetByteIndex(end);

	unsigned8 bitMask = BitHelper::GetHSMask<unsigned8>(GetBitIndex(start), 8);
	if (startByteIndex == endByteIndex)
		bitMask = BitHelper::GetHSMask<unsigned8>(GetBitIndex(start), GetBitIndex(end));
	// Set the bits in the first block:
	mData[startByteIndex] &= ~bitMask;
	if (value) mData[startByteIndex] |= bitMask;
	if (startByteIndex == endByteIndex) return; // We're done if only one byte is affected

	// Set the bits in all bytes between the start and the end bytes
	unsigned8 allSetValue = value ? 0xFF : 0x00;
	for (size_t byte = startByteIndex + 1; byte < endByteIndex; byte++)
		mData[byte] = allSetValue;

	// Set the required bits in the end byte
	if (end % 8 != 0)
	{
		bitMask = BitHelper::GetHSMask<unsigned8>(0, GetBitIndex(end));
		mData[endByteIndex] &= ~bitMask;
		if (value) mData[endByteIndex] |= bitMask;
	}
}

bool BoolArray::Any(const size_t& start, size_t end) const
{
	if (end >= mSize) end = mSize - 1;
	if (end <= start) return false;

	size_t startByteIndex = GetByteIndex(start);
	size_t endByteIndex = GetByteIndex(end);

	if (startByteIndex == endByteIndex)
		return (BitHelper::GetHSMask<unsigned8>(GetBitIndex(start), GetBitIndex(end)) & mData[startByteIndex]) != 0;

	// Check the first block
	unsigned8 bitMask = BitHelper::GetHSMask<unsigned8>(GetBitIndex(start), 8);
	if ((mData[startByteIndex] & bitMask) != 0) return true;

	// Check the bytes inbetween
	for (size_t byte = startByteIndex + 1; byte < endByteIndex; byte++)
		if ((mData[byte] & 0xFF) != 0) return true;

	// Check the last bits
	return (mData[endByteIndex] & BitHelper::GetHSMask<unsigned8>(0, GetBitIndex(end))) != 0;
}

bool BoolArray::Get(const size_t& i) const
{
	assert(i < mSize);
	return BitHelper::GetHS(mData[GetByteIndex(i)], GetBitIndex(i));
}
void BoolArray::Resize(const size_t& size)
{
	mSize = size;
	mData.resize(BitHelper::RoundToBytes(size) / 8);
}

void BoolArray::Clear()
{
	mSize = 0;
	mData.clear();
	mData.shrink_to_fit();
}

void BoolArray::ShrinkToFit()
{
	mData.shrink_to_fit();
}

bool BoolArray::operator[](const size_t& i) const
{
	return Get(i);
}