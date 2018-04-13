// Jonathan Gregory
// This is the code portion for the CS 404 project.
// This program assumes that the maximum dimentions to be tested is 100
// For all files that are read in, it is assumed that there are more than 2 numbers 

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void Selection();
void FileEntery();
string removeSpaces(string);
int MatrixChainOrder(int p[], int);
int RemoveLargestDimensionFirst(vector<int>);
int MostExpensiveMatrixMultiplicationFirst(vector<int>);
int RemoveSmallestDimensionFirst(vector<int>);
int LeastExpensiveMatrixMultiplicationFirst(vector<int>);
int RandomExecutionTree(vector<int>);
int BestRandomExecutionTree(vector<int>);
int IgnorantApproach(vector<int>);
void MatrixChainHeuristicExperiment();
vector <double> BestAverageWorst(vector <vector <int> >, int);
void CreateChart(vector <vector <int> >);
vector<int>getHeuristics(vector<int>);

int main()
{
	cout << "Optimal Matrix Chain and Heuristics" << endl << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "This program can test a matix chain with the following heuristic strategies:" << endl;
	cout << "Strategy A - Remove Largest Dimension First" << endl;
	cout << "Strategy B - Do Most Expensive Matrix Multiplication First" << endl;
	cout << "Strategy C - Remove Smallest Dimensions First" << endl;
	cout << "Strategy D - Do Least Expensive Matrix Multiplication First" << endl;
	cout << "Strategy E - Random Execution Tree" << endl;
	cout << "Strategy F - Ignorant Approach" << endl;
	cout << "--------------------------------------------------------------" << endl << endl;
	Selection();
}

// This function gets the users selection to load the chain from a file or the Matrix Chain Heuristic Experiment
void Selection()
{
	string selection; // This holds the letter that the user types
	bool valid = false;

	cout << "You can either import a matrix chain from a file or see the Matrix Chain Heuristic Experiment." << endl;
	while (!valid) // keeps looping asking for what you want
	{
		cout << "\n\nPlease enter the \"FILE\" for file entery or enter \"EXPERIMENT\" for manual entery: ";
		cin >> selection; // gets the users input
		if (selection == "file" || selection == "FILE" || selection == "File" || selection == "f" || selection == "F")
		{
			FileEntery(); // loads the matrix chain from a file
		}
		else if (selection == "experiment" || selection == "EXPERIMENT" || selection == "Experiment" || selection == "e" || selection == "E")
		{
			MatrixChainHeuristicExperiment(); // goes to the experiment
		}
	}// end while checking input
}

// This function loads the numbers from a file the prints to the terminal the heuristics 
void FileEntery()
{
	int numbers = 0; // holds the number of numbers in the file
	ifstream fin; // input file
	string infilename; //user input file name

	// This loop gets the file name for the file to load from the user
	while (!fin.is_open())
	{
		cout << "Please enter the file name you wish to import the matrix chain: ";
		cin >> infilename;
		fin.open(infilename.c_str(), ios::in | ios::binary);
		// checks that the file eist and an be opened
		if (fin)
		{
			cout << "\nFound " << infilename << endl << endl; 
			break;
		}
	}

	// This loop loads the contents of the file into a vector after removing the spaces and commas
	vector <int> matrixChain; // holds the series of numbers for the matrix chain
	while (fin)
	{
		string s;
		if (!getline(fin, s)) break;
		istringstream ss(s);
		while (ss)
		{
			string s;
			if (!getline(ss, s, ',')) break; // removes the commas
			string tempNum = removeSpaces(s); // removes the spaces
			int temp = stoi(tempNum); // converts to an int
			matrixChain.push_back(temp); // places the number in the vector
		}
	}

	// This just prints out to the screen teh matrix chain
	cout << "Matrix Chain Loaded: ";
	for (int i = 0; i < matrixChain.size(); i++)
		cout << matrixChain[i] << " ";
	cout << endl << endl;

	fin.close(); // closes the input file

	vector<int> heuristics(getHeuristics(matrixChain)); // vector that holds all the heuristics for the matrix chain
	double best = heuristics[6]; // holds the optimal cost

	// Prints out the stats for the given file
	string line = "----------------------------------------------------------"; //line used around the Heurestic
	cout << line << endl;
	cout << "| Heuristics\t\t\t|  Cost  \t\t |\n";
	cout << line << endl;
	cout << "| Largest Dimension First\t| (" <<  heuristics[0] << ")\t" << heuristics[0] / best << "\t | " << endl;
	cout << "| Most Expensive First\t\t| (" <<   heuristics[1] << ")\t" << heuristics[1] / best << "\t | " << endl;
	cout << "| Smallest Dimension First\t| (" << heuristics[2] << ")\t" << heuristics[2] / best << "\t | " << endl;
	cout << "| Least Expensive First\t\t| (" <<  heuristics[3] << ")\t" << heuristics[3] / best << "\t | " << endl;
	cout << "| Best of Several Random\t| (" <<   heuristics[4] << ")\t" << heuristics[4] / best << "\t | " << endl;
	cout << "| One After the Other\t\t| (" <<    heuristics[5] << ")\t" << heuristics[5] / best << "\t | " << endl;
	cout << "| Chain Multiplication\t\t| (" <<   heuristics[6] << ")\t" << heuristics[6] / best << "\t | " << endl;
	cout << line << endl;
}

