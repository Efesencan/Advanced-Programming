#include "DynIntStack.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
// Efe Sencan
// This program finds the common words of two files by using only stacks
void readFile(DynIntStack & first_stack,DynIntStack & second_stack,ifstream & first_input,ifstream & second_input,int & number,DynIntStack & first_temp_stack) // reading the file given by the user
{
	string line,second_line,word,word_added = "";
	while(getline(first_input,line)) // reading the first file line by line
	{
		if(line != "")
		{
			istringstream sentence(line);
			while(sentence>>word)
			{
				word_added += word + " ";
				word_added = word_added.substr(0,word_added.length()-1);
				if(word_added != "")
				{
					first_stack.addItem(word_added,first_temp_stack); // this function checks if the word is already in the stack or not
					first_stack.push(word_added,number);// if the word is already in the stack, then we will only incrament the number of the word by one(we won't add the word to the stack)
				}
			}
			word_added = "";
		}
	}
	word_added = "";
	while(getline(second_input,second_line))
	{
		if(second_line != "")
		{
			istringstream second_sentence(second_line);
			while(second_sentence>>word)
			{
				word_added += word + " ";
				word_added = word_added.substr(0,word_added.length()-1);
				if(word_added != "")
				{
					second_stack.addItem(word_added,first_temp_stack); // the same two functions will also be applied for the second file
					second_stack.push(word_added,number);
				}
			}
			word_added = word_added.substr(0,word_added.length()-1);
			word_added = "";
		}
	}
}

void compare(DynIntStack & temp_stack,DynIntStack & first_stack,DynIntStack & second_stack,string & choice) // printing the common words of the file by using stack
{
	DynIntStack add_first_temp;
	DynIntStack add_second_temp;
	first_stack.isFalse();
	second_stack.isFalse();
	string item = "";
	int count = 0,number = 1,second_number = 0;
	string name = "",first_name = "";
	bool check;
	if(choice == "1") // if the choice is 1, then we will print the common words by the order the words in the first file taken from the user
	{
		while(!first_stack.isEmpty()) // obtaining the exact order of the first file by ading the elements of the first stack to the temp stack
		{
			first_stack.pop(item,count);
			temp_stack.push(item,count);
		}
		while(!temp_stack.isEmpty())
		{
			check = true;
			temp_stack.pop(name,number); // taking the each element of the temp stack
			while(!second_stack.isEmpty() && check) // comparing the element with the each element of the second stack untill finding the element in the second stack
			{
				second_stack.pop(first_name,second_number);
				add_first_temp.push(first_name,second_number);
				if(name == first_name)
				{
					check = false;
					if(number > second_number)
					{
						cout<<"The word "<<"\""<<name<<"\""<<" occurred at least "<<second_number<<" time(s) in both files."<<endl;
					}
					else
					{
						cout<<"The word "<<"\""<<name<<"\""<<" occurred at least "<<number<<" time(s) in both files."<<endl;
					}
				}
			}
			while(!add_first_temp.isEmpty())
			{
				add_first_temp.pop(first_name,second_number);
				second_stack.push(first_name,second_number);
			}
		}
	}
	else if(choice == "2")  // if the choice is 2, then we will print the common words by the order the words in the second file taken from the user
	{
		while(!second_stack.isEmpty()) // obtaining the exact order of the second file by ading the elements of the first stack to the temp stack
		{
			second_stack.pop(item,count);
			temp_stack.push(item,count);
		}
		while(!temp_stack.isEmpty())
		{
			check = true;
			temp_stack.pop(name,number); // taking the each element of the temp stack
			while(!first_stack.isEmpty() && check) // comparing the element with the each element of the second stack untill finding the element in the first stack
			{
				first_stack.pop(first_name,second_number);
				add_second_temp.push(first_name,second_number);
				if(name == first_name)
				{
					check = false;
					if(number > second_number)
					{
						cout<<"The word "<<"\""<<name<<"\""<<" occurred at least "<<second_number<<" time(s) in both files."<<endl;
					}
					else
					{
						cout<<"The word "<<"\""<<name<<"\""<<" occurred at least "<<number<<" time(s) in both files."<<endl;
					}
				}
			}
			while(!add_second_temp.isEmpty())
			{
				add_second_temp.pop(first_name,second_number);
				first_stack.push(first_name,second_number);
			}
		}
	}			
}

void fileNumber(string & choice) // checking the validity of the input given by the user for choosing the file to be printed the words(by the order of that file).
{
	cout<<"Choose with respect to which file the result will be sorted to (1: first file, 2: second file): ";
	cin>>choice;
	while(choice!= "1" && choice != "2")
	{
		cout<<"---"<<endl;
		cout<<"Invalid choice"<<endl;
		cout<<"---"<<endl;
		cout<<"Choose with respect to which file the result will be sorted to (1: first file, 2: second file): ";
		cin>>choice;
	}
}

void fileCheck(ifstream & first_input,ifstream & second_input,string & first_fileName,string & second_fileName,string & choice) // cehcking if the files are valid
{
	cout<<"---"<<endl;
	cout<<"Enter the first file name: ";
	cin>>first_fileName;
	first_input.open(first_fileName.c_str());
	while(first_input.fail())  // getting the first file name from the user until it is valid
	{
		cout<<"Enter the first file name: ";
		cin>>first_fileName;
		first_input.open(first_fileName.c_str());
	}
	cout<<"Enter the second file name: ";
	cin>>second_fileName;
	second_input.open(second_fileName.c_str());
	while(second_input.fail())  // getting the second file name from the user until it is valid
	{
		cout<<"Enter the second file name: ";
		cin>>second_fileName;
		second_input.open(second_fileName.c_str());
	}
	fileNumber(choice);
	cout<<"---"<<endl;
}

int main()
{
	int number = 1;
	ifstream first_input,second_input;
	string first_fileName,second_fileName,choice;
	DynIntStack first_stack,second_stack,temp_stack,first_temp_stack;
	cout<<"This program finds the common words of two files using stacks."<<endl;
	fileCheck(first_input,second_input,first_fileName,second_fileName,choice); // first we check the file
	readFile(first_stack,second_stack,first_input,second_input,number,first_temp_stack); // then reading it and storing them in a stack.
	compare(temp_stack,first_stack,second_stack,choice); // lastly we compare the common words and printing them to the console

	return 0;
}