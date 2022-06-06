//#include <catch2/catch_test_macros.hpp>

#include "string_test.h"
#include "vector_test.h"
#include "list_test.h"
#include "deque_test.h"
#include "heap_test.h"
#include "priority_queue_test.h"
#include "queue_test.h"
#include "stack_test.h"

int main()
{
	poorstl::string_test();
	poorstl::vector_test();
	poorstl::list_test();
    poorstl::deque_test();
    poorstl::heap_test();
    poorstl::priority_queue_test();
    poorstl::queue_test();
    poorstl::stack_test();
	return 0;
}