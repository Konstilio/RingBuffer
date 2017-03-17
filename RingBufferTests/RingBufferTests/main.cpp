//
//  main.cpp
//  RingBufferTests
//
//  Created by Aleksander Konstantinov on 3/12/17.
//  Copyright © 2017 Aleksander Konstantinov. All rights reserved.
//

#include <iostream>
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
    
    Testable()
    {
        std::cout << "~Testable()" << --counter << std::endl;
        delete m_i;
        m_i = nullptr;
    }
    
    
private:
    static int counter;
    int* m_i = 0;
};

TEST (SquareRootTest, PositiveNos) {
    EXPECT_EQ (18.0, 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