// This function removed spaces from a string and returns the result.
// This feature is not a requirment of the project. Original code for this
// can be found here: http://stackoverflow.com/questions/16329358/remove-spaces-from-a-string-in-c
string removeSpaces(string input)
{
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
	return input;
}

// This function is not one of the requirements for the project. I have included it for refference
// for the 'optimal solution' to compare the other heuristics against.
// Original source code can be found on the website listed below:
// http://www.geeksforgeeks.org/dynamic-programming-set-8-matrix-chain-multiplication/
// See the Cormen book for details of the following algorithm
// Matrix Ai has dimension p[i-1] x p[i] for i = 1..n
int MatrixChainOrder(int p[], int n)
{

	/* For simplicity of the program, one extra row and one
	extra column are allocated in m[][].  0th row and 0th
	column of m[][] are not used */
	int m[100][100];

	int i, j, k, L, q;

	/* m[i,j] = Minimum number of scalar multiplications needed
	to compute the matrix A[i]A[i+1]...A[j] = A[i..j] where
	dimension of A[i] is p[i-1] x p[i] */

	// cost is zero when multiplying one matrix.
	for (i = 1; i < n; i++)
		m[i][i] = 0;

	// L is chain length.
	for (L = 2; L < n; L++)
	{
		for (i = 1; i < n - L + 1; i++)
		{
			j = i + L - 1;
			m[i][j] = INT_MAX;
			for (k = i; k <= j - 1; k++)
			{
				// q = cost/scalar multiplications
				q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (q < m[i][j])
					m[i][j] = q;
			}
		}
	}

	return m[1][n - 1];
}

// This function calculates the expense of the matrix chain with the Remove Largest Dimension First heuristic
// inputs: vector of matric chain
// outputs: int of the cost to perform the entire matix chain
int RemoveLargestDimensionFirst(vector<int> matrixChain)
{
	int totalCost = 0;// total cost 
	do // loops untill there are only 2 left
	{
		int subCost = 0; // cost for the individual operation
		int largestDimension = 1; // largest internal shared dimention

		// looks for the largest "inner dimention"
		for (int x = 1; x < matrixChain.size() - 1; x++)
		{
			if (matrixChain[x] >= matrixChain[largestDimension])
				largestDimension = x; // updates the largestDiamention if one is found
			 
		}

		// cost for the last matrix multiplication
		subCost = matrixChain[largestDimension - 1] * matrixChain[largestDimension] * matrixChain[largestDimension + 1]; 
		totalCost += subCost; // adds to the cumulative cost
		
		// The below code is used to print out the matrix chain to prove that this algoritm works
		/*for (int x = 1; x < matrixChain.size() ; x++)
		{
			cout << "[" << matrixChain[x - 1] << "," << matrixChain[x] << "] ";
		}
		cout << endl << "Cost to Multiply largest Innter Dimention [" << matrixChain[largestDimension - 1] << "," << matrixChain[largestDimension] << "] & [";
		cout << matrixChain[largestDimension] << "," << matrixChain[largestDimension + 1] << "] = " << subCost << endl << endl;
		*/

		matrixChain.erase(matrixChain.begin() + largestDimension); // removes the merged 
	} while (matrixChain.size() > 2);
	//cout << "Total cost: " << totalCost << endl;
	return totalCost;
}

