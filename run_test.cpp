#include<iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

using namespace std;
using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Expectation;
using ::testing::Eq;
using ::testing::Invoke;
using ::testing::DoDefault;
using ::testing::NiceMock;

class CL1
{
  public:
    int testb();
    int rt1();
    int rt2();
    int rt3();
    void print_hello();
    int add(int a, int b);

};

class MyTest: public CL1
{
  private:
    CL1 cl1;
  public:

    int Init()
    {
      return cl1.testb();
    }
    int Ret1(){return cl1.rt1();}
    int Ret2(){return cl1.rt2();}
    int Ret3(){return cl1.rt3();}
    int ADD(int a, int b){return cl1.add(a,b);}
    void call_Ret1_Ret2()
    {
      int a = Ret1();
      int b = Ret2();
    }
};

class MyFixture: public ::testing::Test
{
  public:
    std::shared_ptr<MyTest> mytest_1 = std::make_shared<MyTest>();
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
  static NiceMock<MockTest>& GetInstance_Nice()
  {
    static NiceMock<MockTest> instance;
    return instance;
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
}

TEST(Mytest, Basic_ONCALL1)
{
  auto& mock = MockTest::GetIntance();
  ON_CALL(mock,add(3,3)).WillByDefault(Return(9));
  EXPECT_CALL(mock, add)
    .Times(::testing::AnyNumber());
  MyTest m;
  //EXPECT_EQ(9, m.ADD(3,4)); //failed
  EXPECT_EQ(9, m.ADD(3,3)); //pass
}


TEST_F(MyFixture, Basic_NiceMock)
{
  auto& mock1 = MockTest::GetIntance();
  auto& mock2 = MockTest::GetInstance_Nice();
  static MockTest mock3;
  static NiceMock<MockTest> mock4;

  EXPECT_CALL(mock1, rt1)
      .Times(1)
      .WillOnce(Return(false));

  mytest_1->Ret1();
  mytest_1->Ret2();
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  int i = RUN_ALL_TESTS();
  return 0;
}