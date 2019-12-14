#include <iostream>
#include <string>
#include <sstream>
#include "strutils.h"

using namespace std;

// Efe Sencan
// This program implemets a dynamic shopping list where the user can add or delete an item by using circular doubly linked list
struct node
{
	string name;
	int quantity;
	int item_price;
	int total_price;
	node * next;
	node * previous;
	node(string shop_name,int shop_quantity,int shop_item_price,int shop_price,node * shop_next,node * shop_previous)
		:name(shop_name),quantity(shop_quantity),item_price(shop_item_price),total_price(shop_price),next(shop_next),previous(shop_previous)
	{}
};

void changeString(string & s)  // changing the format of the input
{
	string line = "", temp = "";
	istringstream sentence(s);
	while(sentence>>line)
	{
		line = line + " ";
		temp+=line;
	}
	
	s = temp.substr(0,temp.length()-1);
}

void clearList(node * & head,int count) // freeing the memory that was created by the keyword 'new' after the user selects 'EXIT'
{
	if(count != 1 && head != NULL)
	{
		node *ptr = head;
		node * temp = head;
		do
		{
			ptr = head;
			head = head->next;
			delete ptr;
		}
		while(head != temp);
	}
}

void changeHead(node * & head, node * & temp) // updating the head of the node(actually head is only the item that has highest total price value)
{
	temp->previous = head->previous;
	temp->next = head;
	head->previous->next = temp;
	head->previous = temp;
	head = temp; 
}

void sortinOrder(node * & temp,node * & prev, node * & ptr) // sorting the node to the correct place
{
	temp->previous = ptr;
	temp->next = prev;
	ptr->next->previous = temp;
	ptr->next = temp;
}

void addbeforeHead(node * & head, node * & ptr, node * & temp) // adding the node before the head(althought there is no real head)
{
	temp->next = ptr;
	temp->previous = ptr->previous;
	ptr->previous->next = temp;
	ptr->previous = temp;
	head = temp;
}

void connectNodes(node * & temp) // forming the connection of the nodes when the node between them is shifted
{
	temp->next->previous = temp->previous;
	temp->previous->next = temp->next;
}

void sortList(node *& head,node * & temp) // when the same item was added to the list, the program updates it current position if needed
{
	node *prev;
	node * ptr = temp;
	node * hold_value = temp;
	if(temp->total_price > temp->previous->total_price) // if the updated version of the item's total price is still less then previous one, then there is no need to change the order 
	{
		connectNodes(temp);
		do
		{
			prev = ptr;
			ptr = ptr->previous;
		}
		while(ptr != head && ptr->total_price < temp->total_price);
		if(ptr->total_price == temp->total_price)
		{
			if(ptr->name < temp->name)
			{
				sortinOrder(temp,prev,ptr); // updating the place of the node
			}
			else
			{
				bool check = true;
				if(ptr == head)
				{
					check = false; 
					changeHead(head,temp); // the updated item will be the new head
				}
				if(check)  
				{
					do
					{
						prev = ptr;
						ptr = ptr->previous;
					}
					while(ptr != head && ptr->name > temp->name);
					if(ptr != head || head->name < temp->name)
					{
						temp->next = ptr;
						temp->previous = prev;
						prev->previous = temp;
						ptr->next = temp;
					}
					else
					{	
						changeHead(head,temp);
					}
				}

			}
		}
		else
		{
			if(ptr == head)
			{
				if(ptr->total_price > temp->total_price)
				{
					sortinOrder(temp,prev,ptr);
				}
				else
				{
					if(ptr->total_price == temp->total_price)
					{
						if(ptr->name < temp->name)
						{
							sortinOrder(temp,prev,ptr);
						}
						else
						{
							addbeforeHead(head,ptr,temp); 
						}
					}
					else
					{
						addbeforeHead(head,ptr,temp);
					}
				}
			}
			else
			{
				sortinOrder(temp,prev,ptr);
			}
		}
	}
	else if(temp->total_price == temp->previous->total_price)
	{
		if(temp->name < temp->previous->name)
		{
			connectNodes(temp);
			do
			{
				prev = ptr;
				ptr = ptr->previous;
			}
			while(ptr != head && ptr->name > temp->name && temp->total_price == ptr->total_price);
			if(temp->total_price == ptr->total_price)
			{
				if(ptr == head)
				{
					if(ptr->name < temp->name) 
					{
						sortinOrder(temp,prev,ptr);
					}
					else
					{
						changeHead(head,temp);
					}
				}
				else
				{
					sortinOrder(temp,prev,ptr);
				}
			}
			else
			{
				sortinOrder(temp,prev,ptr);
			}
		}
	}
}