// This function does the Most Expensive Matrix Multiplication First
// inputs: vector of matric chain
// outputs: int of the cost to perform the entire matix chain
int MostExpensiveMatrixMultiplicationFirst(vector<int> matrixChain)
{
	int totalCost = 0;// total cost 
	do // loops untill there are only 2 left
	{
		int subCost = 0; // cost for the individual operation
		int mostExpensiveSet = 0; // spot for the most expensive operation
		int mostExpensiveCost = 0; // value spot for the next most expensive

		// looks for the "most expensive" operation
		for (int x = 1; x < matrixChain.size() - 1; x++)
		{
			if (matrixChain[x - 1] * matrixChain[x] * matrixChain[x + 1] >= mostExpensiveCost)
			{
				mostExpensiveSet = x;
				mostExpensiveCost = matrixChain[x - 1] * matrixChain[x] * matrixChain[x + 1];
			}
		}
		subCost = matrixChain[mostExpensiveSet - 1] * matrixChain[mostExpensiveSet] * matrixChain[mostExpensiveSet + 1]; // cost for the last matrix multiplication
		totalCost += subCost; // adds to the cumulative cost

		//// The below code is used to print out the matrix chain to prove that this algoritm works
		//for (int x = 1; x < matrixChain.size() ; x++)
		//{
		//	cout << "[" << matrixChain[x - 1] << "," << matrixChain[x] << "] ";
		//}
		//cout << endl << "Cost to Multiply Most Expensive Pair [" << matrixChain[mostExpensiveSet - 1] << "," << matrixChain[mostExpensiveSet] << "] & [";
		//cout << matrixChain[mostExpensiveSet] << "," << matrixChain[mostExpensiveSet + 1] << "] = " << subCost << endl << endl;
		

		matrixChain.erase(matrixChain.begin() + mostExpensiveSet); // removes the merged 
	} while (matrixChain.size() > 2);
	//cout << "Total cost: " << totalCost << endl;
	return totalCost;
}

// This function calculates the expence of the matrix chain with the smallest Dimension First heuristic
// inputs: vector of matric chain
// outputs: int of the cost to perform the entire matix chain
int RemoveSmallestDimensionFirst(vector<int> matrixChain)
{
	int totalCost = 0;// total cost 
	do // loops untill there are only 2 left
	{
		int subCost = 0; // cost for the individual operation
		int smallestDimension = 1; // smallest internal shared dimention

		// looks for the smallest "inner dimention"
		for (int x = 1; x < matrixChain.size() - 1; x++)
		{
			if (matrixChain[x] <= matrixChain[smallestDimension])
				smallestDimension = x;// updates the smallestDimension if one is found
		}
		subCost = matrixChain[smallestDimension - 1] * matrixChain[smallestDimension] * matrixChain[smallestDimension + 1];// cost for the last matrix multiplication
		totalCost += subCost;// adds to the cumulative cost

		//// The below code is used to print out the matrix chain to prove that this algoritm works
		//for (int x = 1; x < matrixChain.size(); x++)
		//{
		//	cout << "[" << matrixChain[x - 1] << "," << matrixChain[x] << "] ";
		//}
		//cout << endl << "Cost to Multiply the Smallest Innter dimension Pair [" << matrixChain[smallestDimension - 1] << "," << matrixChain[smallestDimension] << "] & [";
		//cout << matrixChain[smallestDimension] << "," << matrixChain[smallestDimension + 1] << "] = " << subCost << endl << endl;

		matrixChain.erase(matrixChain.begin() + smallestDimension); // removes the merged 
	} while (matrixChain.size() > 2);
	//cout << "Total cost: " << totalCost << endl;
	return totalCost;
}

