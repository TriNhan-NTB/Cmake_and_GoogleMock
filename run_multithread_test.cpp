/**
 * @file run_multithread_test.cpp
 * @author nhannguyen (nhannguyen482000@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-29
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include<iostream>
#include <memory>
#include<mutex>
#include<unistd.h>
#include<condition_variable>
#include<memory>
#include<thread>

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


class Conversation
{
    protected:
        bool is_canceled{false};
        std::condition_variable cancel_condition;
        std::mutex cancel_mutex;
    public:
        void Cancel()
        {
            cout << "LINE 30" << endl;
            if(is_canceled)
            {
                std::unique_lock<std::mutex> lock{cancel_mutex};
                cout << "LINE 34" << endl;
                while(is_canceled)
                {
                    cancel_condition.wait(lock);
                }
                cout << "Out HERE" << endl;
            }
        }
};

class ConversationTest: public Conversation
{
    public:
        void Set_is_canceled()
        {
            is_canceled = true;
        }
        void Notify_cancel_condition()
        {
            while(is_canceled == false);
            is_canceled = false;
            cancel_condition.notify_one();
        }

        std::thread Thread_Cancel()
        {
            return std::thread([this]{Conversation::Cancel();});
        }

        std::thread Thread_Notify_cancel_condition()
        {
            return std::thread([this]{Notify_cancel_condition();});
        }
};

class ConversationFixture: public ::testing::Test
{
    public:
        std::shared_ptr<ConversationTest> conversation1 = std::make_unique<ConversationTest>();
};

TEST_F(ConversationFixture, Test1)
{
    std::thread t1 = conversation1->Thread_Cancel();
    std::thread t2 = conversation1->Thread_Notify_cancel_condition();
    conversation1->Set_is_canceled();
    t1.join();
    t2.join();
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  int i = RUN_ALL_TESTS();
  return 0;
}