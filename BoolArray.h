#pragma once
#include "Defines.h"
#include "BitHelper.h"
#include <vector>
#include <stddef.h>

template<typename T = unsigned8>
class BoolArray
{
	const static unsigned8	WORD_SIZE			= sizeof(T) * 8;
	const static T			BIT_INDEX_MASK		= WORD_SIZE - 1;
	const static unsigned8	BYTE_INDEX_SHIFT	= BitHelper::Log2(WORD_SIZE);
	const static T			ALL_SET				= BitHelper::GetAllSet<T>();

public:
	BoolArray(size_t size, bool value = false) :
		mData(std::vector<T>(GetWordIndex(size - 1) + 1, GetAllSet(value))),
		mSize(size)
	{}

	BoolArray() : BoolArray(0) {}
	~BoolArray() {}

	inline void	Set(const size_t& i, bool value)
	{
		if (value) mData[GetWordIndex(i)] |= GetBitMask(GetBitIndex(i)); // Set the bit
		else mData[GetWordIndex(i)] &= ~GetBitMask(GetBitIndex(i)); // Clear the bit
	}

	inline bool Get(const size_t& i) const
	{
		return BitHelper::GetLS(mData[GetWordIndex(i)], GetBitIndex(i));
	}

	void SetRange(const size_t& start, const size_t& end, bool value)
	{
		if (end < start) return;
		if (end > mSize) Resize(end);

		size_t startByteIndex = GetWordIndex(start);
		size_t endByteIndex = GetWordIndex(end);

		T bitMask = BitHelper::GetLSMask<T>(GetBitIndex(start), WORD_SIZE);
		if (startByteIndex == endByteIndex)
			bitMask = BitHelper::GetLSMask<T>(GetBitIndex(start), GetBitIndex(end));

		// Set the bits in the first block:
		mData[startByteIndex] &= ~bitMask;
		if (value) mData[startByteIndex] |= bitMask;
		if (startByteIndex == endByteIndex) return; // We're done if only one byte is affected

		// Set the bits in all bytes between the start and the end bytes
		T allSet = GetAllSet(value);
		for (size_t byte = startByteIndex + 1; byte < endByteIndex; byte++)
			mData[byte] = allSet;

		// Set the required bits in the end byte
		if (GetBitIndex(end) > 0)
		{
			bitMask = BitHelper::GetLSMask<unsigned8>(0, GetBitIndex(end));
			mData[endByteIndex] &= ~bitMask;
			if (value) mData[endByteIndex] |= bitMask;
		}
	}

	bool Any(const size_t& start = 0, size_t end = ~size_t(0)) const
	{
		if (end >= mSize) end = mSize - 1;
		if (end <= start) return false;

		size_t startByteIndex = GetWordIndex(start);
		size_t endByteIndex = GetWordIndex(end);

		if (startByteIndex == endByteIndex)
			return (BitHelper::GetLSMask<T>(GetBitIndex(start), GetBitIndex(end)) & mData[startByteIndex]) != 0;

		// Check the first block
		T bitMask = BitHelper::GetLSMask<T>(GetBitIndex(start), WORD_SIZE);
		if ((mData[startByteIndex] & bitMask) != 0) return true;

		// Check the bytes inbetween
		for (size_t byte = startByteIndex + 1; byte < endByteIndex; byte++)
			if ((mData[byte] & ALL_SET) != 0) return true;

		// Check the last bits
		return (mData[endByteIndex] & BitHelper::GetLSMask<T>(0, GetBitIndex(end))) != 0;
	}

	inline size_t Size() const { return mSize; }
	inline bool Empty() const { return mSize == 0; }
	
	void Resize(const size_t& size)
	{
		mSize = size;
		mData.resize(GetWordIndex(size - 1) + 1);
	}

	void Clear()
	{
		mSize = 0;
		mData.clear();
		mData.shrink_to_fit();
	}

	void			ShrinkToFit()								{ mData.shrink_to_fit(); }
	bool			operator[](const size_t& i) const			{ return Get(i); }

private:
	static constexpr size_t		GetWordIndex(const size_t& i)	{ return i >> BYTE_INDEX_SHIFT; }
	static constexpr unsigned8	GetBitIndex(const size_t& i)	{ return i & BIT_INDEX_MASK; }
	static constexpr T			GetBitMask(const unsigned8 b)	{ return BitHelper::GetLSSingleBitMask<T>(b); }
	static constexpr T			GetAllSet(bool value)			{ return value ? ALL_SET : 0; }

	size_t						mSize;
	std::vector<T>				mData;
};

