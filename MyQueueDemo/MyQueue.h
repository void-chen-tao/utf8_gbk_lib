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
	*	Limit��������Member�ĸ���
	*	q_size���ڱ�����ǰ��������ܹ��洢���ٸ�Member
	*	q_time���ڱ����������е�ǰ״̬�Ļ���
	*/
	const static int Limit = 50;
	int q_size;
	double q_time;

	/*
	*	Member�ṹ���е�object��Ա���ڴ洢�����ж���ĵ�ַ��
	* 
	*	ManagerMember�ṹ�����ڹ���������������������ʼ
	*	���ս�λ�ã��Լ���ǰ����ĳ��ȡ�
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
	MyQueue(int q = 20);							//	����q���ڱ�ʾ��ǰ��������ܹ��洢���ٸ�Member�ṹ��
	MyQueue(const MyQueue& s);						//	���ƹ��캯��
	~MyQueue();										//	����������
	MyQueue& operator=(const MyQueue& s);			//	��ֵ���캯��
	bool IsEmpty() const;							//	�жϵ�ǰ�����Ƿ�Ϊ��
	bool IsFull() const;							//	�жϵ�ǰ�����Ƿ�����
	bool Append(const void* s) const;				//	�����β����³�Ա
	bool Delete() const;							//	ɾ���������еĳ�Ա
	int Size() const;								//	���ض������洢��Ŀ
	int Length() const;								//	���ص�ǰ��Ա��Ŀ
	friend std::ostream& operator<<(std::ostream& os, const MyQueue& q);
};


#endif	/*  OFFICIAL  */


#endif	/*  _MYQUEUE_H_  */