#ifndef m_PRIORITY_QUEUE_TEST_H_
#define m_PRIORITY_QUEUE_TEST_H_

#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"
#include "uninitialized.h"

#include <iostream>

#include "m_heap.h"
#include "m_vector.h"
#include "priority_queue.h"

// priority_queue test : 测试 priority_queue

namespace poorstl
{

void priority_queue_test()
{
  	std::cout << "[==============================================================]"<< std::endl;
  	std::cout << "[---------------------priority_queue test ---------------------]"<< std::endl;
    poorstl::priority_queue<int> q1;
    for (int n=0; n<11; ++n){
        q1.push(n);
    }
    auto s1 = q1.size();
    auto e1 = q1.empty();
    auto t1 = q1.top();
    std::cout << s1 << std::endl;
    std::cout << e1 << std::endl;
    std::cout << t1 << std::endl;

    while(!q1.empty()) {
        std::cout << q1.top() << ' ';
        q1.pop();
    }
    std::cout << std::endl;

}


} // namespace poorstl
#endif // m_HEAP_TEST_H_