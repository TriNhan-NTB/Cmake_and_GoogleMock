#include<iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Expectation;
using ::testing::Eq;
using ::testing::Invoke;
using ::testing::DoDefault;

class CL1
{
  protected:
    int testb();
    int rt1();
    int rt2();
    int rt3();
    void print_hello();
    int add(int a, int b);

};

class MyTest: public CL1
{
  public:

    int Init()
    {
      return CL1::testb();
    }
    int Ret1(){return CL1::rt1();}
    int Ret2(){return CL1::rt2();}
    int Ret3(){return CL1::rt3();}
    int ADD(int a, int b){return CL1::add(a,b);}
};



class MockTest
{
 public:
  MOCK_METHOD(int, testb, ());
  MOCK_METHOD(int, rt1, ());
  MOCK_METHOD(int, rt2, ());
  MOCK_METHOD(int, rt3, ());
  MOCK_METHOD(void, print_hello, ());
  MOCK_METHOD(int, add, (int, int));

  static MockTest& GetIntance()
  {
    static MockTest r = MockTest();
    return r;
  }
  
};

int CL1::testb()
{
  return MockTest::GetIntance().testb();
}

int CL1::rt1()
{
  return MockTest::GetIntance().rt1();
}

int CL1::rt2()
{
  return MockTest::GetIntance().rt2();
}

int CL1::rt3()
{
  return MockTest::GetIntance().rt3();
}

void CL1::print_hello()
{
  cout << "HELLO!!!" << endl;
}

int CL1::add(int a,int b)
{
  return MockTest::GetIntance().add(a,b);
}

TEST(MyTest, Basic_Action1) 
{
  auto& mock = MockTest::GetIntance();

  EXPECT_CALL(mock, testb)
    .Times(2)
    .WillOnce(Return(10))
    .WillOnce(Return(20));    

  MyTest m;
  
  EXPECT_EQ(10, m.Init());
  EXPECT_EQ(20, m.Init());
 
}

TEST(MyTest, Basic_InSequence)
{
  auto& mock = MockTest::GetIntance();
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock, rt1);
    EXPECT_CALL(mock, rt2);
  }
  MyTest m;

  /*make sure rt1 occur before rt2 otherwise it will failed */

  m.Ret1();
  m.Ret2();
}

TEST(MyTest, Basic_Sticky)
{
  auto& mock = MockTest::GetIntance();
  /*
  Note: we should using Insequence to make sure the order is correct 
  otherwise the orthe will reverse
  */
  int n = 2;
  {
    ::testing::InSequence seq;

      for(int i=n; i > 0; i--)
      {
        EXPECT_CALL(mock, testb)
          .WillOnce(::testing::Return(10*i))
          .RetiresOnSaturation();
      }
  }

  MyTest m;
  EXPECT_EQ(20, m.Init());
  EXPECT_EQ(10, m.Init());
}

TEST(Mytest, Basic_Expectation_After)
{
  auto& mock = MockTest::GetIntance();
  Expectation call_rt1 = EXPECT_CALL(mock, rt1);
  Expectation call_rt3 = EXPECT_CALL(mock, rt3);
  EXPECT_CALL(mock, rt2)
    .After(call_rt1, call_rt3);

  /*make sure rt1 and rt3 occur before rt2 otherwise it will failed */
  MyTest m;
  m.Ret1();
  m.Ret3();

  m.Ret2();
}

struct default_value_test
{
  bool return_true()
  {
    return true;
  }
};


TEST(Mytest, Basic_ON_CALL)
{
  auto& mock = MockTest::GetIntance();
  default_value_test dvt;
  ON_CALL(mock, rt1)
    .WillByDefault(Invoke(&dvt, &default_value_test::return_true));
  EXPECT_CALL(mock, rt1)
    .Times(AtLeast(1))
    .WillOnce(DoDefault());

  MyTest m;
  cout << "Basic_ON_CALL: " << m.Ret1() << endl;
  //m.Ret1();
  

}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  int i = RUN_ALL_TESTS();
  return 0;
}