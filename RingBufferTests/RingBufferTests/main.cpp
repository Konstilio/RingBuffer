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

class Testable
{
public:
    explicit Testable(int i, int j = 0)
        : m_i (new int(i + j))
    {
        ++counter;
    }
    
    Testable(const Testable &other)
        : m_i(new int(*other.m_i))
    {
        ++counter;
    }
    
    Testable(Testable &&other)
        : m_i(other.m_i)
    {
        ++counter;
        other.m_i = nullptr;
        std::cout << "Testable(&&)" << std::endl;
    }
    
    ~Testable()
    {
        std::cout << "~Testable()" << --counter << std::endl;
        delete m_i;
        m_i = nullptr;
    }
    
    int getVal() const
    {
        return *m_i;
    }
    
private:
    static int counter;
    int* m_i = 0;
};

bool operator==(const Testable &right, const Testable &left)
{
    return right.getVal() == left.getVal();
}

int Testable::counter = 0;

TEST (RingBuffer, createTests) {
    RingBuffer<Testable> rb(10);
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.capacity(), 10);
    rb.push_back(Testable(2));
    EXPECT_EQ(rb.size(), 1);
    
    RingBuffer<Testable> rbc = rb;
    EXPECT_EQ(rbc.size(), 1);
    EXPECT_EQ(rbc.capacity(), 10);
    EXPECT_EQ(rbc, rb);
    
    RingBuffer<Testable> rbm = std::move(rb);
    EXPECT_EQ(rb.size(), 0);
    EXPECT_EQ(rb.capacity(), 0);
    EXPECT_EQ(rbm.size(), 1);
    EXPECT_EQ(rbm.capacity(), 10);
    EXPECT_NE(rb, rbm);
}

TEST (RingBuffer, iterTests) {
    {
        RingBuffer<Testable>::iterator emptyIt1;
        RingBuffer<Testable>::iterator emptyIt2;
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
    RingBuffer<int> rb(10);
    for (int i = 1; i <= 5; ++i)
        rb.push_back(i);
    EXPECT_EQ(rb.size(), 5);
    EXPECT_EQ(rb.back(), 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


