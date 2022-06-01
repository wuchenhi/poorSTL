#ifndef m_HEAP_TEST_H_
#define m_HEAP_TEST_H_


// max-heap 

#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"
#include "uninitialized.h"

#include <iostream>

#include "m_heap.h"
#include "m_vector.h"
// heap test : 测试 heap

namespace poorstl
{

void heap_test()
{
  	std::cout << "[==============================================================]"<< std::endl;
  	std::cout << "[--------------------------heap test --------------------------]"<< std::endl;
    std::cout << "[----------------------------原数组--- ------- -----------------]"<< std::endl;
    poorstl::vector<int> v1;
     for (int n=0; n<11; ++n){
        v1.push_back(n);
    }
    for (auto it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    std::cout << "[-----------------------make_heap 后数组--- ------- ------------]"<< std::endl;
    make_heap(v1.begin(), v1.end());
    for (auto it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    std::cout << "[-----------------------push_heap 后数组--- ------- ------------]"<< std::endl;
    v1.push_back(11);
    push_heap(v1.begin(), v1.end());
    for (auto it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    std::cout << "[-----------------------pop_heap 后数组--- ------- ------------]"<< std::endl;
    pop_heap(v1.begin(), v1.end());
    v1.pop_back();
    for (auto it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    std::cout << "[-----------------------sort_heap 后数组--- ------- ------------]"<< std::endl;
    sort_heap(v1.begin(), v1.end());
        for (auto it = v1.begin(); it != v1.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    std::cout << "[-----------------------自定义比较器 less 后数组--- ------- ------------]"<< std::endl;
    std::cout << "[-----------------------push_heap 后数组--- ------- ------------]"<< std::endl;
    poorstl::vector<int> v10;
     for (int n=0; n<11; ++n){
        v10.push_back(n);
    }
    make_heap(v10.begin(), v10.end());
    v10.push_back(11);
    push_heap(v10.begin(), v10.end(), poorstl::less<int>() );
    for (auto it = v10.begin(); it != v10.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    std::cout << "[-----------------------pop_heap 后数组--- ------- ------------]"<< std::endl;
    pop_heap(v10.begin(), v10.end());
    v10.pop_back();
    for (auto it = v10.begin(); it != v10.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}


} // namespace poorstl
#endif // m_HEAP_TEST_H_