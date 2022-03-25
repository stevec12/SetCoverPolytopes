#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

void combn(vector<string> &,int,int,string);
int main(void){
	cout << "Set Cover Generator" << endl;
	cout << "Takes a Universe size and an amount of sets to generate, and attempts to produce a full dimensional set cover problem" << endl;
	cout << "The points generated represent the sets" << endl;
	cout << "===============================================" << endl;
	cout << "Enter the size (M) of the Universe, {1,...,M}: ";
	int M;
	cin >> M;
	cout << "Enter the number of sets to generate: ";
	int N;
	cin >> N;
	while (N < 1 || N > pow(2,M)) {
		cout << "Requires 0 < Input < 2^M" << endl;
		cin >> N;
	}
	//We need N length M vectors of {0,1} representing the sets
	//We use stack memory for simplicity
	int sets[M][N];
	//Firstly we determine if we can generate sets 
	//to provide a full-dimension polytope
	srand(time(NULL));
	bool full = (N >= 3) ? true : false;
	if (full) {
		for (int i=0;i<M;i++){
			//2 sets to include include i+1 from Universe
			int first = rand()%N;
			int second = rand()%N;
			while (second == first) {
				second = rand()%N;
			}	
			sets[i][first]=1;
			sets[i][second]=1;
		}
	}
	//If we cannot generate full dimension polytope, could
	//remove an element of the universe and try to generate
	//a full dimension polytope on a lower dimension
	else {
		cout << "Not guaranteed full dimensional" << endl;
	}
	//Randomly seed more 1's
	for (int i=0;i<N;i++){
		for(int j=0;j<M;j++){
			if (rand()%10 >= 6) sets[j][i] = 1;
		}
	}
	for (int i=0;i<N;i++){
		cout << "Set " << i+1 << ": {";
		for (int j=0;j<M;j++){
			//Assumption: 1 not a garbage value
			cout << " " << ((sets[j][i] == 1) ? 1 : 0) << " ";
		}
		cout << "}" << endl;
	}
	//Next we want to enumerate all feasible solutions
	//This requires checking 2^N combinations
	//Firstly generate all binary vectors of length N
	vector<string> feasibles;
	combn(feasibles,1,N,"0");
	combn(feasibles,1,N,"1");
	//Then check each option
	for(auto str: feasibles) cout << str << endl;
}

void combn(vector<string> & feasibles, int iter, int end, string cur) {
	if (iter == end) {
		feasibles.emplace_back(cur);
	}
	else {
		combn(feasibles, iter+1, end, cur+"0");
		combn(feasibles, iter+1, end, cur+"1");
	}
}
