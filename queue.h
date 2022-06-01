#ifndef m_QUEUE_H_
#define m_QUEUE_H_


// queue: 队列是adpter（配接器），以deque为底部结构，封装低端的出口和前端的入口即可
// FIFO 先进先出  没有迭代器

#include "m_deque.h"
#include "functional.h"


namespace poorstl
{

// 默认使用 poorstl::deque 作为底层容器
template <class T, class Container = poorstl::deque<T>>
class queue
{
public:
    typedef Container                           container_type;
    // 使用底层容器的型别
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;

private:
    container_type con;  // 底层容器

public:
    // 构造函数
    queue() = default;

    explicit queue(size_type n) 
        :con(n) {}

    queue(size_type n, const value_type& value)
    :con(n, value){}
    //复制、移动函数
    queue(const Container& c) 
        :con(c) {}

    queue(Container&& c)
        :con(poorstl::move(c)) {}

    queue(const queue& rhs) 
        :con(rhs.con) {}

    queue(queue&& rhs)
        :con(poorstl::move(rhs.con)) {}

    queue& operator=(const queue& rhs) {
        con = rhs.con; 
        return *this; 
    }
    queue& operator=(queue&& rhs) { 
        con = poorstl::move(rhs.con);
        return *this;
    }

    ~queue() = default;

    // 访问元素相关操作
    reference       front()       { return con.front(); }
    reference       back()        { return con.back(); }

    // 容量相关操作
    bool      empty()  noexcept { return con.empty(); }
    size_type size()   noexcept { return con.size(); }

    // 修改容器相关操作
    // 末端进 前端出
    void push(const value_type& value) { 
        con.push_back(value); 
    }

    void pop() { 
        con.pop_front(); //pop_front 有问题
    }

    void clear() { 
        while (!empty()) {
            pop(); 
        }
    }

    void swap(queue& rhs) { 
        poorstl::swap(con, rhs.con); 
    }
};

// 重载比较操作符
template <class T, class Container>
bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return !(lhs < rhs);
}

// 重载 poorstl 的 swap
template <class T, class Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) {
    lhs.swap(rhs);
}

} // namespace poorstl
#endif // !m_QUEUE_H_

