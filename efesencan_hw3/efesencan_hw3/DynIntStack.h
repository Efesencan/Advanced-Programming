#ifndef _DYNINTSTACK_H
#define _DYNINTSTACK_H
// Efe Sencan 25083
#include <string>
using namespace std;
struct StackNode
{
	int number; // this number would be occurrence of the word int the same file.
	string name; // this will be the name of the word
	StackNode *next; // next will be pointer of our data type
	StackNode(string & input_name,StackNode * & ptr,int & input_number)  // forming the constructor
	{
		name = input_name;
		next = ptr;
		number = input_number;
	}

	StackNode() {}; // forming the default constructor
};

class DynIntStack
{
private:
	StackNode * top;
	bool isTrue;

public:
	DynIntStack(); // the constructor of the class
	~DynIntStack(); // the destructor of the class
	void push(string & push_name, int & number);
	void pop(string & pop_name, int & number);
	void addItem(string & word_name,DynIntStack & temp_stack); 
	StackNode * getTop();
	bool isEmpty(void);
	void isFalse(void);
};

#endif