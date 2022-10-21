#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

#define MAX_SIZE  300

int znaki[MAX_SIZE];

int index = 0;

map<char, string> codes;


//czy tablica zawiera dany znak
bool isContains(char* tab, int n, char value)
{
	for (int i = 0; i < n; i++)
	{
		if (tab[i] == value) return true;
	}
	return false;
}

//zamiana elementów w tablicy float
void Swap(float* tab, int first)
{
	float tmp = tab[first];
	tab[first] = tab[first + 1];
	tab[first + 1] = tmp;
}

//zamiana elementów w tablicy char
void Swap(char* tab, int first)
{
	char tmp = tab[first];
	tab[first] = tab[first + 1];
	tab[first + 1] = tmp;
}

//sortowanie elementów
void Sort(float* value, char* character, int size)
{
	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			if (value[j] > value[j + 1])
			{
				Swap(value, j);
				Swap(character, j);
			}
			if (value[j] == value[j + 1])
			{
				if (character[j] > character[j + 1])
				{
					Swap(value, j);
					Swap(character, j);
				}
			}
		}
	}
}

class Node
{
public:
	float value;
	char character;
	int index;

	Node* left = NULL;
	Node* right = NULL;

	Node(char character, float value, int index)
	{	
		this->character = character;
		this->value = value;
		this->index = index;
	}
};

struct compare {

	bool operator()(Node* a, Node* b)
	{
		if (a->value != b->value) 
			return (a->value > b->value);
		
		if (a->value == b->value)
		{
			if (a->character == '$' && b->character != '$')
				return true;
			if (a->character != '$' && b->character == '$')
				return false;
			if (a->character == '$' && b->character == '$')
			{
				return(a->index > b->index);
			}
				

			return (a->character > b->character);
		}									
	}
};

int binToDec(string s)
{

	int i, N, decimal_number = 0, p = 0;
	N = s.size(); 
	for (i = N - 1; i >= 0; i--) 
	{
		if (s[i] == '1') 
		{
			decimal_number += pow(2, p); 
		}
		p++; 
	}
	return decimal_number;
}

void printCodes(Node* root, string str,int size)
{

	ofstream dane("Huff.txt", ios::app);	
	
	if (!root)
	{		
		return;
	}
		

	if (root->character != '$')
	{		
		codes[root->character] = str;
		cout << root->character << ": " << str << " ";
		dane << root->character << ": " << str << " ";
	}
		

	printCodes(root->left, str + "0",size);
	printCodes(root->right, str + "1",size);

}

void printqueue(priority_queue<Node*, vector<Node*>, compare> tree)
{
	while (!tree.empty())
	{
		if (tree.top()->character == '$')
			cout << tree.top()->character << "{ " << tree.top()->left->character << ", " << tree.top()->right->character << " }"<<" ";
		else
			cout << tree.top()->character<< " ";
		tree.pop();
	}
	cout << endl;
}


Node* HuffmanCodes(char* character, float* value, int size)
{
	Node* left, * right, * top;
	priority_queue<Node*, vector<Node*>, compare> tree;
	priority_queue<Node*, vector<Node*>, compare> tree2;

	for (int i = 0; i < size; i++)
		tree.push(new Node(character[i], value[i],0));

	tree2 = tree;
	printqueue(tree2);

	int in = 1;
	while (tree.size() != 1) 
	{					
			left = tree.top();
			tree.pop();


			right = tree.top();
			tree.pop();


			top = new Node('$', left->value + right->value,in);
			in++;

			top->left = left;
			top->right = right;

			tree.push(top);	
			tree2 = tree;
			printqueue(tree2);
	}
	cout << "///////////////////////////////////////"<<endl;
	printCodes(tree.top(), "",size);
	return tree.top();
}


string decode(Node* root, string encoded) 
{
	string decoded = ""; 
	Node* currentNode = root; 
	for (char c : encoded)
	{ 
		if (c == '0') 
		{ 
			if (currentNode->left->character != '$')
			{ 
				decoded += currentNode->left->character; 
				currentNode = root; 
			}
			else 
			{
				currentNode = currentNode->left; 
			}

		}
		else 
		{
			if (currentNode->right->character != '$')
			{ 
				decoded += currentNode->right->character;
				currentNode = root;
			}
			else 
			{
				currentNode = currentNode->right;
			}

		}

	}
	return decoded; 

}

template<typename K, typename V>
void print_map(map<K, V> const& m)
{
	for (auto const& pair : m) {
		std::cout << "{" << pair.first << ": " << pair.second << "}\n";
	}
}

int main()
{
	string T = "";

	ifstream wejscie("In0305.txt");
	getline(wejscie, T);

	int n = T.size();

	int* number = new int[n];
	char* character = new char[n];

	//indeks dla tablic
	int j = 0;

	//faktyczny rozmiar tablic
	int size = 0;

	//czyszczenie pliku
	ofstream plik("Huff.txt");
	plik.close();

	//zliczenie znaków
	for (int i = 0; i < n; i++)
	{
		if (!isContains(character, n, T[i]))
		{			
			character[j] = T[i];
			number[j] = 1;
			j++;
			size++;
		}
		else
		{
			for (int k = 0; k < n; k++)
			{
				if (character[k] == T[i])
				{					
					number[k]++;										
				}
			}
		}
		
	}

	
	//uzupełnienie spacji znakiem '_' 
	for (int i = 0; i < n; i++)
	{
		if (character[i] == ' ')
		{
			character[i] = '_';
			break;
		}
	}
		
	
	//wypisanie częstości
	cout << "Czestosc wystapien kazdego znaku w tekscie: " << endl;
	for (int i = 0; i < size; i++)
	{	
		cout << character[i] << " " << number[i] <<"\t";
	}

	float* value = new float[n];
	for (int i = 0; i < size; i++)
	{
		value[i] = (float)((float)number[i] / (float)n);
	}
			
	//sortowanie na podstawie wartości (value)		
	Sort(value, character, size);

	cout << endl << "Niemalejaca lista drzew jednowezlowych: " << endl;
	for (int i = 0; i < size; i++)
	{
		cout << character[i] << " " << value[i] << "\t";
	}

	cout<<endl << "///////////////////////////////////"<<endl;

	Node* root = HuffmanCodes(character, value, size);
	cout << endl;

	
	string code = "";

	
	//zamiana spacji
	for (int i = 0; i < T.size(); i++)
	{
		if (T[i] == ' ') T[i] = '_';
	}
	
	
	//uporządkowanie kodów
	for (int i=0;i<T.size();i++)
	{
		code += codes[T[i]];
	}	

	int ile_zer = 0;
	//dopisanie do ośmiu znaków
	if (code.size() % 8 != 0)
	{
		while (code.size() % 8 != 0)
		{
			code += '0';
			ile_zer++;
		}
	}

	//print_map(codes);
	
	ofstream dane("Huff.txt", ios::app);
	dane << endl;
	string tmp = "";
	for (int i = 1; i < code.size() + 1; i++)
	{
		tmp += code[i-1];
		if (i % 8 == 0)
		{
			cout << (char)binToDec(tmp);
			dane << (char)binToDec(tmp);
			tmp = "";
		}
	}
	cout << endl << "Odkodowany tekst: "<<endl;
	if (code.size() > 0) code.resize(code.size() - ile_zer);
	cout << decode(root, code);
	
}