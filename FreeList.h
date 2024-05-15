#pragma once
#include <type_traits>
#include <initializer_list>
#include <new>
#include <cassert>
#include <stddef.h>
#include "FreeListNode.h"
#pragma warning(disable : 4101)
#pragma warning(disable : 4984)
#define PROFILE
#include "myProfiler.h"

template <typename T>
class FreeList
{
public:
	FreeList(bool constr_dest_on_alloc_free)
		:constr_dest_on_alloc_free_{ constr_dest_on_alloc_free }, head_{}
#ifdef _DEBUG
		, capacity_{ 0 }, size_{ 0 }
#endif
	{
		top_ = &head_;
	}
#ifdef _DEBUG
	// ��������Ʈ�� ��尡 �ִ뿳������ ����, �ᱹ �Ҹ��� ȣ��������� capacity_ != size_�̸� �޸� ������ �߻��Ѱ�
	int capacity_;
	// ���� �������մ� ��������Ʈ�� ����
	int size_;
#ifdef DETECT_MEMORY_LEAK
	// �Ҹ��ڿ����� ȣ��Ǿ�� �ϴ� �Լ�
	__forceinline bool isMemoryLeak()
	{
		return capacity_ != size_;
	}
#endif
#endif

	~FreeList()
	{
		if (IsEmpty())
			return;
		FreeListNode<T>* temp;
		while (top_->pNext)
		{
			FreeListNode<T>* temp = top_;
			top_ = top_->pNext;
			delete temp;
		}
	}
	
	// constr_dest_on_alloc_free_�� false�ε� ȣ���ϸ� ���α׷� ũ������
	template<typename U = T,typename V,typename = std::enable_if_t<std::is_class_v<U>>>
	FreeListNode<T>* GetNodeFromHeapOrDie(const std::initializer_list<V>& init_list)
	{
	assert(constr_dest_on_alloc_free_ == true && "You must set constr_dest_on_alloc_free to true to use this function");
#ifdef _DEBUG
		++capacity_;
#endif
		return new FreeListNode<T>{ init_list };
	}

// Ÿ�Ի������.
	__forceinline FreeListNode<T>* GetNodeFromHeap()
	{
#ifdef _DEBUG
		++capacity_;
#endif
		return new FreeListNode<T>;
	}

	__forceinline FreeListNode<T>* Fetch()
	{
		FreeListNode<T>* node_to_user = Top();
		Pop();
		return node_to_user;
	}
	
	template<typename U = T, typename V, typename = std::enable_if_t<std::is_class_v<U>>>
	T* AllocOrDie(std::initializer_list<V> init_list)
	{
		assert(constr_dest_on_alloc_free_ == true && "You must set constr_dest_on_alloc_free to true to use init_list");
		FreeListNode<T>* node_to_user;
		if (IsEmpty())
			node_to_user = GetNodeFromHeapOrDie(init_list);
		else
		{
			node_to_user = Fetch();
			if(constr_dest_on_alloc_free_)
				new (&node_to_user->data) T(init_list);
		}

		return &node_to_user->data;
	}

	T* Alloc(void)
	{
		FreeListNode<T>* node_to_user;
		if (IsEmpty())
		{
			node_to_user = GetNodeFromHeap();
		}
		else
		{
			node_to_user = Fetch();
			if (constr_dest_on_alloc_free_)
				new (&node_to_user->data) T{};
		}
		return &node_to_user->data;
	}
	void Free(T* p)
	{
		{
			if (constr_dest_on_alloc_free_)
			{
				p->~T();
			}
		}
		{
			int offset = offsetof(FreeListNode<T>, data);
			FreeListNode<T>* node = reinterpret_cast<FreeListNode<T>*>(reinterpret_cast<char*>(p) - offset);
			Push(node);
		}
	}
	bool constr_dest_on_alloc_free_;
	FreeListNode<T> head_;
	FreeListNode<T>* top_;
	__forceinline bool IsEmpty()
	{
		return top_ == &head_;
	}


	__forceinline void Push(FreeListNode<T>* pData)
	{
		FreeListNode<T>* next_node = top_;
		top_ = pData;
		top_->pNext = next_node;
#ifdef _DEBUG
		++size_;
		if(size_ > capacity_)
			capacity_ = size_;
#endif 
	}
	__forceinline FreeListNode<T>* Top()
	{
		if(IsEmpty())
			return nullptr;

		return top_;
	}

	__forceinline void Pop()
	{
		if (IsEmpty())
			return;

		FreeListNode<T>* pop_target = top_;
		top_ = top_->pNext;
#ifdef _DEBUG
		--size_;
		if (size_ < 0) size_ = 0;
#endif
	}
};



