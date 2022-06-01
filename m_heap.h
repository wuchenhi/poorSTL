#ifndef m_HEAP_H
#define m_HEAP_H

#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"


namespace poorstl
{
/*****************************************************************************************/
// push_heap 函数接受两个迭代器，表示一个 heap 的首尾，并且新元素已经插入到底部容器的最尾端

//实际完成插入的函数
template <class RandomIter, class Distance, class T>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value)
{
    auto parent = (holeIndex - 1) / 2; //父节点
    while (holeIndex > topIndex && *(first + parent) < value)  //*(first + parent) < value  可见是max-heap
    {
        // 使用 operator<，所以 heap 为 max-heap
        *(first + holeIndex) = *(first + parent);  //洞值为父节点值
        holeIndex = parent;                        //percolate up 上溯
        parent = (holeIndex - 1) / 2;              //新的父节点
    }
    *(first + holeIndex) = value;                  //完成插入
}

template <class RandomIter, class Distance>
void push_heap_distance(RandomIter first, RandomIter last, Distance*)
{
    //(last - first) - 1) 容器最尾端的洞序  0为topIndex  *(last - 1)为value
    poorstl::push_heap_aux(first, static_cast<Distance>((last - first) - 1), static_cast<Distance>(0), *(last - 1));
}

template <class RandomIter>
void push_heap(RandomIter first, RandomIter last)
{   // 新元素应该已置于底部容器的最尾端
    poorstl::push_heap_distance(first, last, distance_type(first));
}

/*****************************************************************************************/
// pop_heap 函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点放到容器尾部（注意，尚未取出）

//实际操作
template <class RandomIter, class T, class Distance>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
{
    // 先percolate down(下溯)
    auto topIndex = holeIndex;
    auto rChild = 2 * holeIndex + 2;
    while (rChild < len)
    {
        //比较左右字节点，取较大的
        if (*(first + rChild) < *(first + rChild - 1))
            --rChild;
        *(first + holeIndex) = *(first + rChild);
        holeIndex = rChild;
        rChild = 2 * (rChild + 1);
    }
    if (rChild == len)
    {  // 只有左子节点，没有右子节点
        *(first + holeIndex) = *(first + (rChild - 1));
        holeIndex = rChild - 1;
    }
    // 尚未满足次序特性，再执行一次percolate up（上溯）
    poorstl::push_heap_aux(first, holeIndex, topIndex, value); 
}

template <class RandomIter, class T, class Distance>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                  Distance*)
{
    //首值调至尾节点（所求值），调用底层容器的pop_back取出，然后调整[first, last - 1)使之重新成为一个 max-heap
    *result = *first;
    poorstl::adjust_heap(first, static_cast<Distance>(0), last - first, value); //0为树根， value为调整值（原尾值）
}

template <class RandomIter>
void pop_heap(RandomIter first, RandomIter last)
{
    poorstl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
}

/*****************************************************************************************/
// sort_heap 函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
// sort_heap之后pop不合法
template <class RandomIter>
void sort_heap(RandomIter first, RandomIter last)
{
  // 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
  while (last - first > 1)
  {
        poorstl::pop_heap(first, last--);
  }
}

/*****************************************************************************************/
// make_heap 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap

template <class RandomIter, class Distance>
void make_heap_aux(RandomIter first, RandomIter last, Distance*)
{
    if (last - first < 2)
        return;
    //第一个需要重排的子树头部
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true)
    {
        // 重排以 holeIndex 为首的子树
        poorstl::adjust_heap(first, holeIndex, len, *(first + holeIndex));//adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
        if (holeIndex == 0)
            return;
        --holeIndex;
    }
}

template <class RandomIter>
void make_heap(RandomIter first, RandomIter last)
{
    poorstl::make_heap_aux(first, last, distance_type(first));;
}

}

#endif