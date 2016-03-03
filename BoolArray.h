#pragma once
#include "Defines.h"
#include <vector>
#include <stddef.h>

class BoolArray
{
public:
	BoolArray(size_t size, bool value = false);
	BoolArray() : BoolArray(0) {}
	~BoolArray();

	void			Set(const size_t& i, bool value);
	bool			Get(const size_t& i) const;
	void			SetRange(const size_t& start, const size_t& end, bool value);
	bool			Any(const size_t& start = 0, size_t end = ~size_t(0)) const;
	size_t			Size() const { return mSize; }
	bool			Empty() const { return mSize == 0; }
	void			Resize(const size_t& size);
	void			Clear();
	void			ShrinkToFit();
	
	bool			operator[](const size_t& i) const;
private:
	static inline size_t GetByteIndex(const size_t& i) { return i >> 3; }
	static inline unsigned8 GetBitIndex(const size_t& i) { return i & 0x7; }

	size_t						mSize;
	std::vector<unsigned8>		mData;
};

