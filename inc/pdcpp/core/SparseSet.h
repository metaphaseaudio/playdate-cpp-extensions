/**
	SparseSet.h
	Implements a class template of a sparse
	set of unsigned integers.
	@author Sam Griffiths
	www.computist.xyz
*/

#pragma once

#include <vector>
#include <type_traits>

namespace pdcpp
{
    template <typename T>
    class SparseSet
    {
        static_assert(std::is_unsigned<T>::value, "SparseSet can only contain unsigned integers");

    public:
        SparseSet() = default;

        using iterator       = typename std::vector<T>::const_iterator;
        using const_iterator = typename std::vector<T>::const_iterator;

        iterator begin() 		{ return dense.begin(); }
        const_iterator begin() const 	{ return dense.begin(); }

        iterator end() 			{ return dense.begin() + size_; }
        const_iterator end() const 	{ return dense.begin() + size_; }

        size_t size() const 		{ return size_; }
        size_t capacity() const		{ return capacity_; }

        bool empty() const 		{ return size_ == 0; }

        void clear() 			{ size_ = 0; }

        void reserve(size_t u)
        {
            if (u > capacity_)
            {
                dense.resize(u, 0);
                sparse.resize(u, 0);
                capacity_ = u;
            }
        }

        bool has(const T &val) const
        {
            return val < capacity_ &&
                   sparse[val] < size_ &&
                   dense[sparse[val]] == val;
        }

        void insert(const T &val)
        {
            if (!has(val))
            {
                if (val >= capacity_)
                    reserve(val + 1);

                dense[size_] = val;
                sparse[val] = size_;
                ++size_;
            }
        }

        void erase(const T &val)
        {
            if (has(val))
            {
                dense[sparse[val]] = dense[size_ - 1];
                sparse[dense[size_ - 1]] = sparse[val];
                --size_;
            }
        }

        T operator[] (int index) const { return dense[index]; }

    private:
        std::vector<T> dense;	//Dense set of elements
        std::vector<T> sparse;	//Map of elements to dense set indices

        size_t size_ = 0;	//Current size (number of elements)
        size_t capacity_ = 0;	//Current capacity (maximum value + 1)
    };
}
