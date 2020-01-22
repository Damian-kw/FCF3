#include "catch.hpp"
#include "salloc.h"
uint8_t valid_pointer_variable = 1;
void *test_pointer = &valid_pointer_variable;
uint32_t test_size = sizeof(uint8_t);
SCENARIO("salloc tests", "[salloc]")
{
    salloc_enable();
    GIVEN("salloc is disabled")
    {
        salloc_disable();
        WHEN("salloc is called")
        {
            test_pointer = salloc(test_size);
            THEN("salloc returns null")
            {
                REQUIRE(test_pointer == NULL);
            }
        }
    }
    GIVEN("salloc buffer is full")
    {
        salloc_clear_buffer();
        test_pointer = salloc(BUFFER_SIZE_IN_BYTES);
        WHEN("salloc is called")
        {
            test_pointer = salloc(test_size);
            THEN("salloc returns null ")
            {
                REQUIRE(test_pointer == NULL);
            }
        }
    }
}