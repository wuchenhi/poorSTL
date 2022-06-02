/**
 * isert  erase 待完善 
 * pop_front 有问题
 * 2022.5.31
 * wuyi
 */

#ifndef m_DEQUE_H_
#define m_DEQUE_H_


// deque: 双端队列

#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"
#include "uninitialized.h"

namespace poorstl
{

// deque map 初始化的大小
const int DEQUE_map_node_init_SIZE = 8;

//deque_buf_size设置为类，可根据不同的数据类型确定 bufSize
template <class T>
class deque_buf_size
{
public:
    //if 元素 小于256  4096/sizeof(T)     sizeof(int) = 4
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

// deque 迭代器 
template <class T, class Ref, class Ptr>
struct deque_iterator : public iterator<random_access_iterator_tag, T>
{
    typedef deque_iterator<T, T&, T*>             iterator;
    typedef deque_iterator<T, const T&, const T*> const_iterator;
    typedef deque_iterator                        self;

    typedef T            value_type;
    typedef Ptr          pointer;
    typedef Ref          reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;
    typedef T*           value_pointer;
    typedef T**          map_pointer;

    static const size_type buffer_size = deque_buf_size<T>::value;

    // 迭代器所含成员数据
    value_pointer cur;    // 指向所在缓冲区的当前元素
    value_pointer first;  // 指向所在缓冲区的头部
    value_pointer last;   // 指向所在缓冲区的尾部
    map_pointer   node;   // 缓冲区所在节点  中控器

    // 构造、复制、移动函数
    deque_iterator() noexcept
        :cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}

    deque_iterator(value_pointer v, map_pointer n)
        :cur(v), first(*n), last(*n + buffer_size), node(n) {}

    deque_iterator(const iterator& rhs)
        :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}

    deque_iterator(iterator&& rhs) noexcept
        :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){
        rhs.cur = nullptr;
        rhs.first = nullptr;
        rhs.last = nullptr;
        rhs.node = nullptr;
    }

    deque_iterator(const const_iterator& rhs)
        :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

    self& operator=(const iterator& rhs)
    {
        if (this != &rhs)
        {
            cur = rhs.cur;
            first = rhs.first;
            last = rhs.last;
            node = rhs.node;
        }
        return *this;
     }

    // 转到另一个缓冲区
    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    // 重载运算符
    reference operator*()  const { return *cur; }
    pointer   operator->() const { return cur; }

    difference_type operator-(const self& x) const
    {
        return static_cast<difference_type>(buffer_size) * (node - x.node)  //有无 -1
        + (cur - first) - (x.cur - x.first);
    }
    //前置
    self& operator++()
    {
        ++cur;
        if (cur == last)
        { // 如果到达缓冲区的尾
        set_node(node + 1);
        cur = first;
        }
        return *this;
    }
    //后置
    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    //前置
    self& operator--()
    {
        if (cur == first)
        { // 如果到达缓冲区的头
        set_node(node - 1);
        cur = last;
        }
        --cur;
        return *this;
    }
    //后置
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
    //为实现随机存取
    self& operator+=(difference_type n)
    {
        const auto offset = n + (cur - first);
        if (offset >= 0 && offset < static_cast<difference_type>(buffer_size)) { // 仍在当前缓冲区
            cur += n;
        }
        else{ // 要跳到其他的缓冲区
            const auto node_offset = offset > 0
                ? offset / static_cast<difference_type>(buffer_size)
                : -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
        }
        return *this;
    }
    self operator+(difference_type n) const 
    {
        self tmp = *this;
        return tmp += n;
    }
    self& operator-=(difference_type n)
    {
     return *this += -n;
    }
    self operator-(difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }
    //实现随机存取
    reference operator[](difference_type n) const { return *(*this + n); }

    // 重载比较操作符
    bool operator==(const self& rhs) const { return cur == rhs.cur; }
    bool operator< (const self& rhs) const
    { return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node); } //分别判断是否在同一个缓冲区
    bool operator!=(const self& rhs) const { return !(*this == rhs); }
    bool operator> (const self& rhs) const { return rhs < *this; }
    bool operator<=(const self& rhs) const { return !(rhs < *this); }
    bool operator>=(const self& rhs) const { return !(*this < rhs); }
};

