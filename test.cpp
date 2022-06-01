//#include <catch2/catch_test_macros.hpp>

#include "string_test.h"
#include "vector_test.h"
#include "list_test.h"
#include "deque_test.h"
#include "heap_test.h"

int main()
{
	poorstl::string_test();
	poorstl::vector_test();
	poorstl::list_test();
    poorstl::deque_test();
    poorstl::heap_test();
	return 0;
}