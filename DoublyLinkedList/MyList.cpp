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
		Node *prev;

		Node(T data, Node *next, Node *prev)
		{
			this->data = data;
			this->next = next;
			this->prev = prev;
		}
	};

	size_t size;
	Node<T> *head;
	Node<T> *tail;


public:

	MyList()
	{
		size = 0;
		head = nullptr;
	}
	

	~MyList()
	{

	}

	T& operator[](const size_t index)
	{
		Node<T> *cur;

		if (index <= size / 2)
		{
			cur = head;
			for (size_t i = 0; i < index; i++)
			{
				cur = cur->next;
			}
			return cur->data;
		}
		else
		{
			cur = tail;
			for (size_t i = size - 1; i > index; i--)
			{
				cur = cur->prev;
			}
			return cur->data;
		}
	}

	void Put(T data)
	{
		if (head == nullptr)
		{
			head = new Node<T>(data, nullptr, nullptr);
			tail = head;
		}
		else
		{
			Node<T> *cur = new Node<T>(data, nullptr, tail);
			tail->next = cur;
			tail = cur;
		}

		size++;
	}

	void Delete(size_t index)
	{

		if (index == 0)
		{
			Node<T>* t = head;
			head = head->next;
			delete t;
			size--;
			return;
		}

		if (index == size - 1)
		{
			Node<T>* t = tail;
			tail = tail->prev;
			delete t;
			size--;
			return;
		}

		Node<T> *p;

		if (index < size / 2)
		{
			p = head;

			for (size_t i = 0; i < index; i++)
			{
				p = p->next;
			}

			p->prev->next = p->next;
			p->next->prev = p->prev;

			delete p;

		}
		else
		{
			p = tail;

			for (size_t i = size - 1; i > index; i--)
			{
				p = p->prev;
			}

			p->prev->next = p->next;
			p->next->prev = p->prev;

			delete p;

		}

		size--;
	}

	void Clear()
	{
		for (size_t i = 0; i < size; i++)
		{
			Node<T> *t = head;
			head = head->next;
			delete t;
		}

		size = 0;
	}

	size_t Size()
	{
		return size;
	}


};






int main()
{
	setlocale(0, "");



	MyList<int> list;


	for (int i = 0; i < 10; i++)
		list.Put(i);



	printf("Исходные данные:\nsize = %d\n", list.Size());
	for (size_t i = 0; i < list.Size(); i++)
	{
		//printf("i = %d, %d\n", i, list[i]);
		printf("%d\n", list[i]);
	}
	printf("\n\n");


	list.Delete(5);


	printf("После удаления:\nsize = %d\n", list.Size());
	for (size_t i = 0; i < list.Size(); i++)
	{
		printf("%d\n", list[i]);
	}
	printf("\n\n");

	list.Clear();
	list.Put(228);
	list.Put(12);
	list.Put(2546);

	printf("После очищения и добавления нового значения:\nsize = %d\n", list.Size());
	for (size_t i = 0; i < list.Size(); i++)
	{
		printf("%d\n", list[i]);
	}
	printf("\n\n");


	std::cin.get();
	return 0;
}
