#ifndef m_DEQUE_TEST_H_
#define m_DEQUE_TEST_H_
#include <iostream>

#include "m_deque.h"

// deque test : 测试 deque 的接口

#include "m_deque.h"

namespace poorstl
{

void deque_test()
{
  	std::cout << "[==============================================================]"<< std::endl;
  	std::cout << "[----------------- Run container test : deque -----------------]"<< std::endl;
  	std::cout << "[-------------------------- API test --------------------------]"<< std::endl;
    std::cout << "[-------------------------- 创建deque -------------------------]"<< std::endl;
    poorstl::deque<int> d1(5, 3);
    bool b1 = d1.empty();
    int s1 = d1.size();
    int f1 = d1.front();
    int back1 = d1.back();
    std::cout << "d1是否为空: "<< b1 << std::endl;
    std::cout << "d1的大小: "<< s1 << std::endl;
    std::cout << "d1的头元素: "<< f1 << std::endl;
    std::cout << "d1的尾元素: "<< back1 << std::endl;

    std::cout << "[-------------------------- push pop ------------------------]"<< std::endl;
    poorstl::deque<int> d2(10, 1);
    d2.push_back(2);
    int back2 = d2.back();
    int s12 = d2.size();
    //d1.push_front(10);
    int f2 = d2.front();
    std::cout << "d2的大小: "<< s12 << std::endl;
    std::cout << "d2的尾元素: "<< back2 << std::endl;
    std::cout << "d2的头元素: "<< f2 << std::endl;

	std::cout << "[------------------------- 迭代器 test --------------------------]"<< std::endl;  
    poorstl::deque<int> d3;
    for (int n=0; n<11; ++n){
        d3.push_back(n);
    }
    for (auto it = d3.begin(); it != d3.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    
    std::cout <<"at函数测试: "<< d3.at(3) << std::endl;
}


} // namespace poorstl
#endif // m_DEQUE_TEST_H_