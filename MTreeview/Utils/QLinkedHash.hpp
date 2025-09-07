/**
 * @file	QLinkedHash.hpp
 * @brief	Implementation of a linked hash container using Qt.
 *
 * QLinkedHash uses a combination of QHash and QList to provide a linked hash container,
 * allowing for both fast lookups (via QHash) and ordered access (via QList).
 * 
 * @tparam T storage type of the value
 */

#pragma once

#include <QList>
#include <QHash>

#include <stdexcept>

template<typename Key, typename T>
class QLinkedHash
{
	static_assert(!std::is_same<int, Key>::value,
		"The Key type of 'QLinkedHash' can't be 'int', and have to be supported by 'qHash()'");
public:
	/// Default constructor
	QLinkedHash() {}

	/// Copy constructor
	QLinkedHash(const QLinkedHash& other)
		: m_hash(other.m_hash), m_list(other.m_list) {}

	/// Destructor
	~QLinkedHash() { clear(); }

	/**************************************************************************************
	 * 
	 * @brief Obtains the number of elements in the container.
	 * 
	 * ------------------------------------------------------------------------------------
	 * 
	 * @return | <int> |the number of elements in the container.
	 * 
	 **************************************************************************************/
	int size() const 
	{ 
		return m_list.size(); 
	}

	/**************************************************************************************
	 * 
	 * @brief Checks if the container is empty.
	 * 
	 * ------------------------------------------------------------------------------------
	 * 
	 * @return | <bool> |if the container is empty, returns true; otherwise returns false.
	 * 
	 **************************************************************************************/
	bool empty() const
	{
		return m_list.isEmpty();
	}

	/**************************************************************************************
	 *
	 * @brief Clear all the elements in the container.
	 *
	 **************************************************************************************/
	void clear()
	{
		m_list.clear(); 
		m_hash.clear();
	}

	/**************************************************************************************
	 * 
	 * @brief Checks if the container contains the specified key.
	 * 
	 * ------------------------------------------------------------------------------------
	 * 
	 * @return | <bool> |if the container contains the specified key, returns true; 
	 *		   |		|otherwise returns false.
	 * 
	 **************************************************************************************/
	bool contains(const Key& key) const
	{
		return m_hash.contains(key);
	}

	/**************************************************************************************
	 * 
	 * @brief Converts the container to a QList.
	 *
	 * ------------------------------------------------------------------------------------
	 * 
	 * @return | QList<T> |the list of values in the container, in insertion order.
	 * 
	 **************************************************************************************/
	inline const QList<T> toList() const
	{
		QList<T> _list;

		for (int _i = 0; _i < m_list.size(); ++_i)
		{
			_list.push_back(m_hash[m_list[_i]]);
		}
		return _list;
	}

	/**************************************************************************************
	 * 
	 * @brief Access elements by index.
	 *
	 * Returns the value corresponding to the specified index.
	 *
	 * ------------------------------------------------------------------------------------
	 * 
	 * @param	| idx <int>	|index position, must be in the range [0, size()).
	 * @return	| <const T> |if the index is valid, returns the corresponding value; 
	 *			|			|otherwise returns a default-constructed T object.
	 *			|
	 * @note1	|No support for returning references for direct assignment, 
	 *			|as there are no corresponding scenarios. 
	 *			|Also, since the [] operator may implicitly insert new key-value pairs, 
	 *			|thus it is not supported.
	 *			|
	 * @note2	|Complexity of this operation is O(1).
	 * 
	**************************************************************************************/
	inline const T operator[](int idx) const
	{
		if (idx < m_list.size() && idx >= 0)
		{
			return m_hash[m_list[idx]];
		}
		return T();
	}

	/**************************************************************************************
	 * 
	 * @brief Access elements by key.
	 *
	 * Returns the value corresponding to the specified key.
	 *
	 * ------------------------------------------------------------------------------------
	 *
	 * @param	| key <Key>	|key.
	 * @return	| <const T> |if the key exists, returns the corresponding value;
	 *			|			|otherwise returns a default-constructed T object.
	 *			|
	 * @note1	|No support for returning references for direct assignment,
	 *			|as there are no corresponding scenarios.
	 *			|Also, since the [] operator may implicitly insert new key-value pairs,
	 *			|thus it is not supported.
	 *			|
	 * @note2	|Complexity of this operation is O(1).
	 * 
	**************************************************************************************/
	inline const T operator[](const Key& key) const
	{
		if (m_hash.contains(key))
		{
			return m_hash[key];
		}
		return T();
	}

	/**************************************************************************************
	 * 
	 * @brief	Obtains the value of the first element in the container.
	 *
	 * If the container is empty, an out_of_range exception is thrown.
	 *
	 * ------------------------------------------------------------------------------------
	 * 
	 * @return	| T	|the value of the first element.
	 *			|
	 * @note	|Complexity of this operation is O(1).
	 * 
	 **************************************************************************************/
	inline const T front() const
	{
		if (m_list.isEmpty())
		{
			throw std::out_of_range("Empty Container.");
		}
		return m_hash[m_list.front()];
	}

