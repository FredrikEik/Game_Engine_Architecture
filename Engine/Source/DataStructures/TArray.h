#pragma once
#include "../CoreMinimal.h"
//#include "../Components/Components.h"

template <class T>
class TArray
{
public:
	TArray();
	~TArray();
	void push_back(T element);
	T& back();
	T& at(uint64 element);
	void pop_back();
	void erase(uint64 element);
	T& operator[](uint64 element);
	size_t size();
	T* begin();
	T* end();
private:
	void resize(uint64 amountOfElements);
	
	T* array{};
	char* byteArray{};
	uint64 nextElement{};
	size_t sizeOfElement{};
	size_t sizeOfArray{};
};

template<class T>
inline TArray<T>::TArray()
{
	array = (T*)calloc(3, sizeof(T));
	byteArray = (char*)array;
	sizeOfArray = sizeof(T)*3;
	sizeOfElement = sizeof(T);
}

template<class T>
inline TArray<T>::~TArray()
{
	//delete(array, sizeOfArray*sizeOfElement);
	free(array);
}

template<class T>
inline void TArray<T>::push_back(T element)
{
	// push back
	if (nextElement * sizeOfElement > sizeOfArray)
		resize(nextElement * 2);

	// Might have to do sizeOfElement * end
 	array[nextElement] = element;

	nextElement++;
}

template<class T>
inline T& TArray<T>::back()
{
	return array[nextElement - 1];
}

template<class T>
inline T& TArray<T>::at(uint64 element)
{
	return array[element];
}

template<class T>
inline void TArray<T>::pop_back()
{
	erase(nextElement - 1);
}

template<class T>
inline void TArray<T>::erase(uint64 element)
{
	if(element != nextElement-1)
		for (uint64 i{}; i < sizeOfElement; ++i)
		{
			byteArray[i + element] = byteArray[i + ((nextElement - 1)*sizeOfElement)];
		}

	--nextElement;
}

template<class T>
inline T& TArray<T>::operator[](uint64 element)
{
	return array[element];
}

template<class T>
inline size_t TArray<T>::size()
{
	return nextElement;
}

template<class T>
inline T* TArray<T>::begin()
{
	return array;
}

template<class T>
inline T* TArray<T>::end()
{
	return array[end-1];
}

template<class T>
inline void TArray<T>::resize(uint64 amountOfElements)
{
	//tempArray = new(sizeOfElement * amountOfElements) T;
	realloc(array, sizeOfElement * amountOfElements);
	sizeOfArray = sizeOfElement * amountOfElements;
}
