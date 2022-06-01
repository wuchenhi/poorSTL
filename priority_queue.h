//container adapter 容器配接器  vector + heap
#ifndef m_PRIORITY_QUEUE_H
#define m_PRIORITY_QUEUE_H

#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"
#include "uninitialized.h"
#include "m_vector.h"
#include "functional.h"
#include "m_heap.h"

namespace poorstl
{
    template <class T, class Container = poorstl::vector<T>, class Compare = poorstl::less<typename Container::value_type>>
    class priority_queue
    {
    public:
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
    private:
        Container con;  //底层容器
        Compare com;    //比较器
    public:
        priority_queue() : con() {}
        explicit priority_queue(const Compare& c) : con(), com(c) {} 

        ~priority_queue() {}

        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last, const Compare& c)
            : con(), com(c) {
                make_heap(con.begin(), con.end(), com);
                //make_heap(con.begin(), con.end());
            } 

        bool empty() const { 
            return con.empty();
        }

        size_type size() const {
            return con.size();
        }

        reference top() {
            return con.front();
        }

        void push(const value_type& value) {
            try {
                con.push_back(value);
                //push_heap(con.begin(), con.end());
                push_heap(con.begin(), con.end(), com);
            }
            catch(...){
                con.clear();
            }
        }

        void pop() {
            try {
                //pop_heap(con.begin(), con.end());
                pop_heap(con.begin(), con.end(), com);
                 con.pop_back();
            }
            catch(...){
                con.clear();
            }
        }

    };
      
}

#endif