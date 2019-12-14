#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
// Efe Sencan 25083
//This program searchs the word that the user inputs and checks if it exists in the puzzle which is also given from the user, and calculate the points according to the search results

using namespace std;

void fileCheck(string & fileName, ifstream & input) // checking if the file could be properly opened or not
{
	input.open(fileName.c_str());
	while(input.fail())
	{
		cout<<"Invalid file name!"<<endl;
		cout<<"---"<<endl;
		cout<<"Enter the matrix file name: ";
		cin>>fileName;
		input.open(fileName.c_str());
	}	
}

bool isMatrix(ifstream & input,vector<vector<string> > & matrix,int & line_number) /* checking if the given puzzle is matrix or not
																    and storing letters in the vector of a vector*/
{
	string line;
	getline(input,line);
	int len_line = line.length();
	input.clear();
	input.seekg(0);
	while(getline(input,line))
	{
		line_number++;
		vector <string> temp;
		if(line.length() != len_line)
		{
			return false;
		}
		for(int i = 0; i< line.length(); i++)
		{
			temp.push_back(line.substr(i,1));
		}
		matrix.push_back(temp);
	}
	return true;
}

int calculatePoints(string word,string type,bool isTrue) // calculating the points gained or lost from that word
{
	int value = 0;
	if(type == "row" || type == "column") //first looking for the seearching type that came as parameter and apply the calculating rules according to them
	{
		if(word.length() > 5)
		{
			value+= word.length() + 2;
		}
		else
		{
			value+= word.length();
		}
	}
	else if(type == "diagonal")
	{
		if(word.length() > 5)
		{
			value+= word.length()*2 + 2;
		}
		else
		{
			value+= word.length()*2;
		}
		if(isTrue == false)  //if joker is used than 2 points will be deducted
		{
			value-=2;
		}
	}
	return value;
}

bool checkRow(vector<vector<string> > & matrix,string word) // checking if the word is matched in the row of the puzzle
{
	string search = "";
	for(int i = 0; i < matrix.size(); i++)
	{
		for(int j = 0; j + word.length() <= matrix[i].size(); j++)
		{
			int temp = j;
			int count = 0;
			while(count < word.length())
			{
				search += matrix[i][temp];
				temp++;
				count++;
			}
			string reverse = "";
			for(int i = search.length()-1; i>= 0; i--)
			{
				reverse+= search.substr(i,1);
			}
			if(search == word || reverse == word)
			{
				return true;
			}
			search = "";
		}
	}
	return false;
}

bool checkColumn(vector<vector<string> > & matrix,string word,int & line_number) // checking if the word is matched in the row of the puzzle
{
	string search = "";
	for(int i = 0; i + word.length() <= line_number; i++)
	{
		for(int j = 0; j < matrix[0].size(); j++)
		{
			int temp = i;
			int count = 0;
			while(count < word.length())
			{
				search += matrix[temp][j];
				temp++;
				count++;
			}
			string reverse = "";
			for(int i = search.length()-1; i>= 0; i--)
			{
				reverse+= search.substr(i,1);
			}
			if(search == word || reverse == word)
			{
				return true;
			}
			search = "";
		}
	}
	return false;
}

bool checkDiagonal(vector<vector<string> > & matrix,string word,int & line_number,bool & isTrue) /* checking if the word is found diagonally
																								 additionally, user can use one joker to find the word*/
{
	string search = "";
	for(int i = 0; i + word.length() <= line_number; i++)
	{
		for(int j = 0; j + word.length() < matrix[0].size(); j++)
		{
			int temp_i = i;
			int temp_j = j;
			int count = 0;
			while(count < word.length())
			{
				search += matrix[temp_i][temp_j];
				temp_i++;
				temp_j++;
				count++;
			}
			string reverse = "";
			for(int i = search.length()-1; i>= 0; i--)
			{
				reverse+= search.substr(i,1);
			}
			if(search == word || reverse == word)
			{
				return true;
			}
			else
			{
				int counter = 0;
				int counter_reverse = 0;
				for(int i = 0; i<word.length(); i++)
				{
					if(word.substr(i,1) == search.substr(i,1))
					{
						counter++;
					}
				}
				for(int i = 0; i<word.length(); i++)
				{
					if(word.substr(i,1) == reverse.substr(i,1))
					{
						counter_reverse++;
					}
				}
				if(counter == word.length()-1 || counter_reverse == word.length()-1)
				{
					isTrue = false;
					return true;
				}
			}
			search = "";
		}
	}
	return false;
}
int main()

{
	vector<vector<string> > matrix; 
	string fileName;
	ifstream input;
	cout<<"This program allows you to play a Sunday morning puzzle on your computer."<<endl; // getting inputs from the user
	cout<<"---"<<endl;
	cout<<"Enter the matrix file name: ";
	cin>>fileName;
	fileCheck(fileName,input);
	int line_number = 0;
	if(isMatrix(input,matrix,line_number)) 
	{
		int number_add = 0;
		bool isTrue = true;
		int total_point = 0;
		string word;
		string word_row = "row", word_column = "column", word_diagonal = "diagonal";
		cout<<"---"<<endl;
		cout<<"Enter a word: ";
		cin>>word;
		while(word.length() >= 3) // program will be executed until the user enter word that is shorter than 3 characters
		{
			isTrue = true;
			number_add = 0;
			if(checkRow(matrix,word)) //executing the each searching functions and then calculating the points if the input is a matrix
			{
				number_add = calculatePoints(word,word_row,isTrue);
				cout<<word<<" is found in the row level. It contributes "<<number_add<<" points."<<endl;
				total_point += number_add;
			}
			else if(checkColumn(matrix,word,line_number))
			{
				number_add = calculatePoints(word,word_column,isTrue);
				cout<<word<<" is found in the column level. It contributes "<<number_add<<" points."<<endl;
				total_point += number_add;
			}
			else if(checkDiagonal(matrix,word,line_number,isTrue))
			{
				number_add = calculatePoints(word,word_diagonal,isTrue);
				if(isTrue) // checking if joker was used according to the correctness value of the isTrue variable
				{
					cout<<word<<" is found in the diagonal level. It contributes "<<number_add<<" points."<<endl;
				}
				else 
				{
					cout<<word<<" is found in the diagonal level. One letter was replaced by joker. It contributes "<<number_add<<" points."<<endl;
				}
				total_point += number_add;
			}
			else
			{
				cout<<word<<" is not found in the matrix. 5 points are deducted."<<endl;
				total_point-=5;
			}
			cout<<"---"<<endl;
			cout<<"Enter a word: ";
			cin>>word;
		}
		cout<<"---"<<endl;
		cout<<"Game has ended."<<endl;
		cout<<"Total score of the words: "<<total_point<<endl;
	}
	else
	{
		cout<<"The matrix does not have row equality, terminating..."<<endl; // terminating the program if the given input is not a matrix
	}
	return 0;
}