#include "setCover.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

//Invariant: Sn >= 3, required for full dimensional SCP
//           U >= 2
SetCover::SetCover(int U, int Sn): U{U}, Sn{Sn} {
	//Generate Sn 0 vectors of length |U|
	for (int i = 0; i < Sn; i++) {
		sets.emplace_back();
		for(int i=0; i < U; i++) {
			sets.back().emplace_back(0);
		}
	}
	//Generate subsets: Pseudorandomly
	//(Full dimensional condition)
	//Ensure 2 subsets cover each element
	//Repeat |U| times: roll 2 distinct numbers from 1 to Sn
	//Temp storage as 2 vectors 
	//(No trivial solution condition)
	//Ensure 1st entries of each do not recur throughout
	vector<int> v1;
	vector<int> v2;
	srand(time(NULL));
	for (int i = 0; i < U; i++) {
		v1.emplace_back(rand()%U);
		int v2_num;
		while ((v2_num = rand()%U) == v1.back());
		v2.emplace_back(v2_num);
	}
	//Ensure no trivial solution
	//First check if 1st entry of v1 recurs entirely through v1+v2
	bool trivial_v1 = true;
	bool trivial_v2 = true;
	int t1 = v1[0];
	int t2 = v2[0];
	for (int i = 1; i < Sn; i++) {
		//condition to end loop early
		if (!trivial_v1 && !trivial_v2) break;
		//store locally to avoid accessing multiple times
		int v1i = v1[i];
		int v2i = v2[i];

		if (t1 != v1i && t1 != v2i) trivial_v1 = false;
		if (t2 != v1i && t2 != v2i) trivial_v2 = false;
	}
	//Trivial cases
	//Regenerates the 2nd occurence 
	if (trivial_v1 && !trivial_v2) {
		int new_v;
		while ((new_v = rand()%U) == t2 || new_v == t1);
		if (v1[1] == t1) {
			v1[1] = new_v;
		}
		else {
			v2[1] = new_v;
		}
	}
	if (!trivial_v1 && trivial_v2) {
		int new_v;
		while ((new_v = rand()%U) == t1 || new_v == t2);
		if (v1[1] == t2) {
			v1[1] = new_v;
		}
		else {
			v2[1] = new_v;
		}
	}
	if (trivial_v1 && trivial_v2) {
		int new_v1;
		int new_v2;
		while ((new_v1 = rand()%U) == t1 || new_v1 == t2);
		while ((new_v2 = rand()%U) == t1 || new_v2 == t2);
		v1[1]=new_v1;
		v2[1]=new_v2;
	}
	//Use v1 and v2 to seed 1s to sets
	for(int i = 0; i < U; i++) {
		sets[v1[i]][i] = 1;
		sets[v2[i]][i] = 1;
	}
	//From 1 to Sn; 
	//Copy then randomize vector i 0s
	int count = 0;
	for (auto & vec: sets) {	
		vector<int> vec_copy;
		do {
			vec_copy = vec;
			for(auto & n : vec_copy) {
				//We want 50/50 chance of 1s, including the Full Dimensional 1s
				if (n == 0) {
					n = (rand()%(Sn*U) < Sn*U/2 - 2*U ? 1 : 0);
				}
			}
		//Ensure vector i not 0 or 1 vector
		//Backcheck to ensure vector i does not match prior vectors
		} while (trivial_vector(vec_copy) || vector_check(sets, vec_copy, count)); 
		//If unique, swap copy
		vec = vec_copy;
		count++;
	}
	
}


bool SetCover::vector_check(vector<vector<int>> & vecs, vector<int> & v, int count) {
	int iter = 0;
	for(auto vec: vecs) {
		if (iter == count) return false;
		else if (vec == v) return true;
		iter++;
	}
	return false;
}

bool SetCover::trivial_vector(vector<int> & v) {
	bool first = true;
	int target;
	for(auto n: v) {
		if (first) {
			first = false;
			target = n;
		}
		else if (n != target) return false;
	}
	return true;
}

void SetCover::enumerate(std::ostream & out) {
	//Form feasible solutions
	//Firstly, iterate through elements of universe: find subsets covering 
	//	ea element
	//This forms U sets from 1 to Sn: Call each set U_0,...,U_(M-1)
	vector<vector<int>> Ucovers;
	for(int i = 0; i < U; i++) {
		Ucovers.emplace_back();
		int setcount = 0;
		for (auto vec: sets) {
			if (vec[i] == 1) {
				Ucovers.back().emplace_back(setcount);
			}
			setcount++;
		}
	}
	//Form all combinations covering all elements: set K
	//U_0*...*U_(M-1) such combinations: K_1,...,K_d
	vector<vector<int>> feasibles;
	vector<int> dummy;
	possible_sets(feasibles, 0, Ucovers, dummy);
	//Sort and Take unique elements of each K_i
	for (auto & possible_covers : feasibles) {
		sort(possible_covers.begin(),possible_covers.end());
		auto it = unique(possible_covers.begin(), possible_covers.end());
		possible_covers.resize(distance(possible_covers.begin(),it));
	}
	//Sort and take unique elements of K
	sort(feasibles.begin(), feasibles.end());
	auto it = unique(feasibles.begin(), feasibles.end());
	feasibles.resize(distance(feasibles.begin(),it));
	//This forms our feasible solutions
	
	//Next we need to print them in Polymake format
	out << "use application \"polytope\";" << endl;
	out << "declare $p = new Polytope(POINTS=>[";
	while (feasibles.size()) {
			//Form a string based on feasible cover
			vector<int> feasible_vec;
			for (int i = 0; i < Sn; i++) feasible_vec.emplace_back(0);
			for (auto in: feasibles.back()) feasible_vec[in]=1;
			out << "[";
			int scount = 1;
			for (auto set: feasible_vec) {
				if (scount < Sn) out << set << ',';
				else out << set;
				scount++;
			}
			out << "]";
			if (feasibles.size()>1) out << ",";
			else out << "]";
			feasibles.pop_back();
		}
		out << ");" << endl;
		out << "print_constraints($p);" << endl;
		out << "exit";
}

void SetCover::possible_sets(vector<vector<int>> & feasibles, int level, 
vector<vector<int>> & Ucovers, vector<int> & cur) {
	if (level == U) {
		feasibles.push_back(cur);
	}
	else {
		for (auto covers : Ucovers[level]) {
			//Make a copy of cur for each cover
			vector<int> next{cur};
			next.emplace_back(covers);
			possible_sets(feasibles, level+1, Ucovers, next);
		}
	}
}

std::ostream & operator<<(std::ostream & out, const SetCover &sc) {
	int scount = 1;
	for (auto vec : sc.sets){
		out << "S" << scount << " = [";
		int icount = 1;
		for (auto n : vec) {
			//Comma management
			if (icount == sc.U) {
				out << n;
				break;
			}
			out << n << ',';
			icount++;
			
		}
		out << "]" << endl;
		scount++;
	}
	return out;
}