void notFound(string & item_name) // checking if the item is in the list or not
{
	cout<<"---"<<endl;
	cout<<"The item "<<item_name<<" could not be found in the list."<<endl;
	cout<<"---"<<endl;
}
void deleteItem(node * & head,int & count) // deleting the item from the shopping list
{
	bool checking = true;
	node * temp;
	string item_name;
	cout<<"Enter an item name to delete from the list: ";
	getline(cin,item_name);
	ToUpper(item_name);
	changeString(item_name);
	if(count == 1 && head == NULL)
	{
		notFound(item_name);
	}
	else 
	{
		if(head == NULL)
		{
			notFound(item_name);
		}
		else
		{
			count = 0;
			node *control = head;
			do
			{
				count++;
				control = control->next;
			}
			while(control != head);
			temp = head;
			do
			{
				if(temp->name == item_name)
				{
					checking = false;
				}
				if(checking)
					temp = temp->next;
			}
			while(temp != head && checking);
			if(!checking)
			{
				int num = 0;
				node * check = head;
				do
				{
					check = check->next;
					num++;
				}
				while(check != head);
				if(temp == head && head->name == item_name)
				{
					if(num == 1)
					{
						delete head;
						head = NULL;
					}
					else
					{
						head->previous->next = head->next;
						head->next->previous = head->previous;
						head = head->next;
						delete temp;
					}
				}
				else
				{
					temp->previous->next = temp->next;
					temp->next->previous = temp->previous;
					delete temp;
				}
				cout<<"---"<<endl;
				cout<<"The item "<<item_name<<" is deleted from the list."<<endl;
				cout<<"---"<<endl;
			}
			else
			{
				notFound(item_name); // calling the notfound function if the given input item is not in the list
			}
		}
	}
}

void getItems(node * & head,string & item,int & quantity, int & price) // getting the feature of the item by the user
{
	cout<<"Enter name for the item: ";
	getline(cin,item);
	ToUpper(item);
	changeString(item);
	cout<<"Enter quantity for the item: ";
	cin>>quantity;
	cout<<"Enter estimated price for the item: ";
	cin>>price;
	head = new node(item,quantity,price,price*quantity,NULL,NULL);
	head->next = head;
	head->previous = head; 
	cout<<"---"<<endl;
	cout<<"The item "<<item<<" of quantity "<<quantity<<" is added to the list."<<endl;
	cout<<"---"<<endl;
}
void addItem(node * & head,int & count) // adding an item to the list entered by the user
{
	string item;
	/*cin.clear();*/
	int quantity,price;
	node *ptr;
	if(head == NULL)
	{
		
		getItems(head,item,quantity,price);
	}
	else
	{
		if(count == 1 && head == NULL)
		{
			getItems(head,item,quantity,price);
			count++;
		}
		else
		{
			cout<<"Enter name for the item: ";
			getline(cin,item);
			ToUpper(item);
			changeString(item);
			bool isTrue = true,check = true;
			node * temp = head;
			do
			{
				if(temp->name == item) // if the given item is already in the list, then we will not ask for the price pf it
				{
					ToUpper(item);
					cout<<"Enter quantity for the item: ";
					cin>>quantity;
					temp->quantity+= quantity;
					temp->total_price = temp->quantity*temp->item_price;
					check = false;
					isTrue = false;
					cout<<"---"<<endl;
					cout<<"The quantity of the item "<<temp->name<<" is increased by "<<quantity<<". The final quantity is "<<temp->quantity<<"."<<endl;
					cout<<"---"<<endl;
					sortList(head,temp); // we should also updatethe place of the item if needed 
				}
				temp = temp->next;
			}
			while(temp != head && check);
			if(isTrue)
			{
				cout<<"Enter quantity for the item: ";
				cin>>quantity;
				cout<<"Enter estimated price for the item: ";
				cin>>price;
				int total_price = price*quantity;
				if(head->total_price < total_price)
				{
					ptr = new node(item,quantity,price,price*quantity,head,head->previous);
					head->previous->next = ptr;
					head->previous = ptr;
					head = ptr;
				}
				else if(head->total_price == total_price)
				{
					if(item < head->name)
					{
						ptr = new node(item,quantity,price,price*quantity,head,head->previous);
						head->previous->next = ptr;
						head->previous = ptr;
						head = ptr;
					}
					else
					{
						node * same_prev,*same_point = head;
						do
						{
							same_prev = same_point;
							same_point = same_point->next;
						}
						while(same_point != head && same_point->name < item && same_point->total_price == total_price);
						same_prev->next = new node(item,quantity,price,price*quantity,same_point,same_prev);
						same_prev->next->next->previous = same_prev->next;
					}
				}
				else
				{
					node * prev,*point = head;
					do
					{
						prev = point;
						point = point->next;
					}
					while(point != head && point->total_price > total_price); 
					if(point->total_price == total_price)
					{
						if(item < point->name)
						{
							prev->next = new node(item,quantity,price,price*quantity,point,prev);
							prev->next->next->previous = prev->next;
						}
						else
						{
							if(point->next->total_price == total_price)
							{
								do
								{
									prev = point;
									point = point->next;
								}
								while(point != head && point->name < item);
								prev->next = new node(item,quantity,price,price*quantity,point,prev);
								point->previous = prev->next;
							}
							else
							{
								point->next = new node(item,quantity,price,price*quantity,point->next,point);
								point->next->next->previous = point->next;
							}

						}
					}
					else
					{
						prev->next = new node(item,quantity,price,price*quantity,point,prev);
						prev->next->next->previous = prev->next;
					}
				}
				cout<<"---"<<endl;
				cout<<"The item "<<item<<" of quantity "<<quantity<<" is added to the list."<<endl;
				cout<<"---"<<endl;
			}
		}
		
	}
}

