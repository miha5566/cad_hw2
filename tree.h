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

    public:
    	string name; //
        tree(); //
        ~tree(); //
        void addGate(string Name,int logic,vector<string> Fanin); //
        void adjust_gate_link();        //
        gate* getByName(string Name); //
        
        friend ostream& operator<< (ostream &out, tree &t); //
        //bool operator==(tree & a;);

};



#endif