// 模板类 deque   T 数据类型
template <class T>
class deque
{
public:
  // deque 的型别定义
    typedef poorstl::allocator<T>                      allocator_type;
    typedef poorstl::allocator<T>                      data_allocator;
    typedef poorstl::allocator<T*>                     map_allocator;

    typedef deque_iterator<T, T&, T*>                  iterator; //SGI

    // deque 的嵌套型别定义
    typedef T               value_type;//SGI
    typedef value_type      &reference;
    typedef size_t          size_type;//SGI
    typedef ptrdiff_t       difference_type;

    typedef value_type* pointer;//SGI
protected:
    typedef pointer* map_pointer;//SGI  元素指针的指针

    static const size_type buffer_size = deque_buf_size<T>::value;

private:
    // 2个迭代器 + map_ + map_size组成一个 deque
    iterator       begin_of_deuqe;     // 指向第一个节点
    iterator       end_of_deuqe;       // 指向最后一个结点
    map_pointer    map_;               // 指向一块 map，map 中的每个元素都是一个指针，指向一个缓冲区 SGI
    size_type      map_size;           // map 内指针的数目

public:
    // 构造、复制、移动函数
    deque() { fill_init(0, value_type()); }

    deque(size_type n, const value_type& value) { fill_init(n, value); }

    deque(deque&& rhs) noexcept
        :begin_of_deuqe(poorstl::move(rhs.begin_of_deuqe)),
        end_of_deuqe(poorstl::move(rhs.end_of_deuqe)),
        map_(rhs.map_),
        map_size(rhs.map_size)
    {
        rhs.map_ = nullptr;
        rhs.map_size = 0;
    }
    /*
    //析构函数
    ~deque()
    {
        if (map_ != nullptr)
        {
            clear();
            //data_allocator::deallocate(*begin_of_deuqe.node, buffer_size);  //free(): invalid pointer  TODO
            //*begin_of_deuqe.node = nullptr;
            //map_allocator::deallocate(map_, map_size);
            //map_ = nullptr;
        }
    }
    */

public:
    // 迭代器相关操作

    iterator               begin()         noexcept
    { return begin_of_deuqe; }

    iterator               end()           noexcept
    { return end_of_deuqe; }
  
    // 容量相关操作
    bool      empty()     noexcept  { return begin() == end(); }
    size_type size()      noexcept  { return end_of_deuqe - begin_of_deuqe; }
    size_type max_size()  noexcept  { return static_cast<size_type>(-1); }

    // 访问元素相关操作 
    reference       operator[](size_type n)
    {
        return begin_of_deuqe[n];
    }

    reference       at(size_type n)      
    { 
        return (*this)[n];
    }

    reference       front()
    {
        return *begin();
    }

    reference       back()
    {
        iterator tmp = end();
        --tmp;
        return *tmp;
        //return *(end() - 1);   
    }

    // 修改容器相关操作

