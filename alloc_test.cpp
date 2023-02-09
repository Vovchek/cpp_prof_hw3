#include <gtest/gtest.h>
#include "log_alloc.h"
#include "dumb_container.h"

class logging_allocatorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
       // m1 = std::map<int, int>{};
        int fact{1};
        for (int i = 0; i < 10; ++i, fact *= i)
        {
            m1[i] = fact;
        }


        fact = 1;
        for (int i = 0; i < 10; ++i, fact *= i)
        {
            m2[i] = fact;
        }
    }
    // void tearDown() override {}
    std::map<int, int> m1;
    std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, 10>> m2;
};

class dumb_containerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

        for (int i = 0; i < 10; ++i)
        {
            dc.append(i);
        }

        for (int i = 0; i < 10; ++i)
        {
            dc_a.append(i);
        }
    }
    // void tearDown() override {}
    dumb_container<int> dc;
    dumb_container<int, logging_allocator<int, 10>> dc_a;
};

TEST_F(logging_allocatorTest, CompareToStatndardAllocator)
{
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(m1[i], m2[i]);
    }
}

TEST_F(dumb_containerTest, TestWithStandardAllocator)
{
    for (int i = 0; i < 10; ++i)
    {
        ASSERT_EQ(dc.hasNext(), true);
        EXPECT_EQ(dc.next(), i);
    }
}

TEST_F(dumb_containerTest, TestWithLoggingAllocator)
{
    for (int i = 0; i < 10; ++i)
    {
        ASSERT_EQ(dc_a.hasNext(), true);
        EXPECT_EQ(dc_a.next(), i);
    }
}
