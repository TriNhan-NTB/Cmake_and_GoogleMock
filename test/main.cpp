#include<istream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace::std;

using ::testing::Return;
using ::testing::InSequence;
using ::testing::DoDefault;

#include "functionA.h"

class my_Mock
{
    public:
        MOCK_METHOD(int, sum, (int,int));
        static my_Mock& GetInstance()
        {
            static my_Mock r = my_Mock();
            return r;
        }
};



TEST(Test1, BASIC1)
{

}

int main(int argc, char **argv)
{
    cout << "abc";
    ::testing::InitGoogleTest(&argc, argv);
    int a = RUN_ALL_TESTS();
    return 0;
}