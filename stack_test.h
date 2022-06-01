#ifndef m_STACK_TEST_H_
#define m_STACK_TEST_H_
#include <iostream>

#include "stack.h"

// stack test : 测试 queque 的接口

namespace poorstl
{

void stack_test()
{
  	std::cout << "[==============================================================]"<< std::endl;
  	std::cout << "[----------------- Run adapter test : stack -----------------]"<< std::endl;
  	std::cout << "[-------------------------- API test --------------------------]"<< std::endl;
    std::cout << "[-------------------------- 创建stack -------------------------]"<< std::endl;
    poorstl::stack<int> q1(5, 3);
    bool b1 = q1.empty();
    int s1 = q1.size();
    int f1 = q1.top();
    std::cout << "q1是否为空: "<< b1 << std::endl;
    std::cout << "q1的大小: "<< s1 << std::endl;
    std::cout << "q1的顶端元素: "<< f1 << std::endl;

    std::cout << "[-------------------------- push pop ------------------------]"<< std::endl;
    poorstl::stack<int> d2(10, 1);
    d2.push(2);
    int back2 = d2.top();
    int s12 = d2.size();
    d2.pop(); 
    int f2 = d2.top();
    std::cout << "d2的大小: "<< s12 << std::endl;
    std::cout << "d2的顶端元素: "<< back2 << std::endl;
    std::cout << "d2的顶端元素: "<< f2 << std::endl;
}


} // namespace poorstl
#endif // m_STACK_TEST_H_