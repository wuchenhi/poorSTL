#ifndef m_HEAP_H
#define m_HEAP_H

#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"


namespace poorstl
{
/*****************************************************************************************/
// push_heap
// 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
/*****************************************************************************************/
template <class RandomIter, class Distance, class T>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value)
{
  auto parent = (holeIndex - 1) / 2;
  while (holeIndex > topIndex && *(first + parent) < value)
  {
    // 使用 operator<，所以 heap 为 max-heap
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}

template <class RandomIter, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*)
{
  poorstl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
}

template <class RandomIter>
void push_heap(RandomIter first, RandomIter last)
{ // 新元素应该已置于底部容器的最尾端
  poorstl::push_heap_d(first, last, distance_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Distance, class T, class Compared>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value,
                   Compared comp)
{
  auto parent = (holeIndex - 1) / 2;
  while (holeIndex > topIndex && comp(*(first + parent), value))
  {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}

template <class RandomIter, class Compared, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp)
{
  poorstl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                       *(last - 1), comp);
}

template <class RandomIter, class Compared>
void push_heap(RandomIter first, RandomIter last, Compared comp)
{
  poorstl::push_heap_d(first, last, distance_type(first), comp);
}

/*****************************************************************************************/
// pop_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
/*****************************************************************************************/
template <class RandomIter, class T, class Distance>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
{
  // 先进行下溯(percolate down)过程
  auto topIndex = holeIndex;
  auto rchild = 2 * holeIndex + 2;
  while (rchild < len)
  {
    if (*(first + rchild) < *(first + rchild - 1))
      --rchild;
    *(first + holeIndex) = *(first + rchild);
    holeIndex = rchild;
    rchild = 2 * (rchild + 1);
  }
  if (rchild == len)
  {  // 如果没有右子节点
    *(first + holeIndex) = *(first + (rchild - 1));
    holeIndex = rchild - 1;
  }
  // 再执行一次上溯(percolate up)过程
  poorstl::push_heap_aux(first, holeIndex, topIndex, value);
}

template <class RandomIter, class T, class Distance>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
                  Distance*)
{
  // 先将首值调至尾节点，然后调整[first, last - 1)使之重新成为一个 max-heap
  *result = *first;
  poorstl::adjust_heap(first, static_cast<Distance>(0), last - first, value);
}

template <class RandomIter>
void pop_heap(RandomIter first, RandomIter last)
{
  poorstl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class T, class Distance, class Compared>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                 Compared comp)
{
  // 先进行下溯(percolate down)过程
  auto topIndex = holeIndex;
  auto rchild = 2 * holeIndex + 2;
  while (rchild < len)
  {
    if (comp(*(first + rchild), *(first + rchild - 1)))  --rchild;
    *(first + holeIndex) = *(first + rchild);
    holeIndex = rchild;
    rchild = 2 * (rchild + 1);
  }
  if (rchild == len)
  {
    *(first + holeIndex) = *(first + (rchild - 1));
    holeIndex = rchild - 1;
  }
  // 再执行一次上溯(percolate up)过程
  poorstl::push_heap_aux(first, holeIndex, topIndex, value, comp);
}

template <class RandomIter, class T, class Distance, class Compared>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, 
                  T value, Distance*, Compared comp)
{
  *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
  poorstl::adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
}

template <class RandomIter, class Compared>
void pop_heap(RandomIter first, RandomIter last, Compared comp)
{
  poorstl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
                      distance_type(first), comp);
}

