#ifndef m_queue_TEST_H_
#define m_queue_TEST_H_
#include <iostream>

#include "queue.h"

// queue test : 测试 queque 的接口

namespace poorstl
{

void queue_test()
{
  	std::cout << "[==============================================================]"<< std::endl;
  	std::cout << "[----------------- Run adapter test : queue -----------------]"<< std::endl;
  	std::cout << "[-------------------------- API test --------------------------]"<< std::endl;
    std::cout << "[-------------------------- 创建queue -------------------------]"<< std::endl;
    poorstl::queue<int> q1(5, 3);
    bool b1 = q1.empty();
    int s1 = q1.size();
    int f1 = q1.front();
    int back1 = q1.back();
    std::cout << "q1是否为空: "<< b1 << std::endl;
    std::cout << "q1的大小: "<< s1 << std::endl;
    std::cout << "q1的头元素: "<< f1 << std::endl;
    std::cout << "q1的尾元素: "<< back1 << std::endl;

    std::cout << "[-------------------------- push pop ------------------------]"<< std::endl;
    poorstl::queue<int> d2(10, 1);
    d2.push(2);
    int back2 = d2.back();
    int s12 = d2.size();
    d2.pop(); //pop有问题deque的问题 TODO
    int f2 = d2.front();
    std::cout << "d2的大小: "<< s12 << std::endl;
    std::cout << "d2的尾元素: "<< back2 << std::endl;
    std::cout << "d2的头元素: "<< f2 << std::endl;
}


} // namespace poorstl
#endif // m_queue_TEST_H_