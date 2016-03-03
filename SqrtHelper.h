// Class used to calculate the square root of an integer (with corrections)
// Based on a post by Linoliumz in: http://codereview.stackexchange.com/questions/69069/computing-the-square-root-of-a-64-bit-integer
class SqrtHelper
{
private:
	template <typename T>
	static T sqrt_helper(T x, T lo, T hi)
	{
		if (lo == hi)
			return lo;

		const T mid = (lo + hi + 1) / 2;
		if (x / mid < mid)
			return sqrt_helper<T>(x, lo, mid - 1);
		else
			return sqrt_helper(x, mid, hi);
	}

	template <typename T>
	static T ct_sqrt(T x)
	{
		return sqrt_helper<T>(x, 0, x / 2 + 1);
	}

public:
	template <typename T>
	static T isqrt(T x)
	{
		T r = (T)std::sqrt((double)x);
		T sqrt_max = ct_sqrt(std::numeric_limits<T>::max());

		while (r < sqrt_max && r * r < x)
			r++;
		while (r > sqrt_max || r * r > x)
			r--;

		return r;
	}
};