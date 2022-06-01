#ifndef m_HASHTABLE_H_
#define m_HASHTABLE_H_

// hashtable : 哈希表，使用separate chaining(开链法)处理冲突(维护一个list)
// hashtable表格内的元素为bucket 桶子
#include "m_iterator.h"
#include "m_allocator.h"
#include "m_algo.h"
#include "uninitialized.h"

namespace poorstl
{
//哈希表节点定义
template <class Value>
class hashtableNode{
    hashtableNode *next;
    Value         value;

    hashtableNode(const Value& val) : next(nullptr), value(val) {}
    hashtableNode(const hashtableNode& hash) : next(hash.next), value(hash.value) {}
    hashtableNode(hashtableNode&7 hash) : next(hash.next), value(poorstl::move(hash.value)) {
        hash.next = nullptr;
    }
};

//哈希表迭代器
class hashtableIterator
{
private:
    
public:
    hashtableIterator() {}
    ~hashtableIterator() {}
};



} // namespace poorstl
#endif // !m_HASHTABLE_H_