// This function does the Least Expensive Matrix Multiplication First
// inputs: vector of matric chain
// outputs: int of the cost to perform the entire matix chain
int LeastExpensiveMatrixMultiplicationFirst(vector<int> matrixChain)
{
	int totalCost = 0;// total cost 
	do // loops untill there are only 2 left
	{
		int subCost = 0; // cost for the individual operation
		int leastExpensiveSet = 0;// spot for the least expensive operation
		int leastExpensiveCost = matrixChain[0] * matrixChain[1] * matrixChain[2]; // coast for the very first in order operation. must set to any cost

		// looks for the "least expensive" operation
		for (int x = 1; x < matrixChain.size() - 1; x++)
		{
			if (matrixChain[x - 1] * matrixChain[x] * matrixChain[x + 1] <= leastExpensiveCost)
			{
				leastExpensiveCost = matrixChain[x - 1] * matrixChain[x] * matrixChain[x + 1];
				leastExpensiveSet = x;
			}
		}

		subCost = matrixChain[leastExpensiveSet - 1] * matrixChain[leastExpensiveSet] * matrixChain[leastExpensiveSet + 1];// cost for the last matrix multiplication
		totalCost += subCost; // adds to the cumulative cost

		/*// The below code is used to print out the matrix chain to prove that this algoritm works
		for (int x = 1; x < matrixChain.size(); x++)
		{
			cout << "[" << matrixChain[x - 1] << "," << matrixChain[x] << "] ";
		}
		cout << endl << "Cost to Multiply the Least Expensive Matrix Multiplication Pair [" << matrixChain[leastExpensiveSet - 1] << "," << matrixChain[leastExpensiveSet] << "] & [";
		cout << matrixChain[leastExpensiveSet] << "," << matrixChain[leastExpensiveSet + 1] << "] = " << subCost << endl << endl;*/

		matrixChain.erase(matrixChain.begin() + leastExpensiveSet);// removes the merged 
	} while (matrixChain.size() > 2);
	//cout << "Total cost: " << totalCost << endl;
	return totalCost;
}

// this function looks for the best, worst, and average case for a specific heuristics
// input: 2d vector of heurestics results
// input: int of the heurestic we want the best, worst, and average for
// output: vector of ints with the best, worst, and average, along with the assosiated optimal cost 
vector <double> BestAverageWorst(vector <vector <int> > results, int heurestic)
{
	int best = results[0][heurestic];
	int sum = 0;
	int worst = results[0][heurestic];
	int optimalForBest = 0; // this holds the optimal (matrix tree) value for the assosiated heurestic
	int optimalForWorst = 0;// this holds the optimal (matrix tree) value for the assosiated heurestic
	
	int optimalSum = 0; //this holds the sum of the optimal matrix chain cost
	vector <double> baw; // vector of strings best, average, & worst heurestic
	// loops through all the 30 different heurestic stats looking for the best, worst and average
	for (int x = 0; x < results.size(); x++)
	{
		// gets the best (smallest) result
		if (results[x][heurestic] <= best)
		{
			best = results[x][heurestic];
			optimalForBest = results[x][6];
		}
			
		optimalSum += results[x][6]; // adds to the sum
		sum += results[x][heurestic]; // adds to the sum

		// gets the worst (largest) result
		if (results[x][heurestic] >= worst)
		{
			worst = results[x][heurestic];
			optimalForWorst = results[x][6];
		}

	}
	int optimalForAverage = (optimalSum / results.size());;// this holds the optimal (matrix tree) value for the assosiated heurestic
	int average = (sum / results.size()); // computes the average
	int optimal;
	baw.push_back(best); // adds the best result
	baw.push_back(average); // adds the best result
	baw.push_back(worst); // adds the best result
	baw.push_back(optimalForBest); // retuns the optimal cost for the best trial value
	baw.push_back(optimalForAverage); // retuns the optimal cost for the average trial value
	baw.push_back(optimalForWorst); // retuns the optimal cost for the worst trial value

	return baw;
}

// This function provides takes in the vector of the matrix chain and creates a random
// execution tree. This ends up getting called 2n times for each matrix chain
// input: vector of matrix chain
// output: the total cost for the best exection tree
int RandomExecutionTree(vector<int> matrixChain)
{
	int totalCost = 0;// total cost 
	int subCost;
	do
	{
		int spot = rand() % (matrixChain.size() - 2) + 1;
		subCost = matrixChain[spot - 1] * matrixChain[spot] * matrixChain[spot + 1];
		totalCost += subCost;

		// The below code is used to print out the matrix chain to prove that this algoritm works
		//for (int x = 1; x < matrixChain.size(); x++)
		//{
		//	cout << "[" << matrixChain[x - 1] << "," << matrixChain[x] << "] ";
		//}
		//cout << endl << "Cost to Multiply using a Random Execution Tree [" << matrixChain[spot - 1] << "," << matrixChain[spot] << "] & [";
		//cout << matrixChain[spot] << "," << matrixChain[spot + 1] << "] = " << subCost << endl << endl;

		matrixChain.erase(matrixChain.begin() + spot);
	} while (matrixChain.size() > 2);
	//cout << "Total cost: " << totalCost << endl;
	return totalCost;
}

