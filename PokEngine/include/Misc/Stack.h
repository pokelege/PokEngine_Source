#pragma once

template <class T> class Stack
{
	struct SNode
	{
		SNode* next;
		SNode* prev;
		T* value;
	};

	SNode* head;
	SNode* tail;
	unsigned int size;

public:
	Stack()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	~Stack()
	{
		cleanUp();
	}

	void cleanUp()
	{
		while ( poll() != nullptr );
	}

	T* poll()
	{
		if ( tail == nullptr ) return nullptr;
		else
		{
			T* toReturn = tail->value;
			SNode* toDelete = tail;
			tail = tail->prev;
			delete toDelete;
			if ( tail == nullptr ) head = nullptr;
			else tail->next = nullptr;
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
				tail = new SNode;
				tail->value = t;
				if ( head == nullptr ) head = tail;
				size++;
				return true;
			}
			else
			{
				tail->next = new SNode;
				tail->next->prev = tail;
				tail = tail->next;
				tail->value = t;
				size++;
				return true;
			}
		}
	}

	T* peek()
	{
		if ( tail == nullptr) return nullptr;
		else return tail->value;
	}

	class SIterator
	{
		T** theArray;
		unsigned int size;
		unsigned int currentIndex;
	public:
		SIterator( Stack* stack )
		{
			if ( stack->size > 0 )
			{
				SNode* temp = stack->tail;
				size = stack->size;
				currentIndex = 0;

				theArray = new T*[size];

				for ( unsigned int i = 0; i < size && temp != nullptr; i++ )
				{
					theArray[i] = temp->value;
					temp = temp->prev;
				}
			}
			else
			{
				theArray = nullptr;
				size = 0;
				currentIndex = 0;
			}
		}
		~SIterator()
		{
			if ( size > 0 ) delete[] theArray;
		}
		bool hasNext()
		{
			return currentIndex < size;
		}

		T* next()
		{
			if ( currentIndex < size )
			{
				currentIndex++;
				return theArray[currentIndex - 1];
			}
			else return nullptr;
		}

		void repeat()
		{
			currentIndex = 0;
		}
	};

	SIterator* iterator()
	{
		return new SIterator( this );
	}
};