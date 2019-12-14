#include <iostream>
#include "DynIntStack.h"
using namespace std;
// Efe Sencan 
//************************************************
// Constructor to generate an empty stack.       *
//************************************************
DynIntStack::DynIntStack()
{
	top = NULL;
	isTrue = true;
}

//************************************************
// Member function push pushes the argument onto *
// the stack.                                    *
//************************************************
void DynIntStack::push(string & push_name, int & number)
{
	if(isTrue)
	{
		StackNode *newNode;

		// Allocate a new node & store Num
		newNode = new StackNode;
		newNode->name = push_name;
		newNode->number = number; // it was newNode->number = 1;

		// If there are no nodes in the list
		// make newNode the first node
		if (isEmpty())
		{
			top = newNode;
			newNode->next = NULL;
		}
		else	// Otherwise, insert NewNode before top
		{
			newNode->next = top;
			top = newNode;
		}
	}
}

//****************************************************
// Member function pop pops the value at the top     *
// of the stack off, and copies it into the variable *
// passed as an argument.                            *
//****************************************************
void DynIntStack::pop(string & pop_name,int & number)
{
	StackNode *temp;

	if (isEmpty())
	{
		cout << "The stack is empty.\n";
	}
	else	// pop value off top of stack
	{
		pop_name = top->name;
		number = top->number;
		temp = top->next;
		delete top;
		top = temp;
	}
}

//****************************************************
// Member funciton isEmpty returns true if the stack *
// is empty, or false otherwise.                     *
//****************************************************
bool DynIntStack::isEmpty(void)
{
	bool status;

	if (top == NULL)
		status = true;
	else
		status = false;

	return status;
}

void DynIntStack::addItem(string & word_name,DynIntStack & temp_stack) // this changes the value of the private data 'isTrue' if the word is already exists in the stack
{
	isTrue = true;
	string name = "";
	int number= 1; 
	bool last_check= false;
	while(!isEmpty() && isTrue)
	{
		if(word_name == top->name)
		{
			top->number = top->number+1;
			isTrue = false;
			last_check = true;
		}
		else
		{
			pop(name,number);
			temp_stack.push(name,number);
		}
	}
	isTrue = true;	
	while(!temp_stack.isEmpty())
	{
		temp_stack.pop(name,number);
		push(name,number);
	}
	if(last_check)
	{
		isTrue = false;
	}
}

StackNode * DynIntStack::getTop() // this the returns the pointer which points to the top of the stack
{
	return top;
}

void DynIntStack::isFalse()
{
	if(isTrue == false)
	{
		isTrue = true;
	}
}

DynIntStack::~DynIntStack() // this is for deleting the objects and data's which was allocated dynamically
{
	string name = "";
	int number = 1;
	while(!isEmpty())
	{
		pop(name,number);
	}
	delete top;
}

