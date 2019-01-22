#include "stdafx.h"
#include "CppUnitTest.h"
#include "../DataStructures/UniProjects/Queue.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructuresUnitTests
{
	TEST_CLASS(Queue)
	{
	public:
		TEST_METHOD(empty)
		{
			::Queue<int> a;
			Assert::AreEqual(true, a.empty());
			a.push(5);
			Assert::AreEqual(false, a.empty());
			a.pop();
			Assert::AreEqual(true, a.empty());
		}
		TEST_METHOD(size)
		{
			::Queue<int> a;
			Assert::AreEqual(0u, a.size());
			a.push(5);
			Assert::AreEqual(1u, a.size());
			a.pop();
			Assert::AreEqual(0u, a.size());

			for (unsigned i = 0; i < 10; ++i)
			{
				a.push(i);		/*the idea is to make it overlap*/
				a.push(i);
				a.pop();
			}
			Assert::AreEqual(10u, a.size());

			for (unsigned i = 0; i < 20; ++i)
			{
				a.push(i);			/*the idea is to make it use grow overlap*/
			}

			Assert::AreEqual(30u, a.size());

			for (unsigned i = 0; i < 30; ++i)
			{
				a.pop();
			}

			Assert::AreEqual(0u, a.size());

			for (unsigned i = 0; i < 10000; ++i)
			{
				a.push(i);		/*the idea is to make it overlap and grow*/
				a.push(i);
				a.pop();
				a.push(i);
			}

			Assert::AreEqual(20000u, a.size());

			for (unsigned i = 0; i < 5000; ++i)
			{
				a.pop();
				a.push(i);	/*the idea is to make it overlap and halt*/
				a.pop();
				a.pop();

			}

			Assert::AreEqual(10000u, a.size());
		}
		TEST_METHOD(front)
		{
			::Queue<int> a;
			a.push(5);
			Assert::AreEqual(5, a.front());
			a.pop();

			for (int i = 0; i < 10; ++i)
			{
				a.push(i);		/*the idea is to make it overlap*/
				a.push(i);
				a.pop();
			}

			for (int i = 0; i < 5; ++i)
			{
				Assert::AreEqual(i + 5, a.front());
				a.pop();
				Assert::AreEqual(i + 5, a.front());
				a.pop();
			}

			for (unsigned i = 0; i < 20; ++i)
			{
				a.push(i);			/*the idea is to make it use grow overlap*/
			}

			for (int i = 0; i < 20; ++i)
			{
				Assert::AreEqual(i, a.front());
				a.pop();
			}

			Assert::IsTrue(a.empty());
			for (int i = 0; i < 1200; ++i)
			{
				a.push(i);		//the idea is to make it overlap and grow
				a.push(i);
				a.pop();
				a.push(i);
			}

			for (int i = 0; i < 800; ++i)
			{
				Assert::AreEqual(i + 400, a.front());
				a.pop();
				Assert::AreEqual(i+400, a.front());
				a.pop();
				Assert::AreEqual(i + 400, a.front());
				a.pop();
				a.push(i);
			}

			Assert::AreEqual(800u, a.size());

			for (int i = 0; i < 800; ++i)
			{
				Assert::AreEqual(i, a.front());
				a.pop();
			}
		}
		TEST_METHOD(back)
		{
			::Queue<int> a;
			a.push(5);
			Assert::AreEqual(5, a.back());
			a.pop();

			for (int i = 0; i < 10; ++i)
			{
				a.push(i);		/*the idea is to make it overlap*/
				Assert::AreEqual(i, a.back());
				a.push(i);
				Assert::AreEqual(i, a.back());
				a.pop();
			}

			for (int i = 0; i < 10000; ++i)
			{
				a.push(i);		/*the idea is to make it overlap*/
				Assert::AreEqual(i, a.back());
				a.push(i);
				Assert::AreEqual(i, a.back());
				a.pop();
			}

			for (unsigned i = 0; i < 10009; ++i)
			{
				a.pop();
				Assert::AreEqual(9999, a.back());
			}
			Assert::AreEqual(9999, a.back());
			a.pop();
			Assert::IsTrue(a.empty());

			for (int i = 0; i <= 10000; ++i)
			{
				a.push(i);		/*the idea is to make it overlap*/
			}
			for (int i = 9999; i >= 0; --i)
			{
				a.pop();
				Assert::AreEqual(a.back(), i+1);		//checking for mistakes if it is slowly decreasing
				a.push(i);		
				Assert::AreEqual(a.back(), i);
				a.pop();
				Assert::AreEqual(a.back(), i);
			}

			Assert::AreEqual(a.back(), a.front());
			Assert::AreEqual(a.back(), 0);
		}
	};
}