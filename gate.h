//gate.h

#include <iostream>
#include <string>
#include <vector>
using namespace std;
#ifndef _GATE_H_
#define _GATE_H_

class gate
{
    friend class tree;
    private:
        int logic;
        /*
            0:input 
            1:nand2
            2:inv
            3:nor2
            4:and2
            5:or2
        */
        vector<gate*> fanin;
        vector<gate*> fanout;
        vector<string> FaninNames;
        string Name;
    public:
        gate(); //
        gate(string gate_name,int gate_logic,vector<string> faninName); //
        ~gate(); //
        int getLogic();
		string getName();
        vector<gate*> getFanin(); //
        vector<string>& getFaninName(); //
        bool not_inside(vector<gate*>& v,gate* gptr);
        void addFanin(gate* gptr); //
        void addFanout(gate* gptr); //
        friend ostream& operator<< (ostream &out, gate &g); //
        
        ////for topological 
};



#endif
