#pragma once
#include "Defines.h"
#include "BitHelper.h"
#include <vector>
#include <stddef.h>

class BoolArray
{
public:
	BoolArray(size_t size, bool value = false);
	BoolArray() : BoolArray(0) {}
	~BoolArray();

	inline void	Set(const size_t& i, bool value) 
	{
		if (value) mData[GetByteIndex(i)] |= GetBitMask(GetBitIndex(i)); // Set the bit
		else mData[GetByteIndex(i)] &= ~GetBitMask(GetBitIndex(i)); // Clear the bit
	}

	inline bool Get(const size_t& i) const
	{
		return BitHelper::GetLS(mData[GetByteIndex(i)], GetBitIndex(i));
	}

	void			SetRange(const size_t& start, const size_t& end, bool value);
	bool			Any(const size_t& start = 0, size_t end = ~size_t(0)) const;
	size_t			Size() const { return mSize; }
	bool			Empty() const { return mSize == 0; }
	void			Resize(const size_t& size);
	void			Clear();
	void			ShrinkToFit();
	
	bool			operator[](const size_t& i) const { return Get(i); }
private:
	static constexpr size_t GetByteIndex(const size_t& i) { return i >> 3; }
	static constexpr unsigned8 GetBitIndex(const size_t& i) { return i & 0x7; }
	static constexpr unsigned8 GetBitMask(const unsigned8 b) { return BitHelper::GetLSSingleBitMask<unsigned8>(b); }

	size_t						mSize;
	std::vector<unsigned8>		mData;
};

