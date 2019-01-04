//Author:	Anthony Ho
//Date:		10/30/2018 Part 1 submit
//Date 2:   11/7/2018 Part 2 submit
//Project:	Slider Puzzle 8
//Class:	CSC 414, Intro to Ai
//Purpose:	Solve a 8 Tile slider puzzle using 4 types of searches.

//Logic:	Generate a Random state or Get user input to make a state using menu options. If not default state will be used from slides.
//			Start search based on if their option is chosen. Run the search and have it output results to screen.

#include<iostream>// A lot of different methods were tried as to why there are many include statements.
#include<stdio.h> // I decided just to keep them.
#include<iomanip>
#include<string>
#include<cstring>
#include<map>
#include<queue>
#include<time.h>
#include<fstream>
#include<cstdlib>
#include<vector>
#include <list>
#include<stdlib.h>
#include <algorithm>
#include <stack>
#include<ctime>
#include<sstream>

using namespace std;

#define Row 3// row and col used to define the bounds of array/puzzle
#define Col 3
#define Size 9

struct node // agent node creator
{
	void clear()// deallocate 
	{

		stateString.clear();
		paths.clear();
	}
	string stateString; // Stores my state as a string
	int stateArray[Size]; // Stores int array
	int Depth; // Stores depth 
	int Heuristic; // Used for A* search
	int moveCount = 0;// Used to count how many swaps are made
	int empty; // location of zero
	list<string>paths; // tracking moves


	
	bool operator < (const node& other) const// used to prioritize nodes for A* search
	{
		return Heuristic > other.Heuristic;
	}
}nexts;


const string goalState = { "123456780" }; // goal state of type string as on slide

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This section with the functions getInvCount and isSolvable is from a website
// https://www.geeksforgeeks.org/check-instance-8-puzzle-solvable/
// Article is contributed by Ishan
// Improved By : jit_t

// It was first on google search. I did not copy the main code they gave.
int getInvCount(int arr[]);

// This function returns true if given 8 puzzle is solvable. 
bool isSolvable(int puzzle[3][3]);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const int t1 = 0; // t stands for tile, number beside it stands for tile position, equals array position
const int t2 = 1;
const int t3 = 2;
const int t4 = 3;
const int t5 = 4;
const int t6 = 5;
const int t7 = 6;
const int t8 = 7;
const int t9 = 8;
int initialState[Size]; // adds random gen/ user gen state into initial state
int workingState[Size]; // will input initial state into working state to work with
int tempState[Size]; // temp  state
int myRand[Size] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };// used to randomize
map<string, int> visited; // store visisted states
queue <node> q; // used for BFS
stack <node> s; // used for DFS

priority_queue<node> pq; // used for A* missing tiles function , data structure
priority_queue<node> pq2;// used for A* manhattan distance, data structure

int counter = 0;// count variable
string temp; // store string

string findMoves(int zero, int working[Size], int &myMoves, int &myCount, string &path);// find moves
void showInitial(); // show initial state
void copy(); // temp copies working state
void showWorking();// show working state
void randState(); // rand gen state
void clear(); // clear data
int findWrong(int array[Size]);// find # wrong tiles, return int  value
int findMan(int array[Size]);// find manhattan distance, returns int value


void swap12(int puzzle[Size]); //1 moves right
void swap14(int puzzle[Size]); //2 moves down
void swap23(int puzzle[Size]); //3 move right
void swap25(int puzzle[Size]); //4 move down
void swap36(int puzzle[Size]); //5 move down
void swap45(int puzzle[Size]); //6 move right
void swap47(int puzzle[Size]); //7 move down
void swap56(int puzzle[Size]); //8 move right
void swap58(int puzzle[Size]); //9 move down
void swap69(int puzzle[Size]); //10 move down
void swap78(int puzzle[Size]); //11 move right
void swap89(int puzzle[Size]); //12 move right


int findZero(int puzzle[Size]); // find zero andd returns location
bool history(string state); // check if state is in map
void BFS(int puzzle[Size]); // BFS function
void DFS(int puzzle[Size]); // DFS function
void aTile(int puzzle[Size]);// aTile function
void aMan(int puzzle[Size]);

