#ifndef m_STACK_H_
#define m_STACK_H_

// stack: 队列是adpter（配接器），以deque为底部结构，允许新增、移除、取得最顶端元素
// FILO 先进后出  没有迭代器

#include "m_deque.h"
#include "functional.h" 

namespace poorstl
{

// 默认使用 poorstl::deque 作为底层容器
template <class T, class Container = poorstl::deque<T>>
class stack
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
    stack() = default;

    explicit stack(size_type n) 
        :con(n) {}

    stack(size_type n, const value_type& value)
    :con(n, value){}
    //复制、移动函数
    stack(const Container& c) 
        :con(c) {}

    stack(Container&& c)
        :con(poorstl::move(c)) {}

    stack(const stack& rhs) 
        :con(rhs.con) {}

    stack(stack&& rhs)
        :con(poorstl::move(rhs.con)) {}

    stack& operator=(const stack& rhs) {
        con = rhs.con; 
        return *this; 
    }
    stack& operator=(stack&& rhs) { 
        con = poorstl::move(rhs.con);
        return *this;
    }

    ~stack() = default;

    // 访问元素相关操作
    reference       top()       { return con.back(); }

    // 容量相关操作
    bool      empty()  noexcept { return con.empty(); }
    size_type size()   noexcept { return con.size(); }

    // 修改容器相关操作
    // 末端进 末端端出
    void push(const value_type& value) { 
        con.push_back(value); 
    }

    void pop() { 
        con.pop_back();
    }

    void clear() { 
        while (!empty()) {
            pop(); 
        }
    }

    void swap(stack& rhs) { 
        poorstl::swap(con, rhs.con); 
    }
};

// 重载比较操作符
template <class T, class Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return !(lhs < rhs);
}

// 重载 poorstl 的 swap
template <class T, class Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) {
    lhs.swap(rhs);
}

} // namespace poorstl
#endif // !m_STACK_H_

