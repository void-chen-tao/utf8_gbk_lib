#include "MyQueue.h"

#ifdef OFFICIAL


#else
MyQueue::MyQueue(int q /* = 20 */)
{
	/*  ��ʼ��������Ϣ  */
	q_size = q > Limit ? Limit : q;
	q_time = 0.0;

	/*  ��ʼ��������  */
	manager = new ManagerMember;
	memset(manager, '\0', sizeof(ManagerMember));
}

MyQueue::MyQueue(const MyQueue& s)
{
	/*  ������Ϣ����  */
	q_size = s.q_size;
	q_time = s.q_time;

	/*
	*	������MyQueue����Ĺ�����
	*/
	manager = new ManagerMember;
	manager->m_size = s.manager->m_size;
	if (s.manager->m_size)
	{
		/*  ���s�����е�Member��Ա��Ŀ����0���������  */
		manager->start = new Member;
		Member* temp = manager->start;
		Member* step = s.manager->start;

		for (; step != nullptr; step = step->next)
		{
			temp->object = step->object;
			if (step->next)
			{
				Member* next = new Member;
				temp->next = next;
				temp = temp->next;
			}
			else
			{
				temp->next = nullptr;
				manager->end = temp;
			}	
		}
	}
	else
		memset(manager, '\0', sizeof(ManagerMember));
}

MyQueue& MyQueue::operator=(const MyQueue& s)
{
	if (this == &s)
		return *this;

	/*  �ͷ��Լ���Member�ṹ��  */
	for (Member* temp = manager->start; temp != nullptr;)
	{
		Member* next = temp->next;
		delete temp;
		temp = next;
	}
	memset(manager, '\0', sizeof(ManagerMember));

	if (q_size < s.q_size)
	{
		//	����s����q_size��Member��Ա
		if (s.manager->m_size)
		{
			manager->start = new Member;
			Member* temp = manager->start;
			Member* step = s.manager->start;

			for (; step != nullptr && ++manager->m_size < q_size;)
			{
				temp->object = step->object;
				if (step->next)
				{
					Member* next = new Member;
					temp->next = next;
				}
				else
				{
					temp->next = nullptr;
					manager->end = temp;
				}
				step = step->next;
			}
		}
	}
	else
	{
		//	��������
		manager->start = new Member;
		Member* temp = manager->start;
		Member* step = s.manager->start;

		for (; step != nullptr;)
		{
			temp->object = step->object;
			if (step->next)
			{
				Member* next = new Member;
				temp->next = next;
			}
			else
			{
				temp->next = nullptr;
				manager->end = temp;
			}
			step = step->next;
		}
	}
	
	return *this;
}

MyQueue::~MyQueue()
{
	if (manager->m_size)
	{
		Member* temp = manager->start;
		for (; temp != nullptr;)
		{
			Member* next = temp->next;
			delete temp;
			temp = next;
		}
	}
	delete manager;
}

bool MyQueue::IsEmpty() const
{
	return this->manager->m_size;
}

bool MyQueue::IsFull() const
{
	return q_size == manager->m_size;
}

bool MyQueue::Append(const void* s) const
{
	if (!s || IsFull())
		return false;

	Member* new_member = new Member;
	new_member->object = s;
	new_member->next = nullptr;

	++manager->m_size;
	manager->end->next = new_member;

	return true;
}

bool MyQueue::Delete() const
{
	if (IsEmpty())
		return false;

	Member* new_start = manager->start->next;
	delete manager->start;
	--manager->m_size;
	manager->start = new_start;

	return true;
}

int MyQueue::Size() const
{
	return q_size;
}

int MyQueue::Length() const
{
	return manager->m_size;
}

std::ostream& operator<<(std::ostream& os, const MyQueue& q)
{
	os << "Queue Size is: " << q.q_size << ", Now have " << q.manager->m_size << " Members.\n";
	return os;
}

#endif	/*  OFFICIAL  */