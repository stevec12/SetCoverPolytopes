#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "SetCover.h"
using namespace std;

int main(void){
	cout << "Set Cover Generator" << endl;
	cout << "Takes a Universe size (M) and an amount of sets (N)." << endl;
	cout << "Generates a set cover problem such that:" << endl;
	cout << " > No trivial solution: no subset equals U" << endl;
	cout << " > No empty subsets" << endl;
	cout << " > No duplicate subsets" << endl;
	cout << " > Full dimensional problem" << endl;
	cout << "Requires M >= 3, N >= 2" << endl;
	cout << "===============================================" << endl;
	cout << "Enter the size (M) of the Universe, {1,...,M}: ";
	int M;
	int N;
	bool invalid = true; 
	while (invalid) {
		cin >> M;
		cout << "Enter the number of sets to generate: ";
		cin >> N;
		if (N < 3) {
			cout << "Require N >= 3 for full dimensional problem." << endl;
			continue;
		}
		if (M < 2) {
			cout << "Require M >= 2" << endl;
			continue;
		}
		invalid = false;
	}
	//Take a filename to output Polymake formatted enumerated feasible solutions
	cout << "Provide a filename for polymake output: ";
	string fname;
	cin >> fname;
	fstream writef;
	writef.open(fname+".txt",fstream::out);
	//Construct a valid SCP
	SetCover scp{M,N};
	//Output sets
	cout << scp;
	scp.enumerate(writef);
}