// This function provides takes in the vector of the matrix chain and creates a random
// execution tree. It then makes l exection trees where l = 2n and used the lowest cost 
// input: vector of matrix chain
// output: the total cost for the best exection tree
int BestRandomExecutionTree(vector<int> matrixChain)
{

	int totalCost = RandomExecutionTree(matrixChain); // total cost for the best execution tree
	for (int L = 2 * matrixChain.size() - 1; L > 0; L--)
	{
		int tempRandomExecutionTree = RandomExecutionTree(matrixChain);

		// Updates the total cost if the newest execution tree cost less
		if (tempRandomExecutionTree < totalCost)
			totalCost = tempRandomExecutionTree;
	}
	return totalCost;
}

// This function calculates the cost of just taking B1 x B2, multiply the result with B3, multiply the result with B4. and so on
// inputs: vector of matric chain
// outputs: int of the cost to perform the entire matix chain
int IgnorantApproach(vector<int> matrixChain)
{
	int totalCost = 0;// total cost 
	do // loops untill there are only 2 left
	{
		int subCost = matrixChain[0] * matrixChain[1] * matrixChain[2]; // gets the cost for the individual operation
		totalCost += subCost; // adds the cost for the operation to the cost of the rest of the operations

		// The below code is used to print out the matrix chain to prove that this algoritm works
		/*for (int x = 1; x < matrixChain.size(); x++)
		{
			cout << "[" << matrixChain[x - 1] << "," << matrixChain[x] << "] ";
		}
		cout << endl << "Cost to multiply using the Ignorant Approach [" << matrixChain[0] << "," << matrixChain[1] << "] & [";
		cout << matrixChain[1] << "," << matrixChain[2] << "] = " << subCost << endl << endl;*/

		matrixChain.erase(matrixChain.begin() + 1); // removes the matrix that was just multiplied
	} while (matrixChain.size() > 2);
	//cout << "Total cost: " << totalCost << endl;
	return totalCost;
}

// This function performs the Matrix ChainHeuristic Experiment
// It calculates the computational cost using the optimal calculation and the alternate strategies
// Then analyze the time complexity to deliver the blue print for the optimal calculation and the alternate strategies
void MatrixChainHeuristicExperiment()
{
	int lowValue = 7; // The lower bound for possible dimentions as provided in the instructions
	int highValue = 17; // The lower bound for possible dimentions as provided in the instructions

	// This loop sets the dimentions to n = 10; 15; 20; 25
	for (int n = 10; n <= 25; n += 5)
	{
		vector <vector <int> > results; // vector that holds the heuristics results[30][6]
		string line = "-------------------------------------------------------------------------------------------------"; //line used around the Heurestic
		cout << "\n\n" << line << "\n| Optimal Cost \t\t\t|\t n = " << n << "\t\t\t\t\t\t\t|" << endl;
		cout << "| \t\t\t\t| \t Mo[1," << n << "]\t\t\t\t\t\t|" << endl;

		// This loop performs 30 matrix chains with n dimentions for getting averages
		for (int r = 1; r < 30; r++)
		{
			//creates the vector to hold the rand input dimensions
			vector<int> matrixChain;

			// Fills the matrix chain with random numbers between the high and low values coded above
			for (int i = 0; i < n; i++)
			{
				int randomNumber = rand() % (highValue - lowValue + 1) + lowValue;
				matrixChain.push_back(randomNumber);
			}

			results.push_back(getHeuristics(matrixChain));
		}
		CreateChart(results);
	}
}

// This function takes in a double vector of all the heuristics and prints out the output to the screen
// input: double vector of all heuristics
// output: void
	//0 RemoveLargestDimensionFirst
	//1 MostExpensiveMatrixMultiplicationFirst
	//2 RemoveSmallestDimensionFirst
	//3 LeastExpensiveMatrixMultiplicationFirst
	//4 RandomExecutionTree
	//5 IgnorantApproach
	//6 MatrixChainOrder
	//7 n=?