    // 头尾push函数
    void     push_front(const value_type& value){
    if (begin_of_deuqe.cur != begin_of_deuqe.first) {  //第一缓冲区有备用空间
        data_allocator::construct(begin_of_deuqe.cur - 1, value);
        --begin_of_deuqe.cur;
    }
    else {           //第一缓冲区无备用空间  代码没问题 为什么报错
        create_buffer(begin_of_deuqe.node-2, begin_of_deuqe.node-1);
        //poorstl::_construct(&**(begin_of_deuqe.node-1), 10);
        --begin_of_deuqe;
        //begin_of_deuqe.cur = begin_of_deuqe.last - 1;
        data_allocator::construct(begin_of_deuqe.cur, value);

        //*(begin_of_deuqe.node - 1) = create_node();  //配置一个新节点
        //require_capacity(1, true);
        /*
        try
        {
            //begin_of_deuqe.set_node(begin_of_deuqe - 1);
            --begin_of_deuqe;
            begin_of_deuqe.cur = begin_of_deuqe.last - 1;
            data_allocator::construct(begin_of_deuqe.cur, value);
        }
        catch (...)
        {
            ++begin_of_deuqe;
           // begin_of_deuqe.set_node(begin_of_deuqe + 1);
            begin_of_deuqe.cur = begin_of_deuqe.first;
            //--begin_of_deuqe;
            //deallocate_node(begin_of_deuqe - 1);
            destroy_buffer(begin_of_deuqe.node-1, begin_of_deuqe.node-1);
            throw;
        }
        */
    }
    }

    void     push_back(const value_type& value){
        if (end_of_deuqe.cur != end_of_deuqe.last - 1)
        {
            data_allocator::construct(end_of_deuqe.cur, value);
            ++end_of_deuqe.cur;
        }
        else  //TODO
        {
            //require_capacity(1, false);
            //data_allocator::construct(end_of_deuqe.cur, value);
            //++end_of_deuqe;
        }
    }

    // 头尾pop函数
    void     pop_front(){
        if (begin_of_deuqe.cur != begin_of_deuqe.last - 1)
        {
            data_allocator::destroy(begin_of_deuqe.cur);
            ++begin_of_deuqe.cur;
        }
        else
        {
            data_allocator::destroy(begin_of_deuqe.cur);
            ++begin_of_deuqe;
            destroy_buffer(begin_of_deuqe.node - 1, begin_of_deuqe.node - 1);
        }
    }

    void     pop_back(){
        if (end_of_deuqe.cur != end_of_deuqe.first)
        {
            --end_of_deuqe.cur;
            data_allocator::destroy(end_of_deuqe.cur);
        }
        else
        {
            --end_of_deuqe;
            data_allocator::destroy(end_of_deuqe.cur);
            destroy_buffer(end_of_deuqe.node + 1, end_of_deuqe.node + 1);
        }
    }

    //清空 函数
    void     clear(){
        // clear 会保留一个缓冲区，deque的策略，也是deque的初始状态
        //针对头尾以外的缓冲区 饱满的
        for (map_pointer cur = begin_of_deuqe.node + 1; cur < end_of_deuqe.node; ++cur) {
            data_allocator::destroy(*cur, *cur + buffer_size);
            data_allocator::deallocate(*cur, buffer_size);
        }
        // 有头尾两个以上的缓冲区
        if (begin_of_deuqe.node != end_of_deuqe.node) { 
            data_allocator::destroy(begin_of_deuqe.cur, begin_of_deuqe.last);
            data_allocator::destroy(end_of_deuqe.first, end_of_deuqe.cur);
            //尾部缓冲区释放
            data_allocator::deallocate(end_of_deuqe.first, buffer_size);
        } else {
            //只析构元素，不释放空间
            data_allocator::destroy(begin_of_deuqe.cur, end_of_deuqe.cur);
        }
        //shrink_to_fit();
        end_of_deuqe = begin_of_deuqe;
    }

    // 交换函数
    void     swap(deque& rhs) noexcept {
        if (this != &rhs) {
            poorstl::swap(begin_of_deuqe, rhs.begin_of_deuqe);
            poorstl::swap(end_of_deuqe, rhs.end_of_deuqe);
            poorstl::swap(map_, rhs.map_);
            poorstl::swap(map_size, rhs.map_size);
        }
    }

private:
    // 私有辅助函数

    // create node / destroy node 配置一个新节点
    map_pointer create_node(){
        map_pointer tmp = nullptr;
        tmp = map_allocator::allocate(1);
        *(tmp) = nullptr;
    }

