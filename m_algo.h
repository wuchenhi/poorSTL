#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

// poorstl 的基本算法

#include <cstring>
#include <type_traits>

#include <cstddef>

#include "m_iterator.h"
#include "m_allocator.h"

namespace poorstl{

/*****************************************************************************************/
// max
// 取二者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/
template <class T>
const T& max(const T& lhs, const T& rhs)
{
    return lhs < rhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp)
{
    return comp(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// min 
// 取二者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/
template <class T>
const T& min(const T& lhs, const T& rhs)
{
    return rhs < lhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare comp)
{
    return comp(rhs, lhs) ? rhs : lhs;
}


// equal，比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
template <class InputIter1, class InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (*first1 != *first2)  
            return false;
    }
    return true;
}

/*****************************************************************************************/
// copy_n
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部
/*****************************************************************************************/
template <class InputIter, class Size, class OutputIter>
std::pair<InputIter, OutputIter>
unchecked_copy_n(InputIter first, Size n, OutputIter result, poorstl::input_iterator_tag)
{
  for (; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return std::pair<InputIter, OutputIter>(first, result);
}

template <class RandomIter, class Size, class OutputIter>
std::pair<RandomIter, OutputIter>
unchecked_copy_n(RandomIter first, Size n, OutputIter result, 
                 poorstl::random_access_iterator_tag)
{
  auto last = first + n;
  return std::pair<RandomIter, OutputIter>(last, std::copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
std::pair<InputIter, OutputIter> 
copy_n(InputIter first, Size n, OutputIter result)
{
  return unchecked_copy_n(first, n, result, iterator_category(first));
}

// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
// input_iterator_tag 版本
template <class InputIter, class OutputIter>
OutputIter 
copy_tag(InputIter first, InputIter last, OutputIter result, 
                   poorstl::input_iterator_tag)
{
    for (; first != last; ++first, ++result)
    {
        *result = *first;
    }
    return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter 
copy_tag(RandomIter first, RandomIter last, OutputIter result,
                   poorstl::random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
        *result = *first;
  }
  return result;
}

// input_iterator_tag 版本
template <class InputIter, class OutputIter>
OutputIter 
copy_tag_backward(InputIter first, InputIter last, OutputIter result, 
                   poorstl::input_iterator_tag)
{
    for (auto n = last -first; n>0; --n)
    {
        *--result = *--last;
    }
    return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter 
copy_tag_backward(RandomIter first, RandomIter last, OutputIter result,
                   poorstl::random_access_iterator_tag)
{
    while (first != last)
        *--result = *--last;
    return result;
}

template <class InputIter, class OutputIter>
OutputIter copy_backward(InputIter first, InputIter last, OutputIter result)
{
    return copy_tag_backward(first, last, result, iterator_category(first));
}

template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
    return copy_tag(first, last, result, iterator_category(first));
}



// move
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

// forward
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

// swap
template <class Tp>
void swap(Tp& lhs, Tp& rhs)
{
    auto tmp(poorstl::move(lhs));
    lhs = poorstl::move(rhs);
    rhs = poorstl::move(tmp);
}
/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
    {
        *first = value;
    }
    return first;
}

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    return unchecked_fill_n(first, n, value);
}

/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T& value,
              poorstl::forward_iterator_tag)
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first, RandomIter last, const T& value,
              poorstl::random_access_iterator_tag)
{
    fill_n(first, last - first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value)
{
    fill_cat(first, last, value, iterator_category(first));
}
/*****************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compred>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2, Compred comp)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

// 针对 const unsigned char* 的特化版本
bool lexicographical_compare(const unsigned char* first1,
                             const unsigned char* last1,
                             const unsigned char* first2,
                             const unsigned char* last2)
{
    const auto len1 = last1 - first1;
    const auto len2 = last2 - first2;
    // 先比较相同长度的部分
    const auto result = std::memcmp(first1, first2, poorstl::min(len1, len2));
    // 若相等，长度较长的比较大
    return result != 0 ? result < 0 : len1 < len2;
}

}
#endif