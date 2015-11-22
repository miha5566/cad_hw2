//gate.h

#include <iostream>
#include <string>
#include <vector>
using namespace std;
#ifndef _GATE_H_
#define _GATE_H_

struct CELL
{
	int delay;
	string name;
	string logic_name;
	string out;
	vector<string> FaninNames;
};

class gate
{
    friend class tree;
    private:
        int logic;
        /*
            0:input 
            1:nand2
            2:inv
        */
        vector<gate*> fanin;
        vector<gate*> fanout;
        vector<string> FaninNames;
        string Name;
        
        vector <CELL> match_case;
    public:
    	//for topological
    	int waiting_count;
    	bool done;
    	//for matching cell
    	bool visited;
    	
        gate(); //
        gate(string gate_name,int gate_logic,vector<string> faninName); //
        ~gate(); //
        int getLogic();
		string getName();
        vector<gate*>& getFanin(); //
        vector<gate*>& getFanout(); //
        vector<string>& getFaninName(); //
        bool not_inside(vector<gate*>& v,gate* gptr);
        void addFanin(gate* gptr); //
        void addFanout(gate* gptr); //
        friend ostream& operator<< (ostream &out, gate &g); //
        
        bool identical_structure(gate * gptr);
        void swapfanin();
        
        
};



#endif
