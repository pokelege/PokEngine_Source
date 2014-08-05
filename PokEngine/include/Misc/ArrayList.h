#pragma once
template <class T> class ArrayList
{
	T** theArray;
	unsigned int arraySize;
	unsigned int currentMaxSize;

	void expand()
	{
		unsigned int lastMaxSize = currentMaxSize;
		currentMaxSize++;
		currentMaxSize *= 2;
		T** newArray = new T*[currentMaxSize];

		if ( theArray != nullptr && lastMaxSize > 0 )
		{
			memcpy( newArray , theArray , sizeof( T* ) * arraySize );

			if ( theArray != nullptr && lastMaxSize > 0 )
			{
				delete[] theArray;
				theArray = nullptr;
			}
		}
		theArray = newArray;
	}

	void shrink()
	{
		unsigned int lastMaxSize = currentMaxSize;
		currentMaxSize /= 2;
		if ( currentMaxSize > 0 )
		{
			T** newArray = new T*[currentMaxSize];
			memcpy( newArray , theArray , sizeof( T* ) * arraySize );
			if ( theArray != nullptr && lastMaxSize > 0 )
			{
				delete[] theArray;
				theArray = nullptr;
			}
			theArray = newArray;
		}
		else
		{
			if ( theArray != nullptr && lastMaxSize > 0 )
			{
				delete[] theArray;
				theArray = nullptr;
			}
		}
	}
public:
	ArrayList()
	{
		theArray = new T*[5];
		arraySize = 0;
		currentMaxSize = 5;
	}

	~ArrayList()
	{
		if ( theArray != nullptr && currentMaxSize > 0 )
		{
			delete[] theArray;
			theArray = nullptr;
		}
	}

	bool add( T* t )
	{
		if ( t == nullptr ) return false;
		else
		{
			if ( currentMaxSize <= arraySize ) expand();
			theArray[arraySize] = t;
			arraySize++;
			return true;
		}
	}

	T* get( unsigned int index )
	{
		if ( index < 0 || index >= arraySize ) return nullptr;
		else return theArray[index];
	}

	bool remove( unsigned int index )
	{
		if ( arraySize == 0 ) return false;
		else if ( index < 0 || index >= arraySize ) return false;
		else
		{
			arraySize--;
			theArray[index] = theArray[arraySize];
			if ( arraySize < currentMaxSize / 3 ) shrink();
			return true;
		}
	}

	unsigned int size() { return arraySize; }
};