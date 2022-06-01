#ifndef m_UNINITIALIZED_H_
#define m_UNINITIALIZED_H_

// 作用：在未初始化空间构造元素

#include "m_algo.h"
#include "m_allocator.h"
#include "m_iterator.h"

namespace poorstl
{

// uninit_copy
// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return poorstl::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    auto cur = result;
    try
    {
        for (; first != last; ++first, ++cur)
        {
            poorstl::_construct(&*cur, *first);
        }
    }
    catch (...)
    {
        for (; result != cur; --cur)
            poorstl::_destroy(&*cur);
    }
    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninit_copy(InputIter first, InputIter last, ForwardIter result)
{
    return poorstl::unchecked_uninit_copy(first, last, result, 
                                            std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>:: value_type>{});//如果类型 Ty 是具有普通复制赋值运算符的类，则类型谓词的实例为 true；否则为 false。
}

/*****************************************************************************************/
// uninitialized_copy_n
// 把 [first, first + n) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class InputIter, class Size, class ForwardIter>
ForwardIter 
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::true_type)
{
  return poorstl::copy_n(first, n, result).second;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::false_type)
{
  auto cur = result;
  try
  {
    for (; n > 0; --n, ++cur, ++first)
    {
      poorstl::_construct(&*cur, *first);
    }
  }
  catch (...)
  {
    for (; result != cur; --cur)
      poorstl::_destroy(&*cur);
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result)
{
  return poorstl::unchecked_uninit_copy_n(first, n, result,
                                        std::is_trivially_copy_assignable<
                                        typename iterator_traits<InputIter>::
                                        value_type>{});
}
/*****************************************************************************************/
// fill_uninit
// 在 [first, last) 区间内填充元素值
/*****************************************************************************************/
template <class ForwardIter, class T>
void 
unchecked_fill_uninit(ForwardIter first, ForwardIter last, const T& value, std::true_type)
{
    poorstl::fill(first, last, value);
}

template <class ForwardIter, class T>
void 
unchecked_fill_uninit(ForwardIter first, ForwardIter last, const T& value, std::false_type)
{
    auto cur = first;
    try
    {
        for (; cur != last; ++cur)
        {
            poorstl::_construct(&*cur, value);
        }
    }
    catch (...)
    {
        for (;first != cur; ++first)
            poorstl::_destroy(&*first);
    }
}

template <class ForwardIter, class T>
void  fill_uninit(ForwardIter first, ForwardIter last, const T& value)
{
    poorstl::unchecked_fill_uninit(first, last, value, 
                               std::is_trivially_copy_assignable<
                               typename iterator_traits<ForwardIter>::
                               value_type>{});  //标识 T 是否为平凡复制可分配类型的 Trait 类
}

/*****************************************************************************************/
// fill_uninit_n
// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
/*****************************************************************************************/
template <class ForwardIter, class Size, class T>
ForwardIter 
unchecked_fill_uninit_n(ForwardIter first, Size n, const T& value, std::true_type)
{
    return poorstl::fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter 
unchecked_fill_uninit_n(ForwardIter first, Size n, const T& value, std::false_type)
{
    auto cur = first;
    try
    {
        for (; n > 0; --n, ++cur)
        {
            poorstl::_construct(&*cur, value);
        }
    }
    catch (...)
    {
        for (; first != cur; ++first)
            poorstl::_destroy(&*first);
    }
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter fill_uninit_n(ForwardIter first, Size n, const T& value)
{
    return poorstl::unchecked_fill_uninit_n(first, n, value, 
                                        std::is_trivially_copy_assignable<
                                        typename iterator_traits<ForwardIter>::
                                        value_type>{});
}

/*****************************************************************************************/
// uninitialized_move
// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
  return poorstl::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
  ForwardIter cur = result;
  try
  {
    for (; first != last; ++first, ++cur)
    {
      poorstl::_construct(&*cur, poorstl::move(*first));
    }
  }
  catch (...)
  {
    poorstl::_destroy(result, cur);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
{
  return poorstl::unchecked_uninit_move(first, last, result,
                                      std::is_trivially_move_assignable<
                                      typename iterator_traits<InputIter>::
                                      value_type>{});
}

/*****************************************************************************************/
// uninitialized_move_n
// 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class InputIter, class Size, class ForwardIter>
ForwardIter 
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::true_type)
{
  return poorstl::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::false_type)
{
  auto cur = result;
  try
  {
    for (; n > 0; --n, ++first, ++cur)
    {
      poorstl::_construct(&*cur, poorstl::move(*first));
    }
  }
  catch (...)
  {
    for (; result != cur; ++result)
      poorstl::_destroy(&*result);
    throw;
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result)
{
  return poorstl::unchecked_uninit_move_n(first, n, result,
                                        std::is_trivially_move_assignable<
                                        typename iterator_traits<InputIter>::
                                        value_type>{});
}

} // namespace poorstl
#endif // !MYTINYSTL_UNINITIALIZED_H_