    //创建map
    map_pointer create_map(size_type size){
        map_pointer tmp = nullptr;
        tmp = map_allocator::allocate(size);
        for (size_type i = 0; i < size; ++i)
            *(tmp + i) = nullptr;
        return tmp;
    }

    //创建缓冲区
    void        create_buffer(map_pointer nstart, map_pointer nfinish) {
        map_pointer cur;
        for (cur = nstart; cur <= nfinish; ++cur) {
            *cur = data_allocator::allocate(buffer_size);
        }
    }

    //清空缓冲区
    void        destroy_buffer(map_pointer nstart, map_pointer nfinish){
        for (map_pointer cur = nstart; cur <= nfinish; ++cur) {
            data_allocator::deallocate(*cur, buffer_size);
            *cur = nullptr;
        }     
    }
    
    // initialize
    void        map_node_init(size_type nelem);
    void        fill_init(size_type n, const value_type& value);

    // reallocate
    void        require_capacity(size_type n, bool front){
        if (front && (static_cast<size_type>(begin_of_deuqe.cur - begin_of_deuqe.first) < n))
        {
            const size_type need_buffer = (n - (begin_of_deuqe.cur - begin_of_deuqe.first)) / buffer_size + 1;
            if (need_buffer > static_cast<size_type>(begin_of_deuqe.node - map_))
            {
                reallocate_map_at_front(need_buffer);
                return;
            }
            create_buffer(begin_of_deuqe.node - need_buffer, begin_of_deuqe.node - 1);
        }
        else if (!front && (static_cast<size_type>(end_of_deuqe.last - end_of_deuqe.cur - 1) < n))
        {
            const size_type need_buffer = (n - (end_of_deuqe.last - end_of_deuqe.cur - 1)) / buffer_size + 1;
            if (need_buffer > static_cast<size_type>((map_ + map_size) - end_of_deuqe.node - 1))
            {
                reallocate_map_at_back(need_buffer);
                return;
            }
            create_buffer(end_of_deuqe.node + 1, end_of_deuqe.node + need_buffer);
        }
    }

    void        reallocate_map_at_front(size_type need);
    void        reallocate_map_at_back(size_type need);

};

/*****************************************************************************************/
// 辅助函数

// map_node_init 函数
template <class T>
void deque<T>::map_node_init(size_type nElem)
{
    const size_type nNode = nElem / buffer_size + 1;  // 需要分配的缓冲区个数
    //map管理的节点数 +2 前后各预备一个，最少8个
    map_size = poorstl::max(static_cast<size_type>(DEQUE_map_node_init_SIZE), nNode + 2);
    //map_ = map_allocator::allocate(map_size);
    try
    {
        map_ = create_map(map_size);
    }
    catch (...)
    {
        map_ = nullptr;
        map_size = 0;
        throw;
    }
    // 让 nstart 和 nfinish 都指向 map_ 最中央的区域，方便向头尾扩充
    map_pointer nstart = map_ + (map_size - nNode) / 2;
    map_pointer nfinish = nstart + nNode - 1;
    try
    {
        //配置缓冲区  所有缓冲区加起来就算deque的可用空间
        create_buffer(nstart, nfinish);
    }
    catch (...)
    {
        map_allocator::deallocate(map_, map_size);
        map_ = nullptr;
        map_size = 0;
        throw;
    }
    begin_of_deuqe.set_node(nstart);
    end_of_deuqe.set_node(nfinish);
    begin_of_deuqe.cur = begin_of_deuqe.first;
    end_of_deuqe.cur = end_of_deuqe.first + (nElem % buffer_size);
}


