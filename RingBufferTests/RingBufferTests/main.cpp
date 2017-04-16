//
//  main.cpp
//  RingBufferTests
//
//  Created by Aleksander Konstantinov on 3/12/17.
//  Copyright © 2017 Aleksander Konstantinov. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <RingBuffer.h>
#include <gtest/gtest.h>

class TestableWithoutCoppyAssign
{
public:
    explicit TestableWithoutCoppyAssign(int i, int j = 0)
        : m_i (new int(i + j))
    {
        ++counter;
    }
    
    TestableWithoutCoppyAssign(const TestableWithoutCoppyAssign &other)
        : m_i(new int(*other.m_i))
    {
        ++copyConstructs;
        ++counter;
    }
    
    void operator=(const TestableWithoutCoppyAssign& other) = delete;
    
    TestableWithoutCoppyAssign(TestableWithoutCoppyAssign &&other)
        : m_i(other.m_i)
    {
        ++counter;
        other.m_i = nullptr;
    }
    
    TestableWithoutCoppyAssign& operator=(TestableWithoutCoppyAssign&& other)
    {
        delete m_i;
        m_i = other.m_i;
        other.m_i = nullptr;
        ++moveAssigns;
        return *this;
    }
    
    ~TestableWithoutCoppyAssign()
    {
        delete m_i;
        m_i = nullptr;
        --counter;
        ++destructs;
    }
    
    int getVal() const
    {
        return *m_i;
    }
    
    static int getCounter()
    {
        return counter;
    }
    
    static int getCopyConstructs()
    {
        return copyConstructs;
    }
    
    static int getMoveAssigns()
    {
        return moveAssigns;
    }
    
    static int getDestructs()
    {
        return destructs;
    }
    
    static void reset()
    {
        moveAssigns = 0;
        counter = 0;
        copyConstructs = 0;
        destructs = 0;
    }
    
private:
    static int counter;
    static int copyConstructs;
    static int moveAssigns;
    static int destructs;
    int* m_i = 0;
};

bool operator==(const TestableWithoutCoppyAssign &right, const TestableWithoutCoppyAssign &left)
{
    return right.getVal() == left.getVal();
}

int TestableWithoutCoppyAssign::counter = 0;
int TestableWithoutCoppyAssign::moveAssigns = 0;
int TestableWithoutCoppyAssign::copyConstructs = 0;
int TestableWithoutCoppyAssign::destructs = 0;

TEST (RingBuffer, createTests) {
    RingBuffer<TestableWithoutCoppyAssign> rb(10);
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.capacity(), 10);
    rb.push_back(TestableWithoutCoppyAssign(2));
    EXPECT_EQ(rb.size(), 1);
    
    RingBuffer<TestableWithoutCoppyAssign> rbc = rb;
    EXPECT_EQ(rbc.size(), 1);
    EXPECT_EQ(rbc.capacity(), 10);
    EXPECT_EQ(rbc, rb);
    
    RingBuffer<TestableWithoutCoppyAssign> rbm = std::move(rb);
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.capacity(), 0);
    EXPECT_EQ(rbm.size(), 1);
    EXPECT_EQ(rbm.capacity(), 10);
    EXPECT_NE(rb, rbm);
}

TEST (RingBuffer, iterTests) {
    {
        RingBuffer<TestableWithoutCoppyAssign>::iterator emptyIt1;
        RingBuffer<TestableWithoutCoppyAssign>::iterator emptyIt2;
        EXPECT_TRUE(emptyIt1 == emptyIt2);
        EXPECT_FALSE(emptyIt1 != emptyIt2);
        EXPECT_TRUE(emptyIt1 <= emptyIt2);
        EXPECT_FALSE(emptyIt1 < emptyIt2);
        EXPECT_TRUE(emptyIt1 >= emptyIt2);
        EXPECT_FALSE(emptyIt1 > emptyIt2);
        auto it1 = emptyIt1 + 5;
        auto it2 = 5 + emptyIt1;
        EXPECT_EQ(it1 - it2, 0);
    }
    
    {
        RingBuffer<int> rb(5);
        for (int i = 1; i <=5; ++i)
            rb.push_back(i);
        
        auto it = rb.begin();
        EXPECT_EQ(*it, 1);
        EXPECT_EQ(it[1], 2);
        EXPECT_EQ(it[2], 3);
        EXPECT_EQ(it[3], 4);
        EXPECT_EQ(it[4], 5);
        
        EXPECT_EQ(*it++, 1);
        EXPECT_EQ(*it++, 2);
        EXPECT_EQ(*it++, 3);
        EXPECT_EQ(*it++, 4);
        EXPECT_EQ(*it++, 5);
        
        EXPECT_EQ(it, rb.end());
        
        EXPECT_EQ(*(--it), 5);
        EXPECT_EQ(*(--it), 4);
        EXPECT_EQ(*(--it), 3);
        EXPECT_EQ(*(--it), 2);
        EXPECT_EQ(*(--it), 1);
        
        rb.push_back(6);
        it = rb.begin();
        EXPECT_EQ(*it, 2);
        
        RingBuffer<int> rb2(5);
        for (int i = 1; i <=5; ++i)
            rb2.push_back(1);
        std::copy(rb.begin(), rb.end(), rb2.begin());
        EXPECT_EQ(rb, rb2);
        
        it = std::find(rb.begin(), rb.end(), 3);
        EXPECT_EQ(it - rb.begin(), 1);
        
        *it = 10;
        auto it1 = std::find(rb.begin(), rb.end(), 10);
        EXPECT_EQ(it, it1);
    }
}

TEST (RingBuffer, behaviourTests) {
    TestableWithoutCoppyAssign::reset();
    constexpr int elemsSize = 4;
    RingBuffer<TestableWithoutCoppyAssign> rb(elemsSize);
    for (int i = 1; i <= elemsSize; ++i)
    {
        TestableWithoutCoppyAssign t(i,i);
        rb.push_back(t);
    }
    EXPECT_EQ(rb.size(), 4);
    
    for (int i = 1; i <= elemsSize; ++i)
    {
        TestableWithoutCoppyAssign t(i,i);
        rb.push_back(t);
    }
    EXPECT_EQ(rb.size(), elemsSize);
    
    EXPECT_EQ(TestableWithoutCoppyAssign::getCounter(), elemsSize);
    EXPECT_EQ(TestableWithoutCoppyAssign::getCopyConstructs(), 2 * elemsSize);
    EXPECT_EQ(TestableWithoutCoppyAssign::getMoveAssigns(), 0);
    EXPECT_EQ(TestableWithoutCoppyAssign::getDestructs(), 3 * elemsSize);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


