#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <mutex>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include "Queue.h"
#include "randgen.h"
#include "DynamicStringStack.h"
// Efe Sencan 25083
// This program decryptes the data of the file given by the user
using namespace std;

int final_count = 0;
bool isTrue = true;
bool isCheck = true;
int isLeft = 0;
DynamicStringStack fileStack;
Queue fileQueue;
mutex dataMutex,printMutex,myMutex;

void decryptFile(int number,ifstream & inputFile,string & pop_element) // decrypting the encrpted version of the file if the file could appropriately be opened  
{
	string element = pop_element.substr(0,pop_element.length()-4); // finding the string before '.'
	string decrypt = element + "_decrypted.txt";
	ofstream outputFile;
	outputFile.open(decrypt.c_str()); // opening the output file
	inputFile.clear();
	inputFile.seekg(0);
	vector<int>countLetters(26,0); // forming a vector with size 26 and both of the elements value is initally 0(since there are 26 character in English letters)
	char ch;
	while(inputFile.get(ch))
	{
		if(int(ch)>= 97 && int(ch)<= 122) // we are increasing the index of the corresponding letter when see it in the file
		{
			countLetters[ch-'a']+=1;
		}
	}
	int max = 0;
	for(int i = 0;i<countLetters.size();i++) // finding the index which has the highest occurency level
	{
		if(countLetters[i] > countLetters[max])
		{
			max = i;
		}
	}
	int shift = max - 4; // we substract 4 (which corresponds to 'e') from that index to find the shift amount 
	inputFile.clear(); // we clean the file in order to read again
	inputFile.seekg(0);
	while(inputFile.get(ch))
	{
		if(ch>= 'a' && ch<='z')
		{
			ch = ch - shift; // applying the shift amount
			if(ch > 'z')
			{
				ch = ch - 26;
			}
			else if(ch < 'a')
			{
				ch = ch + 26;
			}
			outputFile << char(ch);
		}
		else
			outputFile << ch;
	}
	outputFile.close(); // closing the output file
	printMutex.lock();
	cout<<"Consumer Thread-"<<number<<" is done handling \""<<pop_element<<"\" with a shift of "<<shift<<" and written the result to "<<decrypt<<endl;
	printMutex.unlock();
}

void producer(int number) // producer function pops the file name from the stack and enques it to the queue(there are 2 producer threads)
{
	while(isTrue) // the loop continues until there is no element in the stack
	{
		RandGen random; // generating a random sleep in order producer threads to sleep before their operations
		int sleep_time = random.RandInt(1000,4000);
		printMutex.lock();
		cout<<"Producer Thread-"<<number<<" starts sleeping for "<<sleep_time<<" milliseconds"<<endl;
		printMutex.unlock();
		this_thread::sleep_for(chrono::milliseconds(sleep_time));
		string element = "";
		dataMutex.lock();
		if(!fileStack.isEmpty())
		{
			fileStack.pop(element);
			fileQueue.enqueue(element);
			final_count++;
			dataMutex.unlock();
			printMutex.lock();
			cout<<"Producer Thread-"<<number<<" is now enqueuing \""<<element<<"\""<<endl;
			printMutex.unlock();
		}
		else
		{
			dataMutex.unlock();
			printMutex.lock();
			cout<<"Producer Thread-"<<number<<" cannot find any file to enqueue, joining..."<<endl;
			printMutex.unlock();
			isTrue = false;
		}
	}
}

void consumer(int number) // custumer function dequeues the file from the queue and decrpyt it the file could be opened
{
	ifstream inputFile;
	ofstream outputFile;
	int count = 0;
	string pop_element = "";
	while(isCheck)
	{
		RandGen random;
		int sleep_time = random.RandInt(2500,3500); // generating the sleep time
		printMutex.lock();
		cout<<"Consumer Thread-"<<number<<" starts sleeping for "<<sleep_time<<" milliseconds"<<endl;
		printMutex.unlock();
		this_thread::sleep_for(chrono::milliseconds(sleep_time)); // sleeping the consumer threads before thier operations
		myMutex.lock();
		if(!fileQueue.isEmpty())
		{
			fileQueue.dequeue(pop_element);
			myMutex.unlock();
			printMutex.lock();
			cout<<"Consumer Thread-"<<number<<" is now handling \""<<pop_element<<"\""<<endl;
			printMutex.unlock();
			inputFile.clear();
			inputFile.seekg(0);
			inputFile.open(pop_element.c_str());
			if(inputFile.fail())
			{
				printMutex.lock();
				cout<<"Consumer Thread-"<<number<<"  cannot process \""<<pop_element<<"\", there is no such file!"<<endl;
				printMutex.unlock();
			}
			else if(!inputFile.fail())
			{
				decryptFile(number,inputFile,pop_element);
				inputFile.close();
			}
			else
			{
				printMutex.lock();
				cout<<"Consumer Thread-"<<number<<" cannot find any file to dequeue for now."<<endl;
				printMutex.unlock();
				count = 0;
			}
		}
		else
		{
			if(fileQueue.isEmpty() && fileStack.isEmpty())
			{
				isCheck = false;
				myMutex.unlock();
			}
			else if(fileQueue.isEmpty() && !fileStack.isEmpty())
			{
				myMutex.unlock();
				printMutex.lock();
				cout<<"Consumer Thread-"<<number<<" cannot find any file to dequeue for now."<<endl;
				printMutex.unlock();
				count = 0;
			}
			else
				myMutex.unlock();
		}
	}
	if(fileQueue.isEmpty() && fileStack.isEmpty()) // when there is no file to process consumers will join to the main function
	{
		printMutex.lock();
		cout<<"Consumer Thread-"<<number<<" cannot find any file to dequeue, joining..."<<endl;
		printMutex.unlock();
		isCheck = false;
	}

}

int main()
{
	string control = "";
	while(control != "-") // getting the file names from the user and push it to the stack
	{
		cout<<"Please enter a file name: ";
		cin>>control;
		if(control != "-")
		{
			fileStack.push(control);
			isLeft++;
		}

	}
	// we have 5 threads
	thread first_producer(&producer,1);
	thread second_producer(&producer,2);
	thread first_consumer(&consumer,1);
	thread second_consumer(&consumer,2);
	thread third_consumer(&consumer,3);


	first_producer.join(); // after sending them to corresponding functions we join them
	second_producer.join();
	first_consumer.join();
	second_consumer.join();
	third_consumer.join();
	
	cout<<"All threads have joined with main, exiting..."<<endl;

	return 0;
}