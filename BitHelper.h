#pragma once
#include "Defines.h"
#include "CompileTimeArray.h"
#include <vector>
#include <assert.h>

template<size_t x> struct BitCount
{
	// The line below actually counts the number of bits set in x
	enum {
		value = (((((x & 0x55) + ((x >> 1) & 0x55)) & 0x33) + ((((x & 0x55) + ((x >> 1) & 0x55)) >> 2) & 0x33)) & 0x0F) +
		((((((x & 0x55) + ((x >> 1) & 0x55)) & 0x33) + ((((x & 0x55) + ((x >> 1) & 0x55)) >> 2) & 0x33)) >> 4) & 0x0F)
	};
};

typedef generate_array<256, BitCount>::result BitCountLookupTable;

namespace BitHelper
{
	inline static unsigned8 GetSet(unsigned64 x)
	{
		x = (x & 0x5555555555555555) + ((x >> 1) & 0x5555555555555555); // 0x55 = 01010101
		x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333); // 0x33 = 00110011
		x = (x & 0x0F0F0F0F0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F0F0F0F0F); // 0x0F = 00001111
		x = (x & 0x00FF00FF00FF00FF) + ((x >> 8) & 0x00FF00FF00FF00FF);
		x = (x & 0x0000FFFF0000FFFF) + ((x >> 16) & 0x0000FFFF0000FFFF);
		x = (x & 0x00000000FFFFFFFF) + ((x >> 32) & 0x00000000FFFFFFFF);
		return (unsigned8)x;
	}

	inline static unsigned8 GetSet(unsigned32 v)
	{
		// From https://graphics.stanford.edu/~seander/bithacks.html, method only uses 12 operations :)
		v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
		v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
		return (unsigned8)((((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24); // count

		//x -= ((x >> 1) & 0x55555555);
		//x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
		//x = (((x >> 4) + x) & 0x0f0f0f0f);
		//x += (x >> 8);
		//x += (x >> 16);
		//return (x & 0x0000003f);
	}

	inline static unsigned8 GetSet(unsigned16 x)
	{
		x = (x & 0x5555) + ((x >> 1) & 0x5555); // 0x55 = 01010101
		x = (x & 0x3333) + ((x >> 2) & 0x3333); // 0x33 = 00110011
		x = (x & 0x0F0F) + ((x >> 4) & 0x0F0F); // 0x0F = 00001111
		x = (x & 0x00FF) + ((x >> 8) & 0x00FF);
		return (unsigned8)x;
	}

	constexpr static unsigned8 GetSet(unsigned8 x)
	{
		return BitCountLookupTable::data[x];
		//// Parallel bit count
		//x = (x & 0x55) + ((x >> 1) & 0x55); // 0x55 = 01010101
		//x = (x & 0x33) + ((x >> 2) & 0x33); // 0x33 = 00110011
		//x = (x & 0x0F) + ((x >> 4) & 0x0F); // 0x0F = 00001111
		//return x;
	}

	template<typename T>
	inline T GetLSMaskUntil(const unsigned8 bit) { return (~((T)0)) << bit; }

	template<typename T> constexpr T GetLSSingleBitMask(const unsigned8 bit) { return (T)1 << bit; }
	template<typename T, unsigned8 bitsInT = sizeof(T) * 8> constexpr T GetHSSingleBitMask(const unsigned8 bit) { return (T)1 << (bitsInT - 1 - bit); }
	// Template specialization for HSSingleBitMask
	template<> constexpr unsigned8  GetHSSingleBitMask(const unsigned8 pos) { return (unsigned8)0x80 >> pos; }
	template<> constexpr unsigned16 GetHSSingleBitMask(const unsigned8 pos) { return (unsigned16)0x8000 >> pos; }
	template<> constexpr unsigned32 GetHSSingleBitMask(const unsigned8 pos) { return (unsigned32)0x80000000 >> pos; }
	template<> constexpr unsigned64 GetHSSingleBitMask(const unsigned8 pos) { return (unsigned64)0x8000000000000000 >> pos; }

	template<typename T, unsigned8 bitsInT = sizeof(T) * 8>
	inline T GetLSMask(const unsigned8 startIndex, const unsigned8 endIndex) {
		if (startIndex == endIndex) return (T)0;
		// Note that s needs to be defined first. If this happens in the same line, I couldn't get the compiler to make sure the size of the datatype is correct
		T s = ~((T)0);
		unsigned8 firstShift = bitsInT - endIndex;
		s <<= firstShift;
		s >>= firstShift + startIndex;
		s <<= startIndex;
		return s;
	}

	template<typename T, unsigned8 bitsInT = sizeof(T) * 8>
	inline T GetHSMask(const unsigned8 startIndex, const unsigned8 endIndex) {
		if (startIndex == endIndex) return (T)0;
		// Note that s needs to be defined first. If this happens in the same line, I couldn't get the compiler to make sure the size of the datatype is correct
		T s = ~((T)0);
		unsigned8 firstShift = bitsInT - endIndex;
		s >>= firstShift;
		s <<= firstShift + startIndex;
		s >>= startIndex;
		return s;
	}

	/* Returns the opposite of 0 for some type T (i.e. for unsigned8 will return 0xFF, for unsigned32 will return 0xFFFFFFFF */
	template<typename T> constexpr T GetAllSet() { return ~((T)0); }

	template<typename T, unsigned8 bitsInT = sizeof(T) * 8>
	inline unsigned8 GetHSSetBefore(const T value, const unsigned8 pos) { return GetSet((T)(value << (bitsInT - pos))); }

	template<typename T> constexpr bool GetHS(const T value, const unsigned8 pos) { return (GetHSSingleBitMask<T>(pos) & value) != 0; }
	template<typename T> constexpr bool GetLS(const T value, const unsigned8 pos) { return (BitHelper::GetLSSingleBitMask<T>(pos) & value) != 0; }

	template<typename T> constexpr T SetLS(const T value, const T& setMask, const T& setValue) { return (value & ~setMask) | (setMask & setValue); }

	template<typename T> constexpr void SetLS(T& value, const unsigned8 pos) { value |= GetLSSingleBitMask<T>(pos); }
	template<typename T> constexpr void SetHS(T& value, const unsigned8 pos) { value |= GetHSSingleBitMask<T>(pos); }

	template<typename T> constexpr void SetLS(T& value, const unsigned8 pos, const bool& set) {
		T bitmask = GetLSSingleBitMask<T>(pos);
		value &= ~bitmask; // Clear the bit
		if (set) value |= bitmask; // Set it if necessary
	}
	template<typename T> constexpr void SetHS(T& value, const unsigned8 pos, const bool& set)
	{
		T bitmask = GetHSSingleBitMask<T>(pos);
		value &= ~bitmask;
		if (set) value |= bitmask;
	}

	inline unsigned32 CeilToNearestPowerOfTwo(unsigned32 v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}
	inline unsigned32 FloorToNearestPowerOfTwo(unsigned32 value)	{ return CeilToNearestPowerOfTwo(value) >> 1; }

	template<typename T>
	constexpr bool IsPowerOfTwo(T v) { return v && !(v & (v - 1)); }

	// Log2 = the index of the highest significant bit.
	template<typename T>
	constexpr unsigned8 Log2(T v) {
		return v <= 1 ? 0 : 1 + Log2(v >> 1);
	}

	template<typename T>
	constexpr unsigned8  Log2Ceil(T v) {
		return Log2(v) + (IsPowerOfTwo(v) ? 0 : 1); // If v is not a power of two, the number was rounded down.
	}

	// 2^x is equal to taking the number 1 and shifting it x indices.
	template<typename T>
	constexpr unsigned64 Exp2(const T& v) { return ((unsigned64)1) << v; }

	template<typename T>
	inline T RoundToBytes(const T& v)
	{
		if ((v & GetLSMask<T>(0, 3)) != 0) return ((v >> 3) + 1) << 3;
		else return v;
	}



	template<typename T, unsigned8 bitsInT = sizeof(T) * 8>
	constexpr T CircularShiftLeft(T v, T shift)  { shift %= bitsInT; return (v << shift) | (v >> (bitsInT - shift)); }

	template<typename T, unsigned8 bitsInT = sizeof(T) * 8>
	constexpr T CircularShiftRight(T v, T shift) { shift %= bitsInT; return (v >> shift) | (v << (bitsInT - shift)); }

	template<typename T>
	inline void SplitInBytesAndMove(T value, std::vector<unsigned8>& destination, size_t offset, size_t size = sizeof(T))
	{
		SplitInBytesAndMove<T>(value, &destination[0], offset, size);
	}

	template<typename T>
	inline void SplitInBytesAndMove(T value, unsigned8* destination, size_t offset, size_t size = sizeof(T))
	{
		for (size_t byte = 0; byte < size; byte++)
		{
			size_t lsByte = size - 1 - byte;
			destination[offset + byte] = (unsigned8)((GetLSMask<T>((unsigned8)(lsByte * 8), (unsigned8)((lsByte + 1) * 8)) & value) >> (lsByte << 3));
		}
	}

	template<typename T>
	inline std::vector<unsigned8> SplitInBytes(T value, size_t size = sizeof(T))
	{
		std::vector<unsigned8> res(size);
		SplitInBytesAndMove(value, res, 0, size);
		return res;
	}

	template<typename T>
	inline void JoinBytes(const std::vector<unsigned8>& source, T& dest, size_t offset = 0, size_t size = sizeof(T)) { JoinBytes(&source[0], dest, offset, size); }

	template<typename T>
	inline void JoinBytes(const unsigned8* source, T& dest, size_t offset = 0, size_t size = sizeof(T))
	{
		for (size_t byte = 0; byte < size; byte++)
			dest |= source[offset + byte] << ((size - byte - 1) * 8);
	}

	template<typename T>
	inline void JoinBytesLittleEndian(const std::vector<unsigned8>& source, T& dest, size_t offset = 0, size_t size = sizeof(T)) { JoinBytes(&source[0], dest, offset, size); }

	template<typename T>
	inline void JoinBytesLittleEndian(const unsigned8* source, T& dest, size_t offset = 0, size_t size = sizeof(T))
	{
		for (size_t byte = 0; byte < size; byte++)
			dest |= source[offset + (size - byte - 1)] << ((size - byte - 1) * 8);
	}
};

