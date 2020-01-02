//get lenght of a list
int getCount(Node* head)
{
	int count = 0; // Initialize count  
	Node* current = head; // Initialize current  
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}

//get middle node
Node* getMiddle(Node* head, int lenght)
{
	for (int i = 0; i < lenght / 2;i++)
	{
		ptr = ptr->next;
	}
	return ptr;
}

Node* searchIfExist(int num, Node* head, int lenght)
{
	Node* ptr = head;
	len = lenght;
	Node* check = getMiddle(head, len);
	while (true)
	{
		len /= 2;
		if (check->num == num)
			return check;
		if (check->num > num)
			getMiddle(ptr, len);
		else
		{
			getMiddle(check->next, len);
			ptr = check->next;
		}
	}
}

Node* find_previous(Node* head, Node* ptr)
{
	Node* h = head;
	while (h->next != ptr)
		h = h->next;
	return h;
}
//insert a node
void insert(Node* head, int num)
{
	//just if isn't exist
	len = getCount(head);
	Node* ptr = searchIfExist(num, head, len);
	Node* _new = new Node();
	_new->num = num;
	if (ptr->num < num)
	{
		_new->next = ptr->next;
		ptr->next = _new;
	}
	else
	{
		_new->next = ptr;
		find_previous(head, ptr)->next = _new;
	}
}



