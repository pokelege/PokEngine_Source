#pragma once

template <class T> class Queue
{
	struct QNode
	{
		QNode* next;
		QNode* prev;
		T* value;
	};

	QNode* head;
	QNode* tail;
	unsigned int size;

public:
	Queue()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	~Queue()
	{
		cleanUp();
	}

	void cleanUp()
	{
		while ( poll() != nullptr );
	}

	T* poll()
	{
		if ( head == nullptr ) return nullptr;
		else
		{
			T* toReturn = head->value;
			QNode* toDelete = head;
			head = head->next;
			delete toDelete;
			if ( head == nullptr ) tail = nullptr;
			else head->prev = nullptr;
			size--;
			return toReturn;
		}
	}

	bool offer( T* t )
	{
		if ( t == nullptr ) return false;
		else
		{
			if ( tail == nullptr )
			{
				tail = new QNode();
				tail->value = t;
				if ( head == nullptr ) head = tail;
				size++;
				return true;
			}
			else
			{
				tail->next = new QNode();
				tail->next->prev = tail;
				tail = tail->next;
				tail->value = t;
				size++;
				return true;
			}
		}
	}
};