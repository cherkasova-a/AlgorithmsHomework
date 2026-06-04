#ifndef SORTING_H
#define SORTING_H

#include "collvalue.h"

template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end || begin + 1 == end) return;

    for (Iterator i = begin; i != end; ++i)
    {
        for (Iterator j = begin; j < end - 1; ++j)
        {
            if (*(j + 1) < *j)
            {
                using std::swap;
                swap(*j, *(j + 1));
            }
        }
    }
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    if (begin == end || begin + 1 == end) return;

    auto pivot = *(begin + (end - begin) / 2);
    Iterator left = begin;
    Iterator right = end - 1;

    while (left <= right)
    {
        while (*left < pivot) ++left;
        while (pivot < *right) --right;

        if (left <= right)
        {
            using std::swap;
            swap(*left, *right);
            ++left;
            --right;
        }
    }

    quick_sort(begin, right + 1);
    quick_sort(left, end);
}

#endif // SORTING_H