/*****************************************************************************************/
// sort_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
/*****************************************************************************************/
template <class RandomIter>
void sort_heap(RandomIter first, RandomIter last)
{
  // 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
  while (last - first > 1)
  {
    poorstl::pop_heap(first, last--);
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compared>
void sort_heap(RandomIter first, RandomIter last, Compared comp)
{
  while (last - first > 1)
  {
    poorstl::pop_heap(first, last--, comp);
  }
}

/*****************************************************************************************/
// make_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
/*****************************************************************************************/
template <class RandomIter, class Distance>
void make_heap_aux(RandomIter first, RandomIter last, Distance*)
{
  if (last - first < 2)
    return;
  auto len = last - first;
  auto holeIndex = (len - 2) / 2;
  while (true)
  {
    // 重排以 holeIndex 为首的子树
    poorstl::adjust_heap(first, holeIndex, len, *(first + holeIndex));
    if (holeIndex == 0)
      return;
    holeIndex--;
  }
}

template <class RandomIter>
void make_heap(RandomIter first, RandomIter last)
{
  poorstl::make_heap_aux(first, last, distance_type(first));;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Distance, class Compared>
void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp)
{
  if (last - first < 2)
    return;
  auto len = last - first;
  auto holeIndex = (len - 2) / 2;
  while (true)
  {
    // 重排以 holeIndex 为首的子树
    poorstl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
    if (holeIndex == 0)
      return;
    holeIndex--;
  }
}

template <class RandomIter, class Compared>
void make_heap(RandomIter first, RandomIter last, Compared comp)
{
  poorstl::make_heap_aux(first, last, distance_type(first), comp);
}
}

#endif


/*
//时间堆类
class time_heap
{
public:
        //初始化大小为cap的空堆   默认实参 10
        time_heap(int cap = 10) throw(std::exception) : capacity(cap), cur_size(0)
        {
                array = new heap_timer* [capacity]; //创建堆数组
                if(!array)
                {
                        throw std::exception();
                }
                for(int i = 0; i < capacity; ++i)
                {
                        array[i] = NULL;
                }
        }

        //用已有数组初始化堆
        time_heap(heap_timer** init_array, int size, int capacity) throw (std::exception) :cur_size(size), capacity(capacity)
        {
                if(capacity < size)
                {
                        throw std::exception();
                }
                array = new heap_timer* [capacity]; //创建堆数组
                if(!array)
                {
                        throw std::exception();
                }
                for(int i = 0; i < capacity; ++i)
                {
                        array[i] = NULL;
                }
                if(size != 0)
                {
                        //初始化堆数组
                        for(int i = 0; i < size; ++i)
                        {
                                array[i] = init_array[i];
                        }
                        for(int i = (cur_size-1)/2; i>=0; --i)
                        {
                                //对数组的第(cur_size-1)/2 ~ 0 执行下虑操作  TODO
                                percolate_down(i);
                        }
                }
        }
	//销毁时间堆
        ~time_heap()
        {
                for(int i = 0; i < cur_size; ++i)
                {
                        delete array[i];
                }
                delete []array;
        }
public:
        //添加目标定时器timer
        void add_timer(heap_timer* timer) throw (std::exception)
        {
                if(!timer)
                {
                        return;
                }
                //容量不够,扩大一倍
                if(cur_size >= capacity)
                {
                        resize();
                }
                //新插入一个元素，堆大小+1，hole是新建空穴位置
                int hole = cur_size + 1;
                int parent = 0;
                //空穴到根节点路径上所有节点执行上虑操作 fixme
                for(; hole > 0; hole = parent)
                {
                        parent = (hole - 1)/2;
                        if(array[parent]->expire <= timer->expire)
                        {
                                break;
                        }
                        array[hole] = array[parent];
                }
                array[hole] = timer;
        }
        //删除目标定时器timer
        void del_timer(heap_timer* timer)
        {
                if(!timer)
                {
                        return;
                }
                //延迟销毁，仅将目标定时器的回调函数设为空。可节省真正删除定时器的开销，但易使堆数组>膨胀
                timer->cb_func = NULL;
        }
	//获得堆顶定时器
        void pop_timer()
        {
                if(empty())
                {
                        return;
                }
                if(array[0])
                {
                        delete array[0];
                        //讲原来堆顶元素替换为数组最后一个元素
                        array[0] = array[--cur_size];
                       percolate_down(0); //对新的堆顶元素执行下虑操作  
                }
        }
        //心搏函数
        void tick()
        {
                heap_timer* tmp = array[0];
                time_t cur = time(NULL);
                while(!empty())
                {
                        if(!tmp)
                        {
                                break;
                        }
                        //如果堆顶定时器没到期，退出循环
                        if(tmp->expire > cur)
                        {
                                break;
                        }
                        //否则，执行堆顶定时器的任务
                        if(array[0]->cb_func)
                        {
                                array[0]->cb_func(array[0]->user_data);
                        }
                        //堆顶元素删除，生成新的堆顶定时器array[0]
                        pop_timer();
                        tmp = array[0];
                }
        }
        bool empty() const
        {
                return cur_size == 0;
        }
private:
        //下虑操作，确保数组以第hole个节点作为根的子树拥有最小堆性质
        void percolate_down(int hole)
        {
                heap_timer* temp = array[hole];
                int child = 0;
                for(;((hole*2+1) <= (cur_size-1));hole = child)
                {
                        child = hole*2+1;
                        if((child < (cur_size-1)) && (array[child+1]->expire < array[child]->expire))
                        {
                                ++child;
                        }
                        if(array[child]->expire < temp->expire)
                        {
                                array[hole] = array[child];
                        }
                        else
                        {
                                break;
                        }
                }
                array[hole] = temp;
        }
        void resize() throw(std::exception)
        {
                heap_timer** temp = new heap_timer* [2*capacity];
                for(int i = 0; i < 2*capacity; ++i)
                {
                        temp[i] = NULL;
                }
                if(!temp)
                {
                        throw std::exception();
                }
                capacity = 2 * capacity;
                for(int i = 0; i < cur_size; ++i)
                {
                        temp[i] = array[i];
                }
               delete []array;
               array = temp;
        }
//private:
public:
        heap_timer** array; //堆数组
        int capacity;       //堆数组的容量
        int cur_size;       //堆数组当前包含元素的个数
};

*/