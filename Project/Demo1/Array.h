#ifndef ARRAY_CLASS_DEFINE
#define ARRAY_CLASS_DEFINE
#include <assert.h>
#include <string.h>
typedef unsigned int t_size;

template<class T>
class Array
{
public:
	Array()
	{
		m_size = 0;
		m_array = 0;
		m_bDeleteArray = true;
	}

	Array(t_size size)
	{
		m_size = 0;
		m_array = 0;
		m_bDeleteArray = true;
		Resize(size);
	}

	Array(const Array &other)
	{
		m_array = other.GetArray();
		m_size = other.GetSize();
	}

	~Array()
	{
		if(m_array && m_bDeleteArray)
		{
			delete[] m_array;
		}
	}

	void ClearArray()
	{
		if(m_array)
		{
			delete[] m_array;
			m_size = 0;
			m_array = 0;
		}
	}

	bool m_bDeleteArray;

	t_size GetSize() const				{ return m_size; }

	void Resize(t_size newSize)
	{
		if(newSize == 0)
		{
			return;
		}

		t_size oldSize = m_size;
		T* tempStorage = m_array;
		m_array = new T[newSize];

		for(t_size i = 0; i < oldSize && i < newSize; ++i)
		{
			m_array[i] = tempStorage[i];
		}

		if(tempStorage)
		{
			delete[] tempStorage;
		}

		m_size = newSize;
	}

	T& operator[](t_size index)
	{
		if(index < 0 || index >= m_size)
		{
			assert(false);
		}
		return m_array[index];
	}

	T operator[](t_size index) const
	{
		if(index < 0 || index >= m_size)
		{
			assert(false);
		}
		return m_array[index];
	}

	T *GetArray() const
	{
		return m_array;
	}

	bool Find(const T toFind, t_size *indexOut = 0) const
	{
		for(int i = 0; i < m_size; ++i)
		{
			if(m_array[i] == toFind)
			{
				if(indexOut != 0)
				{
					*indexOut = i;
				}
				return true;
			}
		}

		return false;
	}

	bool Find(const T toFind, const t_size startAt, t_size *indexOut = 0) const
	{
		for(int i = startAt; i < m_size; ++i)
		{
			if(m_array[i] == toFind)
			{
				if(indexOut != 0)
				{
					*indexOut = i;
				}
				return true;
			}
		}

		return false;
	}

	t_size FindNumberOf(const T toFind, const t_size startAt, const t_size endAt) const
	{
		t_size start(startAt),end(m_size - 1);

		if(endAt < m_size)
		{
			end = endAt;
		}

		t_size numberOf = 0;
		for(int i = start; i <= end; ++i)
		{
			if(m_array[i] == toFind)
			{
				++numberOf;
			}
		}

		return numberOf;
	}

	t_size FindNumberOf(const T toFind) const
	{
		t_size start(0),end(m_size);

		t_size numberOf = 0;
		for(int i = start; i < end; ++i)
		{
			if(m_array[i] == toFind)
			{
				++numberOf;
			}
		}

		return numberOf;
	}

	void InsertAt(T item, const t_size index)
	{
		if(index >= m_size)
		{
			return;
		}

		memcpy(m_array + index + 1, m_array + index, (m_size - index - 1) * sizeof(T));
		m_array[index] = item;
	}

private:
	T* m_array;
	t_size m_size;
};

// important to note: memory storage is not the same as normal array, char* 's, bit shifters, and bit-wise operators 
//	are used instead to save on memory
class FlagArray
{
public:

	FlagArray()
	{
		m_size = 0;
		m_array = 0;
	}

	FlagArray(const t_size size)
	{
		m_size = 0;
		m_array = 0;

		Resize(size);
	}

	t_size GetSize() { return m_size * k_storageContainerSize; }

	void Resize(const t_size newSize)
	{
		t_size realNewSize = (t_size)(newSize / 8) + (newSize % 8 > 0 ? 1 : 0);
		t_size oldSize = m_size;
		char* tempStorage = m_array;
		m_array = new char[realNewSize];

		memset(m_array, 0, sizeof(char) * realNewSize);

		for(t_size i = 0; i < oldSize && i < realNewSize; ++i)
		{
			m_array[i] = tempStorage[i];
		}

		if(tempStorage)
		{
			delete[] tempStorage;
		}

		m_size = realNewSize;
	}

	bool operator[] (const t_size index) const
	{ 
		t_size realIndex = 0;// = index / k_storageContainerSize;
		unsigned char mask = 0;// = 1 << (index % k_storageContainerSize);
		AdjustedIndex(index, &realIndex, &mask);
		return (m_array[realIndex] & mask) > 0; 
	}

	void SetFlag(t_size index, bool flag)
	{
		t_size realIndex = 0;
		unsigned char mask = 0;
		AdjustedIndex(index, &realIndex, &mask);

		char flags = m_array[realIndex];

		if(flag)
		{
			flags = flags | mask;
		}
		else
		{
			flags = flags & (~mask);
		}

		m_array[realIndex] = flags;
	}

	void *GetArray()
	{
		return m_array;
	}

private:
	char* m_array;
	t_size m_size;
	const static unsigned int k_storageContainerSize = 8;

	void AdjustedIndex(const t_size index, t_size *realIndexOut, unsigned char *mask) const
	{
		*realIndexOut = index / k_storageContainerSize;
		*mask = 1 << (index % k_storageContainerSize);
	}
};

#endif