// fill_init 函数 产生并安排deque的结构，设定初值
template <class T>
void deque<T>::fill_init(size_type n, const value_type& value)
{
    map_node_init(n); //安排好deque的结构
    if (n != 0)
    {
        //为每个节点的缓冲区设定初值
        for (auto curr = begin_of_deuqe.node; curr < end_of_deuqe.node; ++curr)
        {
            //poorstl::fill_uninit(*cur, *cur + buffer_size, value);
            for (; (*curr) != (*curr + buffer_size); ++curr) {
                poorstl::_construct(&**curr, value);
            }
        }
        //poorstl::fill_uninit(end_of_deuqe.first, end_of_deuqe.cur, value);
        for (auto curr = end_of_deuqe.first ; curr != end_of_deuqe.cur; ++curr) {
                poorstl::_construct(&*curr, value);
            }
    }
}

// reallocate_map_at_front 函数
template <class T>
void deque<T>::reallocate_map_at_front(size_type need_buffer)
{
    const size_type new_map_size = poorstl::max(map_size << 1,
                                            map_size + need_buffer + DEQUE_map_node_init_SIZE);
    map_pointer new_map = create_map(new_map_size);
    const size_type old_buffer = end_of_deuqe.node - begin_of_deuqe.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;

    // 另新的 map 中的指针指向原来的 buffer，并开辟新的 buffer
    auto begin = new_map + (new_map_size - new_buffer) / 2;
    auto mid = begin + need_buffer;
    auto end = mid + old_buffer;
    create_buffer(begin, mid - 1);
    for (auto begin1 = mid, begin2 = begin_of_deuqe.node; begin1 != end; ++begin1, ++begin2)
        *begin1 = *begin2;

    // 更新数据
    map_allocator::deallocate(map_, map_size);
    map_ = new_map;
    map_size = new_map_size;
    begin_of_deuqe = iterator(*mid + (begin_of_deuqe.cur - begin_of_deuqe.first), mid);
    end_of_deuqe = iterator(*(end - 1) + (end_of_deuqe.cur - end_of_deuqe.first), end - 1);
}

// reallocate_map_at_back 函数
template <class T>
void deque<T>::reallocate_map_at_back(size_type need_buffer)
{
    const size_type new_map_size = poorstl::max(map_size << 1,
                                            map_size + need_buffer + DEQUE_map_node_init_SIZE);
    map_pointer new_map = create_map(new_map_size);
    const size_type old_buffer = end_of_deuqe.node - begin_of_deuqe.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;

    // 另新的 map 中的指针指向原来的 buffer，并开辟新的 buffer
    auto begin = new_map + ((new_map_size - new_buffer) / 2);
    auto mid = begin + old_buffer;
    auto end = mid + need_buffer;
    for (auto begin1 = begin, begin2 = begin_of_deuqe.node; begin1 != mid; ++begin1, ++begin2)
        *begin1 = *begin2;
    create_buffer(mid, end - 1);

    // 更新数据
    map_allocator::deallocate(map_, map_size);
    map_ = new_map;
    map_size = new_map_size;
    begin_of_deuqe = iterator(*begin + (begin_of_deuqe.cur - begin_of_deuqe.first), begin);
    end_of_deuqe = iterator(*(mid - 1) + (end_of_deuqe.cur - end_of_deuqe.first), mid - 1);
}

/*****************************************************************************************/
// 重载比较操作符
template <class T>
bool operator==(const deque<T>& lhs, const deque<T>& rhs)
{
    return lhs.size() == rhs.size() && 
        poorstl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const deque<T>& lhs, const deque<T>& rhs)
{
    return poorstl::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const deque<T>& lhs, const deque<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
bool operator>(const deque<T>& lhs, const deque<T>& rhs)
{
    return rhs < lhs;
}

template <class T>
bool operator<=(const deque<T>& lhs, const deque<T>& rhs)
{
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const deque<T>& lhs, const deque<T>& rhs)
{
    return !(lhs < rhs);
}

// 重载 poorstl 的 swap
template <class T>
void swap(deque<T>& lhs, deque<T>& rhs)
{
    lhs.swap(rhs);
}

} // namespace poorstl
#endif

