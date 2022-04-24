//Set Cover class stores set cover problems (SCPs) with:
// > No trivial solution, no subset Si=U
// > No empty subsets
// > No duplicate subsets
// > Full dimensional problem
#include <vector>
#include <iostream>

//Stores an SCP
class SetCover {
    //size of universe, |U|
    int U;
    //# of subsets in SCP
    int Sn;
    //sets stored as int[Sn][U] 2D vector of integers
    //1 or 0 denoting unit inclusion
    std::vector<std::vector<int>> sets;
    
    //Checks if the given vector matches one already in the set of vectors
    //Return true if v is in vecs
    //Return false if v is not in vecs
    bool vector_check(std::vector<std::vector<int>> & vecs, std::vector<int> & v, int count);
    //Checks if a vector is all 1 or 0
    bool trivial_vector(std::vector<int> & v);
    
    //Recursive function to create all possible covers
    void possible_sets(std::vector<std::vector<int>> & possibles, int level, 
std::vector<std::vector<int>> & Ucovers, std::vector<int> & cur);
    friend std::ostream & operator<<(std::ostream & out, const SetCover &sc);
    public: 
        SetCover(int U, int Sn);
        //Enumerate all valid set covers in Polymake format
        void enumerate(std::ostream & out);
};

//Output sets of the SCP
std::ostream & operator<<(std::ostream & out, const SetCover &sc);
