#ifndef LIST
#define LIST

template <typename T>
struct Node {
	T data;
	Node* next = nullptr;
};

template <class T>
class List {
private:
    Node<T>* head;
public:
	List();
	bool push(const T value);
	T pop();
	bool remove();
	int size(); 
    Node<T>* getNode(int);
    Node<T>* getHead();
    void reverse();
    ~List();
};

template <class T>
void List<T>::reverse()
{
    Node<T>* current = head;
    Node<T>* prev = NULL, * next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}

template <class T>
List<T>::List()
{
    head = nullptr;
}

template <class T>
bool List<T>::push(T data)
{
    Node<T>* temp = new Node<T>();
    (*temp).data = data;
    (*temp).next = nullptr;
    if (head)
    {
        temp->next = head;
        head = temp;
        return true;
    }
    head = temp;
}

template <class T>
T List<T>::pop()
{
    if (head)
    {
        return head->data;
    }
    std::cout << "Invalid pop()\nExiting...";
    std::exit(1);
}

template <class T>
Node<T>* List<T>::getHead()
{
    return head;
}

template <class T>
bool List<T>::remove() {
    if (head) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        return true;
    }
    return false;
}

template <class T>
int List<T>::size()
{
    int count = 0;
    if (head) {
        count = 1;
        Node<T>* temp = head;
        while (temp->next)
        {
            count++;
            temp = temp->next;
        }
    }
    return count;
}

template <class T>
Node<T>* List<T>::getNode(int index)
{
	Node<T>* temp = head;
    if (index == 0) {
        return temp;
    }

    for (int i = 1; i <= index && temp; i++)
        temp = temp->next;

    if(temp){
        return temp;
    }

}

template <class T>
List<T>::~List()
{
	Node<T>* temp = head;
	while (temp)
	{
		head = head->next;
		delete temp;
		temp = head;
	}
}

#endif