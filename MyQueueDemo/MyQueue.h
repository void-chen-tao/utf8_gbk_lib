/*  This file is the declaration of MyQueue  */
#ifndef _MYQUEUE_H_
#define _MYQUEUE_H_

//#define OFFICIAL

#ifdef OFFICIAL
/*  Official  */

class Queue
{
private:
	enum { Q_SIZE = 10 };
	//	private representation to be developed later
	//	class scope definitions
		//	Node is a nested structure definition local to this class
	struct Node
	{
		Item item;
		struct Node* next;
	};
	//	private class members
	Node* front;		//	pointer to front of Queue
	Node* rear;			//	pinter to rear of Queue
	int items;			//	current number of items in Queue
	const int qsize;	//	maximum number of items in Queue
public:
	Queue(int qs = Q_SIZE);			//	create queue with a qs limit
	~Queue();
	bool IsEmpty() const;
	bool IsFull() const;
	int QueueCount() const;
	bool EnQueue(const Item& item);			//	add item to end
	bool DeQueue(Item& item);				//	remove item from front
};


#else
/*  By myself  */
#include <iostream>

class MyQueue
{
private:
	/*
	*	Limit用于限制Member的个数
	*	q_size用于表明当前队列最大能够存储多少个Member
	*	q_time用于表明整个队列当前状态的花费
	*/
	const static int Limit = 50;
	int q_size;
	double q_time;

	/*
	*	Member结构体中的object成员用于存储进队列对象的地址。
	* 
	*	ManagerMember结构体用于管理队列链表，包括链表的起始
	*	与终结位置，以及当前链表的长度。
	*/
	struct Member
	{
		const void* object;
		Member* next;
	};
	struct ManagerMember
	{
		Member* start;
		Member* end;
		int m_size;
	};
	ManagerMember* manager;
public:
	MyQueue(int q = 20);							//	参数q用于表示当前队列最大能够存储多少个Member结构体
	MyQueue(const MyQueue& s);						//	复制构造函数
	~MyQueue();										//	队列清理函数
	MyQueue& operator=(const MyQueue& s);			//	赋值构造函数
	bool IsEmpty() const;							//	判断当前队列是否为空
	bool IsFull() const;							//	判断当前队列是否已满
	bool Append(const void* s) const;				//	向队列尾添加新成员
	bool Delete() const;							//	删除队列首中的成员
	int Size() const;								//	返回队列最大存储数目
	int Length() const;								//	返回当前成员数目
	friend std::ostream& operator<<(std::ostream& os, const MyQueue& q);
};


#endif	/*  OFFICIAL  */


#endif	/*  _MYQUEUE_H_  */