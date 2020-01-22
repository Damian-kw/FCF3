#include "catch.hpp"
extern "C"
{
#include "salloc.h"
}
uint8_t valid_pointer_variable = 1;
void *test_pointer = &valid_pointer_variable;
uint32_t test_size = sizeof(uint8_t);
SCENARIO("salloc tests", "[salloc]")
{
    GIVEN("salloc is disabled")
    {
        salloc_disable();
        WHEN("salloc is called")
        {
            test_pointer = salloc(test_size);
            THEN("salloc return null")
            {
                REQUIRE(test_pointer == NULL);
            }
        }
    }
}