int main()
{
	int ans = 0;// stores menu option choice
	int checkstate = 0; //used to decide if default state needs to be used
	int check = 0; // checks input
	int counter = 0; // used to increment to take valid amount of input from user
	int userInput = 0;//used to take numbers to make user generated state
	int array[Size]; // int array to store values and used as parameter
	int slidestartState[Size] = { 7, 8, 3, 4, 1, 5, 6, 0, 2 }; // from slides
	int twoD[Row][Col]; // used to test if puzzle is solvable
	int c = 0;// used in functions used from website to check if state is solvable
	double timerStart = 0;// timing variables
	double timerEnd = 0;// timing variable

	while (ans != 99)
	{
		counter = 0; // set variables to 0 
		userInput = 0;
		c = 0;
		check = 0;
		timerEnd = 0;
		timerStart = 0;

		// My menu
		cout << "Type a number between 1-7, Based on the option number you want to use" << endl;
		cout << "If you decide to run searches before a state is chosen, a default state will be used. " << endl;
		cout << "Option 1: Generate a random starting state. " << endl;
		cout << "Option 2: Create your starting state. " << endl;
		cout << "Option 3: Run BFS after starting state has been chosen. " << endl;
		cout << "Option 4: Run DFS after starting state has been chosen. " << endl;
		cout << "Option 5: Run A*(Number of out of place tiles) after start state is chosen." << endl;
		cout << "Option 6: Run A*(Manhattan Distance) after start state is chosen. " << endl;
		cout << "Option 7: Exit the program. " << endl << endl;
		cout << "Which option would you like to choose? :";
		cin >> ans;
		cout << endl;

		if (cin.good()) // if input is good, making sure no letters
		{
			//~~~~~~~~~~~~~~~~~~~
			if (ans == 1) // Random generate a state.
			{
				checkstate = 1;
				randState();
				cout << endl;

				for (int i = 0; i < Row; i++)
				{
					for (int j = 0; j < Col; j++)// store initial state into a 2d array to check
					{
						twoD[i][j] = initialState[c];
						c++;
					}
				}

				isSolvable(twoD) ? cout << "Solveable" :// check if solvable
					cout << "Not solveable";
				cout << endl << endl;;
				
			}
			//~~~~~~~~~~~~~~~~~~~~
			if (ans == 2) // user generated state option
			{
				checkstate = 1;// check to see if default state is needed

				while (counter < Size)// loop to get input from user
				{
					cout << "Enter values between 0-8 once. ";
					cin >> userInput;
					if (cin.good())// check for letters
					{
						if (userInput < 9) // make sure input is less than 9
						{
							for (int i = 0; i < Size; i++)
							{
								check = 0;
								if (userInput == array[i])// making sure not duplicate value, only one of each number < 9
								{
									cout << "You entered a duplicate." << endl;
									check = 1;
									break;
								}
							}

							if (check == 0)// if not dupe, add to array
							{
								array[counter] = userInput;
								counter++;
							}
						}
						else
							cout << "Invalid input. " << endl;

					}
					else
					{
						cout << "Not an integer. " << endl;
						cin.clear();
						cin.ignore(INT_MAX, '\n');
					}


				}
				system("CLS");
				cout << "Your user starting State: ";
				for (int j = 0; j < Size; j++)
				{
					initialState[j] = array[j];// Initital state copies user generated state
				}

				for (int i = 0; i < Size; i++)
				{
					array[i] = 99;// prepping array for reuse
				}

				showInitial();// show intital state
				cout << endl;
				cout << "Zero Index Location: " << findZero(initialState) << endl; // show 0 index
				cout << endl;


				for (int i = 0; i < Row; i++)// check if solveable
				{
					for (int j = 0; j < Col; j++)
					{
						twoD[i][j] = initialState[c];
						c++;
					}
				}

				isSolvable(twoD) ? cout << "Solveable" :
					cout << "Not solveable";
				cout << endl << endl;

			}
			//~~~~~~~~~~~~~~~~~~~~~~
			// BFS OPTION

			if (ans == 3) // if user gens a random or own input, default state will not be used, default state is from slides
			{
				if (checkstate == 0)
				{
					system("CLS");
					cout << "BFS is using a default state since one was not initialized." << endl;
					cout << "BFS is running......." << endl;
					timerStart = clock();// used for timing
					BFS(slidestartState);// takes default parameter in bfs function
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC)); // used for timing
					cout << "Time ran in Seconds: " << timerEnd << endl << endl;// output time
					cout << endl;
				}
				if (checkstate == 1)
				{
					system("CLS");
					cout << "BFS is using a Random Generated or User Generated State." << endl;// lets user know what general state is used
					cout << "BFS is running......" << endl;
					timerStart = clock();// timing variable
					BFS(initialState); // initial state as parameter for BFS
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC)); // using for timing
					cout << "Time ran in Seconds: " << timerEnd << endl << endl; // output time
					cout << endl;
				
				}
				clear();// clear out the data for reuse

			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// DFS version of BFS, code is almost similar to BFS so No need to write in depth comments

			if (ans == 4)  // DFS Option
			{
				if (checkstate == 0) // check for default or not state
				{
					system("CLS");
					cout << "DFS is using a default state since one was not initialized." << endl;
					cout << "DFS is running......." << endl;
					timerStart = clock();// time 
					DFS(slidestartState);// DFS function
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC));// time
					cout << "Time ran in Seconds: " << timerEnd << endl<< endl;// output time
					cout << endl;
					
				}
				if (checkstate == 1)
				{
					system("CLS");
					cout << "DFS is using a Random Generated or User Generated State." << endl;
					cout << "DFS is running......" << endl;
					timerStart = clock();
					DFS(initialState);// initial state as parameter for DFS
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC));
					cout << "Time ran in Seconds: " << timerEnd << endl << endl;
					cout << endl;
					
				}
				clear();// clear data
			}

			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// A* find misplaced tiles option 
			if (ans == 5)  
			{
				if (checkstate == 0) // check for default or not state
				{
					system("CLS");
					cout << "A* missing tiles is using a default state since one was not initialized." << endl;
					cout << "A* missing tiles is running......." << endl;
					timerStart = clock();// time 
					aTile(slidestartState);// A* missing tiles function
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC));// time
					cout << "Time ran in Seconds: " << timerEnd << endl << endl;// output time
					cout << endl;
					
				}
				if (checkstate == 1)
				{
					system("CLS");
					cout << "A* missing tiles is using a Random Generated or User Generated State." << endl;
					cout << "A* missing tiles is running......" << endl;
					timerStart = clock();
					aTile(initialState);// initial state as parameter for A* missing tiles
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC));
					cout << "Time ran in Seconds: " << timerEnd << endl << endl;
					cout << endl;
					
				}
				clear();// clear data
			}


			if (ans == 6)
			{

				if (checkstate == 0) // check for default or not state
				{
					system("CLS");
					cout << "A* Manhattan Distance is using a default state since one was not initialized." << endl;
					cout << "A* Manhattan Distance tiles is running......." << endl;
					timerStart = clock();// time 
					aMan(slidestartState);// A* missing tiles function
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC));// time
					cout << "Time ran in Seconds: " << timerEnd << endl << endl;// output time
					cout << endl;
			
				}
				if (checkstate == 1)
				{
					system("CLS");
					cout << "A* Manhattan Distance is using a Random Generated or User Generated State." << endl;
					cout << "A* Manhattan Distance is running......" << endl;
					timerStart = clock();
					aMan(initialState);// initial state as parameter for A* missing tiles
					timerEnd = (clock() - timerStart) / (double(CLOCKS_PER_SEC));
					cout << "Time ran in Seconds: " << timerEnd << endl << endl;
					cout << endl;
				}
				clear();// clear data

			}

			if (ans == 7)// exit
			{
				ans = 99;
			}
		}
		else
		{
			cout << "Not an integer. " << endl;// if not a number
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}


	system("PAUSE");
	return 0;

}

