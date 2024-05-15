#pragma once
#include <iostream>
template<typename T>
class MyList
{
public:
	struct Node
	{
		T _data;
		Node* _prev;
		Node* _next;

		Node(const T& data = T())
			: _data{ data }, _prev{ nullptr }, _next{ nullptr } {}

		~Node() {}
	};

	Node _head;
	Node _tail;
	size_t _size;
	MyList()
		:_size{ 0 }
	{
		_head._next = &_tail;
		_tail._prev = &_head;
	}

	~MyList()
	{
		clear();
	}

	class iterator
	{
	public:
		Node* _node;
		iterator(Node* node = nullptr)
		{
			_node = node;
		}

		iterator& operator= (const iterator& other)
		{
			_node = other._node;
			return *this;
		}

		iterator& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		iterator& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		T& operator *() const
		{
			return _node->_data;
		}

		T* operator->()
		{
			return &_node->_data;
		}

		friend bool operator ==(const iterator& i1, const iterator& i2)
		{
			return i1._node == i2._node;
		}

		friend bool operator !=(const iterator& i1, const iterator& i2)
		{
			return i1._node != i2._node;
		}
	};

	bool empty()
	{
		return _size == 0;
	}
	iterator begin()
	{
		return iterator{ _head._next };
	}

	iterator end()
	{
		return	iterator{ &_tail };
	}

	void push_front(const T& data)
	{
		Node* temp = new Node{ data };
		temp->_prev = &_head;
		temp->_next = _head._next;
		_head._next->_prev = temp;
		_head._next = temp;
		++_size;
	}

	void push_back(const T& data)
	{
		Node* temp = new Node{ data };
		temp->_next = &_tail;
		temp->_prev = _tail._prev;
		_tail._prev->_next = temp;
		_tail._prev = temp;
		++_size;
	}

	void pop_front()
	{
		if (empty()) return;
		Node* temp = _head._next;
		_head._next = temp->_next;
		temp->_next->_prev = &_head;
		delete temp;
	}

	void pop_back()
	{
		if (empty()) return;
		Node* temp = _tail._prev;
		_tail._prev = temp->_prev;
		temp->_prev->_next = &_tail;
		delete temp;
	}

	iterator erase(iterator& target)
	{
		iterator ret_iter{ target._node->_next };
		target._node->_prev->_next = target._node->_next;
		target._node->_next->_prev = target._node->_prev;
		delete target._node;
		--_size;

		return ret_iter;
	}

	void remove(const T& data)
	{
		MyList<T>::iterator iter;
		for (iter = begin(); iter != end(); ++iter)
		{
			if (*iter == data)
				iter = erase(iter);
		}
	}

	void clear(void)
	{
		MyList<T>::iterator iter;
		for (iter = begin(); iter != end();)
		{
			iter = erase(iter);
		}
	}



#ifdef PRINT
	void print()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			std::cout << it._node->_data << ' ';
		}
		std::cout << '\n';
	}
#endif 
};