void emptyList() // if the list is empty, printing the appropriate message to the screen
{
	cout<<"The shopping list is empty."<<endl;
	cout<<"---"<<endl;
}
void checkList(node * & head) // checking the list whether it is empty or not
{
	if(head == NULL)
	{
		emptyList();
	}
}

void printList(node * head,int & count)  // printing the each item of the shopping list and their features such as their quantity and price
{
	if(count == 1 && head == NULL)
	{
		emptyList();
	}
	else
	{
		node * temporary = head;
		do
		{
			cout<<"Item: "<<temporary->name<<endl;
			cout<<"Quantity: "<<temporary->quantity<<endl;
			cout<<"Est. Price: "<<temporary->item_price<<endl;
			cout<<"---"<<endl;
			temporary = temporary->next;
		}
		while(temporary != head);
	}
}	
		
void printReverse(node * & head,int & count) // printing the items of the list with the reverse order
{
	if(count == 1 && head == NULL)
	{
		emptyList();
	}
	else
	{
		node * reverse = head->previous;
		do
		{
			cout<<"Item: "<<reverse->name<<endl;
			cout<<"Quantity: "<<reverse->quantity<<endl;
			cout<<"Est. Price: "<<reverse->item_price<<endl;
			cout<<"---"<<endl;
			reverse = reverse->previous;
		}
		while(reverse != head->previous);
	}
}
void printCost(node *head,int & count) // priting the total cost of the shopping list by iterating over the items
{
	if(count == 1 && head == NULL)
	{
		cout<<"The total estimated price of 0 kinds of items is: 0"<<endl;
		cout<<"---"<<endl;
	}
	else
	{
		int sum = 0,count = 0;
		if(head == NULL)
		{
			cout<<"The total estimated price of 0 kinds of items is: 0"<<endl;
			cout<<"---"<<endl;
		}
		else
		{
			node * ptr = head;
			do
			{
				sum+= ptr->total_price;
				ptr = ptr->next;
				count++;
			}
			while(ptr != head);
			cout<<"The total estimated price of "<<count<<" kinds of items is: "<<sum<<endl;
			cout<<"---"<<endl;
		}
	}
}

void printMenu(string & choice) // printing the menu to the screen
{
	string solve = "";
	cout<<"MENU\n""1. Add item to the shopping list\n""2. Delete item from the shopping list\n""3. Print the shopping list\n""4. Print the shopping list in reverse order\n"
		"5. Show total estimated cost of the list\n""6. Exit\n""---\n";
	cout<<"Enter your choice: ";
	cin>>choice; 
	getline(cin,solve);
	cout<<"---"<<endl;
}
int main()
{
	int count = 0;
	cout<<"This program helps Alan with gathering the shopping list for his psychedelic breakfast.\n""---\n";
	node * head = NULL,*tail = NULL;
	string choice = ""; // týrnak eklendi
	printMenu(choice);
	while(choice != "6") // program will be executed until the user press 6
	{
		if(choice == "1") // if the given input is '1' then the function which adds the item to the list would be called
		{
			addItem(head,count);
		}
		else if(choice == "2") // if the given input is '2' then the function which deleted the item from the list would be called
		{
			deleteItem(head,count);
		}
		else if(choice == "3")  // if the given input is '3' then the function which prints all the elements would be called
		{
			checkList(head);
			if(head != NULL)
				printList(head,count);
		}
		else if(choice == "4") // if the given input is '4' then the function which prints all the elements from the reverse order would be called
		{
			checkList(head);
			if(head != NULL)
				printReverse(head,count);
		}
		else if(choice == "5") // if the given input is '5' then the function prints the total cost of the list would be called
		{
			printCost(head,count); 
		}
		else
		{
			if(choice != "6") 
			{
				cout<<"This is not a valid option!\n""---"<<endl; // if the input is not valid then printing the appropriate message to the screen
			}
		}
		printMenu(choice);
	}
	cout<<"Clearing the shopping list...\n""Exiting the program..."<<endl; // if the given input is'6', then the loop will be terminated and the function which clears the element of the list would be called
	clearList(head,count);

	return 0;
}