#pragma once
template <class T> class PriorityQueue
{
	struct PQNode
	{
		PQNode* next;
		PQNode* prev;
		T* value;
	};

	PQNode* head;
	PQNode* tail;
	unsigned int size;

public:
	PriorityQueue()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	T* poll()
	{
		if ( head == nullptr ) return nullptr;
		else
		{
			T* toReturn = head->value;
			PQNode* todelete = head;
			head = head->next;
			delete todelete;
			if ( head == nullptr )
			{
				tail = nullptr;
			}
			else
			{
				head->prev = nullptr;
			}
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
				tail = new PQNode;

				tail->value = t;
				tail->prev = nullptr;
				tail->next = nullptr;
				if ( head == nullptr ) head = tail;
				size++;
				return true;
			}
			else
			{
				PQNode* small = tail;

				while ( small != nullptr && small->value->compareTo(t) > 0)
				{
					small = small->prev;
				}

				if ( small == nullptr )
				{
					PQNode* newNode = new PQNode;

					newNode->value = t;
					newNode->next = head;
					head->prev = newNode;
					head = newNode;
					size++;
				}
				else if (small == tail )
				{
					PQNode* newNode = new PQNode;

					newNode->value = t;
					newNode->prev = tail;
					tail->next = newNode;
					tail = newNode;
					size++;
				}
				else
				{
					PQNode* newNode = new PQNode;
					PQNode* nodeAfterSmall = small->next;

					newNode->value = t;
					newNode->prev = small;
					newNode->next = small->next;
					small->next = newNode;
					nodeAfterSmall->prev = newNode;
					size++;
				}
				return true;
			}
		}
	}

	T* peek()
	{
		if ( head == nullptr ) return nullptr;
		else return head->value;
	}

	T* deleteExisting( T* t )
	{
		T* toReturn = nullptr;
		PQNode* test = head;
		while ( test != nullptr && !test->value->equals(t))
		{
			test = test->next;
		}
		if ( test != nullptr )
		{
			if ( test == head && test == tail )
			{
				head = nullptr;
				tail = nullptr;
				toReturn = test->value;
				delete test;
			}
			else if ( test == head )
			{
				head = test->next;
				head->prev = nullptr;
				toReturn = test->value;
				delete test;
			}
			else if ( test == tail )
			{
				tail = test->prev;
				tail->next = nullptr;
				toReturn = test->value;
				delete test;
			}
			else
			{
				PQNode* prev = test->prev;
				PQNode* next = test->next;
				prev->next = next;
				next->prev = prev;
				toReturn = test->value;
				delete test;
			}
			size--;
		}

		return toReturn;
	}
};