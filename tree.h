//tree.h
#include <vector>
#include <iostream>
#include <string>
#include "gate.h"
using namespace std;
#ifndef _TREE_H_
#define _TREE_H_

class tree
{
    private:
    	
        vector<gate> gates; //topological order
		vector<gate*> gstack;
    public:
	    vector<gate*> topologicalOrderGates;
	    vector<tree*> expandtrees; //expand trees
    	int celldelay;
    	string name; //
        tree(); //
        ~tree(); //
        void addGate(string Name,int logic,vector<string> Fanin); //
        void addMissingGate();
        void topologicalSort();
        void adjust_gate_link();        //
        gate* getByName(string Name); //
        
        friend ostream& operator<< (ostream &out, tree &t); //
        
        vector<tree*>& expand();
        void recursive_expand(tree* t);
        tree* copy();
        
        //bool operator==(tree & a;);

};



#endif