void showInitial() // show initial state
{
	for (int i = 0; i < Size; i++)
	{
		cout << initialState[i] << " ";
	}
	cout << endl;
}

void copy() // copy states 
{
	for (int i = 0; i < Size; i++)
	{
		tempState[i] = workingState[i];
	}
}

void showWorking()// shows working state
{
	for (int i = 0; i < Size; i++)
	{
		cout << workingState[i] << " ";
	}
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This section with the functions getInvCount and isSolvable is from a website
// https://www.geeksforgeeks.org/check-instance-8-puzzle-solvable/
// Article is contributed by Ishan
// Improved By : jit_t

// It was first on google search. I did not copy the main code they gave.
//

int getInvCount(int arr[])
{
	int inv_count = 0;
	for (int i = 0; i < 9 - 1; i++)
		for (int j = i + 1; j < 9; j++)
			// Value 0 is used for empty space 
			if (arr[j] && arr[i] && arr[i] > arr[j])
				inv_count++;
	return inv_count;
}

// This function returns true if given 8 puzzle is solvable. 
bool isSolvable(int puzzle[3][3])
{
	// Count inversions in given 8 puzzle 
	int invCount = getInvCount((int *)puzzle);

	// return true if inversion count is even. 
	return (invCount % 2 == 0);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void randState()//option one, to generate random state
{
	random_shuffle(std::begin(myRand), std::end(myRand));

	for (int i = 0; i < Size; i++)// initital state stores random state data
	{
		initialState[i] = myRand[i];
	}
	system("CLS");
	cout << "Random Generated: ";
	showInitial();

}

int findZero(int puzzle[Size])// find my zero
// returns 0/empty location
{
	int ans = 0;

	for (int i = 0; i < Size; i++)
	{
		if (puzzle[i] == 0)
		{
			ans = i;
		}
	}

	return ans;

}

bool history(string state) // this function checks if state string is in map
{
		map <string, int> ::iterator it = visited.find(state);
		
		if (it == visited.end())
		{
			return false;
		}
		else
			return true;

}

void clear()// will use to clear data
{
	
	visited.clear();

	while (!q.empty())
	{
		q.pop();
	}

	while (!s.empty())
	{
		s.pop();
	}
}

int findWrong(int puzzle[Size])// find amount of misplaced tiles 
{
	int misTiles = 0; // misplacedTiles = 0
	int tempPuzzle[Size];// holds inputted array to be converted to string

	for (int i = 0; i < Size; i++)// storing data into temp array
	{
		tempPuzzle[i] = puzzle[i];
	}

	string str2;// convert puzzle to a string
	for (int i : tempPuzzle) 
	{
		str2 += std::to_string(i);
	}
	

	for (int i = 0; i < Size; i++)
	{
		if (str2[i] != goalState[i])// if current state tiles doesnt match up with goal state, increment misTiles
		{
			misTiles++;
		}
	}
	return misTiles;// return misTiles when done
}

int findMan(int array[Size])
{
	int numGoal[Size];
	string tempGoal[Size];
	int tempArr[Size];
	int manD = 0;
	int ValueY;
	int ValueX;

	for (int j = 0; j < Size; j++)// add const goalstate values into non const temp goal array
	{
		tempGoal[j] = goalState[j];
	}

	for (int i = 0; i < Size; i++)// convert temp goal array type string into int goal array 
	{
		numGoal[i] = stoi(tempGoal[i]);
	}

	for (int a = 0; a < Size; a++)// add array used as parameter to temp array 
	{
		tempArr[a] = array[a];
	}

	// calculate manhattan distance
	for (int i = 0; i < Size; i++)
	{
		if (tempArr[i] != 0 && tempArr[i] != numGoal[i])// does not equal 0 and not a tile in correct position
		{
			ValueY = (tempArr[i] ) / 3; // y pos 
			ValueX = (tempArr[i] ) % 3; // x pos

			// manhattan distance = sum/manD, but using a 1d array
			// (x1+ x2) - (y1 + y2)
			manD += (abs((i % 3) - (ValueX))) + abs((i / 3) - (ValueY));
		}
	}

	return manD;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 12 swap functions as slide stated, 
void swap12(int puzzle[Size]) // 1 
{
	int temp = puzzle[t1]; 
	puzzle[t1] = puzzle[t2];
	puzzle[t2] = temp;
}
void swap14(int puzzle[Size]) // 2
{
	int temp = puzzle[t1];
	puzzle[t1] = puzzle[t4];
	puzzle[t4] = temp;
}
void swap23(int puzzle[Size]) //3
{
	int temp = puzzle[t2];
	puzzle[t2] = puzzle[t3];
	puzzle[t3] = temp;
}
void swap25(int puzzle[Size]) //4
{
	int temp = puzzle[t2];
	puzzle[t2] = puzzle[t5];
	puzzle[t5] = temp;
}
void swap36(int puzzle[Size]) //5
{
	int temp = puzzle[t3];
	puzzle[t3] = puzzle[t6];
	puzzle[t6] = temp;
}
void swap45(int puzzle[Size]) //6
{
	int temp = puzzle[t4];
	puzzle[t4] = puzzle[t5];
	puzzle[t5] = temp;
}
void swap47(int puzzle[Size]) //7
{
	int temp = puzzle[t4];
	puzzle[t4] = puzzle[t7];
	puzzle[t7] = temp;
}
void swap56(int puzzle[Size]) //8
{	
	int temp = puzzle[t5];
	puzzle[t5] = puzzle[t6];
	puzzle[t6] = temp;
}
void swap58(int puzzle[Size]) //9
{
	int temp = puzzle[t5];
	puzzle[t5] = puzzle[t8];
	puzzle[t8] = temp;
}
void swap69(int puzzle[Size]) //10
{
	int temp = puzzle[t6];
	puzzle[t6] = puzzle[t9];
	puzzle[t9] = temp;
}

void swap78(int puzzle[Size]) //11
{
	int temp = puzzle[t7];
	puzzle[t7] = puzzle[t8];
	puzzle[t8] = temp;	
}
void swap89(int puzzle[Size]) //12
{
	int temp = puzzle[t8];
	puzzle[t8] = puzzle[t9];
	puzzle[t9] = temp;
}

// uses data from function, zero location, array, moves available for each index, count variable, and path
string findMoves(int zero, int working[Size], int &myMoves, int &myCount, string &path)// Used to find moves
{
	int newPuzzle[Size];// int puzzle

	for (int i = 0; i < Size; i++)// new puzzle copies working array
	{
		newPuzzle[i] = working[i]; 
		
	}
	//~~~~~~~~~~~~ Tile 1 , each index has its tile number, and number of moves each tile has. path equals swap used as a string. 
	if (zero == 0)
	{
		myMoves = 2;
		myCount++;
		switch(myCount)
		{
			case 1:
				swap12(newPuzzle);
				path = "1 -> 2, ";
				break;
			case 2:
				swap14(newPuzzle);
				path = "1 -> 4, ";
				break;
		}
	}
	//~~~~~~~~~ Tile 2

	if (zero == 1)
	{
		myMoves = 3;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap23(newPuzzle);
			path = "2 -> 3, ";
			break;
		case 2:
			swap12(newPuzzle);
			path = "2 -> 1, ";
			break;
		case 3:
			swap25(newPuzzle);
			path = "2 -> 5, ";
			break;
		}

	}
	//~~~~~~~~ Tile 3

	if (zero == 2)
	{
		myMoves = 2;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap23(newPuzzle);
			path = "3 -> 2, ";
			break;
		case 2:
			swap36(newPuzzle);
			path = "3 -> 6, ";
			break;
		}

	}
	//~~~~~~~~ Tile 4
	
	if (zero == 3)
	{
		myMoves = 3;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap45(newPuzzle);
			path = "4 -> 5, ";
			break;
		case 2:
			swap47(newPuzzle);
			path = "4 -> 7, ";
			break;
		case 3:
			swap14(newPuzzle);
			path = "4 -> 1, ";
			break;
		}

	}
	//~~~~~~~~~ Tile 5
	if (zero == 4)
	{
		myMoves = 4;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap56(newPuzzle);
			path = "5 -> 6, ";
			break;
		case 2:
			swap58(newPuzzle);
			path = "5 -> 8, ";
			break;
		case 3:
			swap45(newPuzzle);
			path = "5 -> 4, ";
			break;
		case 4:
			swap25(newPuzzle);
			path = "5 -> 2, ";
			break;
		}
	}
	//~~~~~~~~~ Tile 6
	if (zero == 5)
	{
		myMoves = 3;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap69(newPuzzle);
			path = "6 -> 9, ";
			break;
		case 2:
			swap56(newPuzzle);
			path = "6 -> 5, ";
			break;
		case 3:
			swap36(newPuzzle);
			path = "6 -> 3, ";
			break;
		//	}
		}
	}
	//~~~~~~~ Tile 7
	if (zero == 6)
	{
		myMoves = 2;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap78(newPuzzle);
			path = "7 -> 8, ";
			break;
		case 2:
			swap47(newPuzzle);
			path = "7 -> 4, ";
			break;
		}
	}
	//~~~~~~~~ Tile 8
	if (zero == 7)
	{
		myMoves = 3;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap89(newPuzzle);
			path = "8 -> 9, ";
			break;
		case 2:
			swap58(newPuzzle);
			path = "8 -> 5, ";
			break;
		case 3:
			swap78(newPuzzle);
			path = "8 -> 7, ";
			break;

		}
	}
	//~~~~~~~~~~ Tile 9
	if (zero == 8)
	{
		myMoves = 2;
		myCount++;
		switch (myCount)
		{
		case 1:
			swap69(newPuzzle);
			path = "9 -> 6, ";
			break;
		case 2:
			swap89(newPuzzle);
			path = "9 -> 8, ";
			break;
		}

	}

	for (int i = 0; i < Size; i++)
	{
		tempState[i] = newPuzzle[i];// temp state copies new puzzle after each swap
	}

	string str2;// convert new puzzle to a string
	for (int i : newPuzzle)
	{
		str2 += std::to_string(i);
	}
	
	return str2; // return string
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void BFS(int puzzle[Size]) // BFS function
{
	// variables used in BFS
	int myCount = 0; 
	int start;
	node iState; 
	int check = 0;
	string first = "Start, ";
	string workingString;
	int myMoves = 0;
	list<string>::iterator it;
	int depth;
	string path;
	int nodeCount = 1;

	// initial state take on value of puzzle to make sure its right
	for (int i = 0; i < Size; i++)
	{
		initialState[i] = puzzle[i];
	}
	

	// convert initital state to a string
	string str;
	for (int i : initialState)
	{
		str += std::to_string(i);
	}
	
 // add created string and count which is 0, to map
	visited.insert(std::pair<string, int>(str, myCount));

	start = findZero(puzzle);// find 0 location for initital starting puzzle

	// create starting node
	iState.moveCount = myCount;// add counter which is 0 at the moment
	iState.stateString = str; // add array as a string, used to compare goalState
	for (int i = 0; i < Size; i++)// input array into node
	{
		iState.stateArray[i] = puzzle[i];
	}
	iState.Depth = 0; // add depth = 0
	iState.Heuristic = 0; // = 0
	iState.paths.push_back(first);// add starting location string
	iState.empty = start;// add location of empty spot in initial state

	q.push(iState);// push onto data structure


	while (!q.empty()) // start loop
	{
		int myCount = 0;// count variable
		
		node current = q.front();// dequeue, current takes on data

		for (int i = 0; i < Size; i++)
		{
			workingState[i] = current.stateArray[i]; // working state takes on node's array value
		}
		workingString = current.stateString; // var to store node string


		if (workingString == goalState) // compare to see if goal state
		{  // output results if goal state
			system("CLS");
			check = 1;
			cout << "CONGRATS BFS COMPLETED SUCCESSFULLY...." << endl;
			cout << "Starting State: ";
			showInitial();
			cout << "Solution at Depth: " << current.Depth << endl;
			cout << "Path: " << endl;

			for (it = current.paths.begin(); it != current.paths.end(); it++)
			{
				cout << ' ' << *it << endl;
				
			}
			cout << "End." << endl << endl;
			cout << "Total Nodes: " << nodeCount << endl;
			cout << "Solution State: ";
			showWorking();
			cout << endl << endl;
			break;
		}


	
		q.pop();	// pop the node

		int zero = findZero(workingState);// find the 0 location to start swaps 

		do// do loop start
		{
			temp = findMoves(zero, workingState, myMoves, myCount, path); // temp = returned string value from find moves
			
			
			if(!history(temp))// check if temp is in map
			{
				// if not in map create next node
				nodeCount++;
				visited.insert(std::pair<string, int>(temp, myCount));
				nexts.Depth = current.Depth + 1;
				nexts.moveCount = myCount;
				nexts.stateString = temp;
				for (int i = 0; i < Size; i++)
				{
					nexts.stateArray[i] = tempState[i];
 				}
				nexts.paths = current.paths;
				nexts.paths.push_back(path);
				q.push(nexts); // push that new node into data structure
				nexts.clear(); // deallocate
			
			}

		} while (myCount != myMoves);//do swaps until count != moves available per index
	}



	if (check == 0)// if goal state not found, error or unsuccessful attempt
	{
		system("CLS");
		cout << "UNSUCCESSFUL ATTEMPT. " << endl << endl;
		cout << "Total nodes Explored: " << nodeCount << endl << endl;
		cout << "The unsolvable initial state is: ";
		showInitial();
		cout << endl;
	}
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Just about the same comments i used for BFS will apply for DFS. DFS will use .top instead of .front.
// bfs uses node q, dfs uses stack s.
// As for outputting results, path will not be included in DFS result output, as stated reasons from slide

void DFS(int puzzle[Size])// DFS function
{

	// DFS varialbles used
	int myCount = 0; 
	int start;
	node iState; 
	int check = 0;
	string first = "Start, ";
	string workingString;
	int myMoves = 0;
	string path;
	int nodeCount = 1;

	for (int i = 0; i < Size; i++)
	{
		initialState[i] = puzzle[i];
	}

	
	string str;
	for (int i : initialState)
	{
		str += std::to_string(i);
	}


	
	visited.insert(std::pair<string, int>(str, myCount));// 3. Put the state and counter in the Map

	start = findZero(puzzle);
	iState.moveCount = myCount;// add counter which is 0 at the moment
	iState.stateString = str; // add array as a string, used to compare goalState
	for (int i = 0; i < Size; i++)// input array into node
	{
		iState.stateArray[i] = puzzle[i];
	}
	iState.Depth = 0; // add depth = 0


	s.push(iState);


	while (!s.empty())
	{
		int myCount = 0;

		node dcurrent = s.top();// instead of q.front();

		for (int i = 0; i < Size; i++)
		{
			workingState[i] = dcurrent.stateArray[i];
		}
		workingString = dcurrent.stateString;

		if (workingString == goalState)
		{
			system("CLS");
			check = 1;
			cout << "CONGRATS DFS COMPLETED SUCCESSFULLY...." << endl;	
			cout << "Starting State: ";
			showInitial();
			cout << "Solution at Depth: " << dcurrent.Depth << endl;
			cout << "Total Nodes: " << nodeCount << endl;
			cout << "Solution State: ";
			showWorking();
			cout << endl << endl;
			break;
		}
		s.pop();

		int zero = findZero(workingState);
		do
		{
			temp = findMoves(zero, workingState, myMoves, myCount, path);
			if (!history(temp))
			{
				nodeCount++;
				visited.insert(std::pair<string, int>(temp, myCount));

				nexts.Depth = dcurrent.Depth + 1;
				nexts.moveCount = myCount;
				nexts.stateString = temp;
				for (int i = 0; i < Size; i++)
				{
					nexts.stateArray[i] = tempState[i];
				}
				s.push(nexts);
				nexts.clear();
				
			}

		} while (myCount != myMoves);
	}


	if (check == 0)
	{
		system("CLS");
		cout << "UNSUCCESSFUL ATTEMPT. " << endl << endl;
		cout << "Total Nodes Explored: " << nodeCount << endl << endl;
		cout << "The unsolvable initial state is: ";
		showInitial();
		cout << endl;
	}

}

//~~~~~~~~~~~~~~~
// aTile = A* missing tiles , same comments basically used for DFS AND BFS, priority queue is used instead of a stack or regular queue
// added path use and output like bfs function
// amcurrent = a* missing tiles current node

void aTile(int puzzle[Size])// A* missing tiles function
{

	// aTile varialbles used
	int myCount = 0;
	int start;
	node iState;
	int check = 0;
	string first = "Start, ";
	string workingString;
	int myMoves = 0;
	string path;
	int nodeCount = 1;
	int depth = 0;
	list<string>::iterator it;

	for (int i = 0; i < Size; i++)
	{
		initialState[i] = puzzle[i];
	}


	string str;
	for (int i : initialState)
	{
		str += std::to_string(i);
	}



	visited.insert(std::pair<string, int>(str, myCount));// 3. Put the state and counter in the Map

	start = findZero(puzzle);
	iState.moveCount = myCount;// add counter which is 0 at the moment
	iState.stateString = str; // add array as a string, used to compare goalState
	for (int i = 0; i < Size; i++)// input array into node
	{
		iState.stateArray[i] = puzzle[i];
	}
	iState.Depth = depth; // add depth = 0

	iState.Heuristic = (depth + (findWrong(puzzle)));// starting heuristic

	iState.paths.push_back(first);// add starting location string
	iState.empty = start;// add location of empty spot in initial state


	pq.push(iState);


	while (!pq.empty())
	{
		int myCount = 0;

		node amcurrent = pq.top();// am = A* missing tile, current node

		for (int i = 0; i < Size; i++)
		{
			workingState[i] = amcurrent.stateArray[i];
		}
		workingString = amcurrent.stateString;

		if (workingString == goalState)// output goal if goalstate
		{
			system("CLS");
			check = 1;
			cout << "CONGRATS A* missing tile search COMPLETED SUCCESSFULLY...." << endl;
			cout << "Starting State: ";
			showInitial();
			cout << "Solution at Depth: " << amcurrent.Depth << endl;
			cout << "Path: " << endl;
			for (it = amcurrent.paths.begin(); it != amcurrent.paths.end(); it++)
			{
				cout << ' ' << *it << endl;
			}
			cout << "End." << endl << endl;
			cout << "Total Nodes: " << nodeCount << endl;
			cout << "Solution State: ";
			showWorking();
			cout << endl << endl;
			break;
		}
		pq.pop();

		int zero = findZero(workingState);
		do
		{
			temp = findMoves(zero, workingState, myMoves, myCount, path);
			if (!history(temp))
			{

				nodeCount++;
				visited.insert(std::pair<string, int>(temp, myCount));
				
				nexts.Depth = amcurrent.Depth + 1;

				nexts.Heuristic = ((amcurrent.Depth + 1) + findWrong(tempState));// calculates heuristic needed for missing tiles
				// make heuristic = depth + missing tiles

				nexts.moveCount = myCount;
				nexts.stateString = temp;
				for (int i = 0; i < Size; i++)
				{
					nexts.stateArray[i] = tempState[i];
				}
				
				nexts.paths = amcurrent.paths;
				nexts.paths.push_back(path);
				pq.push(nexts);
				nexts.clear();

			}

		} while (myCount != myMoves);
	}


	if (check == 0)
	{
		system("CLS");
		cout << "UNSUCCESSFUL ATTEMPT. " << endl << endl;
		cout << "Total Nodes Explored: " << nodeCount << endl << endl;
		cout << "The unsolvable initial state is: ";
		showInitial();
		cout << endl;
	}

}


// This should have same comments i used for A* except the formula used to calculate heuristic is different
void aMan(int puzzle[Size])
{

	// a manhattan varialbles used
	int myCount = 0;
	int start;
	node iState;
	int check = 0;
	string first = "Start, ";
	string workingString;
	int myMoves = 0;
	string path;
	int nodeCount = 1;
	int depth = 0;
	list<string>::iterator it;

	for (int i = 0; i < Size; i++)
	{
		initialState[i] = puzzle[i];
	}


	string str;
	for (int i : initialState)
	{
		str += std::to_string(i);
	}



	visited.insert(std::pair<string, int>(str, myCount));// 3. Put the state and counter in the Map

	start = findZero(puzzle);
	iState.moveCount = myCount;// add counter which is 0 at the moment
	iState.stateString = str; // add array as a string, used to compare goalState
	for (int i = 0; i < Size; i++)// input array into node
	{
		iState.stateArray[i] = puzzle[i];
	}
	iState.Depth = depth; // add depth = 0

	iState.Heuristic = (depth + findMan(puzzle));// starting heuristic, find man returns int 

	iState.paths.push_back(first);// add starting location string
	iState.empty = start;// add location of empty spot in initial state


	pq2.push(iState);// priority queue for manhattan distance


	while (!pq2.empty())
	{
		int myCount = 0;

		node amancurrent = pq2.top();// am = A* manhattan, current node

		for (int i = 0; i < Size; i++)
		{
			workingState[i] = amancurrent.stateArray[i];
		}
		workingString = amancurrent.stateString;

		if (workingString == goalState)// output goal if goalstate
		{
			system("CLS");
			check = 1;
			cout << "CONGRATS A* Manhattan Distance COMPLETED SUCCESSFULLY...." << endl;
			cout << "Starting State: ";
			showInitial();
			cout << "Solution at Depth: " << amancurrent.Depth << endl;
			cout << "Path: " << endl;
			for (it = amancurrent.paths.begin(); it != amancurrent.paths.end(); it++)
			{
				cout << ' ' << *it << endl;
			}
			cout << "End." << endl << endl;
			cout << "Total Nodes: " << nodeCount << endl;
			cout << "Solution State: ";
			showWorking();
			cout << endl << endl;
			break;
		}
		pq2.pop();

		int zero = findZero(workingState);
		do
		{
			temp = findMoves(zero, workingState, myMoves, myCount, path);
			if (!history(temp))
			{

				nodeCount++;
				visited.insert(std::pair<string, int>(temp, myCount));

				nexts.Depth = amancurrent.Depth + 1;

				nexts.Heuristic = ((amancurrent.Depth + 1) + findMan(tempState));// calculate manhattan distance, findMan returns an int value

				nexts.moveCount = myCount;
				nexts.stateString = temp;
				for (int i = 0; i < Size; i++)
				{
					nexts.stateArray[i] = tempState[i];
				}

				nexts.paths = amancurrent.paths;
				nexts.paths.push_back(path);
				pq2.push(nexts);
				nexts.clear();

			}

		} while (myCount != myMoves);
	}


	if (check == 0)
	{
		system("CLS");
		cout << "UNSUCCESSFUL ATTEMPT. " << endl << endl;
		cout << "Total Nodes Explored: " << nodeCount << endl << endl;
		cout << "The unsolvable initial state is: ";
		showInitial();
		cout << endl;

	}

}