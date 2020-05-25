#include <iostream>


template<typename T>
class MyList
{
private:

	template<typename T>
	class Node
	{
	public:
		T data;

		Node *next;
		//Node *prev;

		Node(T data, Node *next = nullptr)
		{
			this->data = data;
			this->next = next;
		}
	};

	size_t size;
	Node<T> *head;
	//Node<T> *tail;


public:

	MyList()
	{
		size = 0;
		head = nullptr;
	}

	~MyList();

	/*
	void Put(T data)
	{
		if (head == nullptr)
		{
			head = new Node<T>(data);
		}
		else
		{
			Node<T> *cur = this->head;
			while (cur->next != nullptr)
			{
				cur = cur->next;
			}
			cur->next = new Node<T>(data);
		}
		size++;
	}
	*/
	int GetSize()
	{
		return size;
	}

};




int main()
{

	MyList<int> lsd;
    
	return 0;
}