void CreateChart(vector <vector <int> > results)
{
	vector <double> baw; // vector that the best, average, and worst for the given Heurestic
	string line = "-------------------------------------------------------------------------------------------------"; //line used around the Heurestic
	cout << "| Heuristics\t\t\t|\t  Yb \t\t\t Ya \t\t\tYw\t|\n";
	cout << line << endl;

	// gets the best, average, and worst of the Remove Largest Dimension First Heurestic
	baw = BestAverageWorst(results, 0);
	cout << "| Largest Dimension First\t| (" << baw[0] << ") " << baw[0]/ baw[3] << "  -\t (" << baw[1] << ") " << baw[1] / baw[4] << "   -\t(" << baw[2] << ") " << baw[2] / baw[5] << "\t|" << endl;

	// gets the best, average, and worst of the Remove Most Expensive First Heurestic
	baw = BestAverageWorst(results, 1);
	cout << "| Most Expensive First\t\t| (" << baw[0] << ") " << baw[0] / baw[3] << "  -\t (" << baw[1] << ") " << baw[1] / baw[4] << "   -\t(" << baw[2] << ") " << baw[2] / baw[5] << "\t|" << endl;

	// gets the best, average, and worst of the Remove Smallest Dimension Firs Heurestic
	baw = BestAverageWorst(results, 2);
	cout << "| Smallest Dimension First\t| (" << baw[0] << ") " << baw[0] / baw[3] << "  -\t (" << baw[1] << ") " << baw[1] / baw[4] << "   -\t(" << baw[2] << ") " << baw[2] / baw[5] << "\t|" << endl;

	// gets the best, average, and worst of the Remove Least Expensive First Heurestic
	baw = BestAverageWorst(results, 3);
	cout << "| Least Expensive First\t\t| (" << baw[0] << ") " << baw[0] / baw[3] << "  -\t (" << baw[1] << ") " << baw[1] / baw[4] << "   -\t(" << baw[2] << ") " << baw[2] / baw[5] << "\t|" << endl;

	// gets the best, average, and worst of the Best of Several Random Heurestic
	baw = BestAverageWorst(results, 4);
	cout << "| Best of Several Random\t| (" << baw[0] << ") " << baw[0] / baw[3] << "  -\t (" << baw[1] << ") " << baw[1] / baw[4] << "   -\t(" << baw[2] << ") " << baw[2] / baw[5] << "\t|" << endl;

	// gets the best, average, and worst of the One After the Other Heurestic
	baw = BestAverageWorst(results, 5);
	cout << "| One After the Other\t\t| (" << baw[0] << ") " << baw[0] / baw[3] << "  -\t (" << baw[1] << ") " << baw[1] / baw[4] << "   -\t(" << baw[2] << ") " << baw[2] / baw[5] << "\t|" << endl;

	//// gets the best, average, and worst of the One After the Other Heurestic
	//baw = BestAverageWorst(results, 6);
	//cout << "| Using a Matrix Chain\t\t| (" << baw[0] << ") " << baw[0] / baw[1] << "\t " << baw[1] << "\t" << baw[2] << "\t| " << endl;

	cout << line << endl;
}

// This function loads all the heuristic values into a vector 
// This is just to make it simpler to perform all the heuristics at ones
// input: vector of the matrix chain
// output: vector with all the heuristics loaded into it in the order of the following:
	//RemoveLargestDimensionFirst
	//MostExpensiveMatrixMultiplicationFirst
	//RemoveSmallestDimensionFirst
	//LeastExpensiveMatrixMultiplicationFirst
	//RandomExecutionTree
	//IgnorantApproach
	//MatrixChainOrder
vector<int>getHeuristics(vector<int> matrixChain)
{
	vector<int>allHeuristics; // stores all the heuristics for the matrix chain

	allHeuristics.push_back(RemoveLargestDimensionFirst(matrixChain));
	allHeuristics.push_back(MostExpensiveMatrixMultiplicationFirst(matrixChain));
	allHeuristics.push_back(RemoveSmallestDimensionFirst(matrixChain));
	allHeuristics.push_back(LeastExpensiveMatrixMultiplicationFirst(matrixChain));
	allHeuristics.push_back(BestRandomExecutionTree(matrixChain));
	allHeuristics.push_back(IgnorantApproach(matrixChain));

	// This converts the vector to an array for the sake of using a function to find the Matrix Chain Order.
	int matrixChainArray[100]; // Creates an array to send to the MatrixChainOrder
	for (int i = 0; i < matrixChain.size(); ++i)
		matrixChainArray[i] = matrixChain[i];

	allHeuristics.push_back(MatrixChainOrder(matrixChainArray, matrixChain.size()));

	return allHeuristics;
}
