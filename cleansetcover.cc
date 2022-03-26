#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

void combn(vector<string> &,int,int,string);

int main(void){
	int M;
	cin >> M;
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
		cerr << "Not guaranteed full dimensional" << endl;
	}
	//Randomly seed more 1's
	for (int i=0;i<N;i++){
		for(int j=0;j<M;j++){
			if (rand()%10 >= 6) sets[j][i] = 1;
		}
	}
	//Next we want to enumerate all feasible solutions
	//This requires checking 2^N combinations
	//Firstly generate all binary vectors of length N
	vector<string> subsets;
	combn(subsets,1,N,"0");
	combn(subsets,1,N,"1");
    vector<string> feasibles;
	//Then check each option
    while (subsets.size()){
        //Check each element of universe covered
        bool deletion = false;
        for(int i=0;i<M;i++){
            bool covered = false;
            //Check if each set contains element i of universe
            for (int j=0;j<N;j++) {
                if (sets[i][j] == 1 && subsets.back()[j]=='1') {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                subsets.pop_back();
                deletion = true;
                break;
            }
        }
        if (!deletion) {
            feasibles.emplace_back(subsets.back());
            subsets.pop_back();
        }
    }
    //Finally, convert into polymake formatting and write to a file
	cout << endl;
	char input;
	cin >> input;
	if (input == 'y') {
		string fname;
		cin >> fname;
		fstream writef;
		writef.open(fname+".txt",fstream::out);
		//We write the sets into the script as comments too
	for (int i=0;i<N;i++){
		writef << "# Set " << i+1 << ": {";
		for (int j=0;j<M;j++){
			//Assumption: 1 not a garbage value
			writef << " " << ((sets[j][i] == 1) ? 1 : 0) << " ";
		}
		writef << "}" << endl;
	}	
		writef << "use application \"polytope\";" << endl;
		writef << "declare $p = new Polytope(POINTS=>[";
		while (feasibles.size()) {
			//Note homogenous vector form begins with x0 = 1
			writef << "[1,";
			//Iterate thru each string, put out comma seperated numbers
			for(int i = 0;i<N;i++){
				writef << feasibles.back()[i];
				if (i < N - 1) writef << ",";
			}
			writef << "]";
			if (feasibles.size()>1) writef << ",";
			else writef << "]";
			feasibles.pop_back();
		}
		writef << ");" << endl;
		writef << "print_constraints($p);" << endl;
		writef << "exit";
	}

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