	/**************************************************************************************
	 * 
	 * @brief	Obtains the value of the last element in the container.
	 *
	 * If the container is empty, an out_of_range exception is thrown.
	 *
	 * ------------------------------------------------------------------------------------
	 *
	 * @return	| T	|the value of the last element.
	 *			|
	 * @note	|Complexity of this operation is O(1).
	 * 
	 **************************************************************************************/
	inline const T back() const
	{
		if (m_list.isEmpty())
		{
			throw std::out_of_range("Empty Container.");
		}
		return m_hash[m_list.back()];
	}

	 /**************************************************************************************
	  * 
	  * @brief Obtains the index of the specified key.
	  *
	  * Returns the index of the specified key if it exists; otherwise returns -1.
	  *
	  * ------------------------------------------------------------------------------------
	  *
	  * @param	| key <Key>	|key.
	  * @return	|   <int>	|if the key exists, returns the corresponding index;
	  *			|			|otherwise returns -1.
	  *			|
	  * @note	|Complexity of this operation is O(N), N is the length of the list.
	  * 
	 **************************************************************************************/
	inline int indexOf(const Key& key) const
	{
		return m_list.indexOf(key);
	}

	/**************************************************************************************
	 * 
	 * @brief Inserts a key-value pair at the specified index.
	 *
	 * If the key already exists, the existing key-value pair is removed first.
	 *
	 * ------------------------------------------------------------------------------------
	 * 
	 * @param	| idx <int> |index position, must be in the range [0, size()].
	 * @param	| key <Key>	|key.
	 * @param	|  val <T>	|value to be inserted.
	 *			|
	 * @note	|Complexity of this operation is O(N) in the worst case.
	 * 
	 **************************************************************************************/
	inline void insert(int idx, const Key& key, const T& val)
	{
		if (idx <= m_list.size() && idx >= 0)
		{
			if (m_hash.contains(key))
			{
				m_list.removeOne(key);
			}

			m_list.insert(idx, key);
			m_hash[key] = val;
		}
	}

	/**************************************************************************************
	 * 
	 * @brief Appends a key-value pair to the end of the container.
	 *
	 * Same as insert(size(), key, val).
	 *
	 * ------------------------------------------------------------------------------------
	 * 
	 * @param	| key <QString> |key to be appended.
	 * @param	|   val <T>		|value to be appended.
	 *			|
	 * @note	|Complexity of this operation is O(N) in the worst case, same as insert.
	 *			|But for a new key, the complexity is O(1).
	 * 
	 **************************************************************************************/
	inline void append(const QString& key, const T& val)
	{
		return insert(size(), key, val);
	}

	/**************************************************************************************
	 * 
	 * @brief Removes the key-value pair with the specified key.
	 * 
	 * ------------------------------------------------------------------------------------
	 * 
	 * @param	| key <QString> |key to be removed.
	 *			|
	 * @note	|Complexity of this operation is O(N) in the worst case.
	 * 
	 **************************************************************************************/
	void remove(const QString& key)
	{
		if (m_hash.contains(key))
		{
			m_hash.remove(key);
			m_list.removeOne(key);
		}
	}

	/**************************************************************************************
	 * 
	 * @brief Removes the key-value pair at the specified index.
	 * 
	 * ------------------------------------------------------------------------------------
	 * 
	 * @param	| idx <int>	|index position, must be in the range [0, size()).
	 *			|
	 * @note	|Complexity of this operation is O(N) in the worst case.
	 * 
	 **************************************************************************************/
	void removeAt(int idx)
	{
		if (idx < m_list.size() && idx >= 0)
		{
			m_hash.remove(m_list[idx]);
			m_list.removeAt(idx);
		}
	}

	/**************************************************************************************
	 * 
	 * @class Iterator
	 *
	 * @brief defines an iterator for QLinkedHash.
	 * 
	 **************************************************************************************/
	class Iterator
	{
	public:
		Iterator(QLinkedHash* container, int index)
			: m_container(container), m_index(index) {
		}

		///< Dereference operator
		T& operator*() { return m_container->m_hash[m_container->m_list[m_index]]; }

		///< prefix increment
		Iterator& operator++()
		{
			++m_index;

			return *this;
		}

		///< postfix increment
		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		///< Comparison operators
		bool operator==(const Iterator& other) const
		{
			return m_container == other.m_container && m_index == other.m_index;
		}

		bool operator!=(const Iterator& other) const
		{
			return !(*this == other);
		}

	private:
		QLinkedHash* m_container;
		int m_index;
	};

	// Returns an iterator to the beginning of the container
	Iterator begin() { return Iterator(this, 0); }
	// Returns an iterator to the end of the container
	Iterator end() { return Iterator(this, m_list.size()); }

private:
	QHash<Key, T>	m_hash;		///< Hash table for fast key-value lookups
	QList<Key>		m_list;		///< List to maintain insertion order
};