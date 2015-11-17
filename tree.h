//tree.h
#include <vector>

#ifndef _TREE_H_
#define _TREE_H_
class tree:
{
    private:
        vector<gate*> gates; //topological order

    public:
        gate* getByName();
        tree();
        ~tree();
        void addGate(string Name,int logic,string * Fanin,int Fanin_num);
        void addInput(string Name,int logic)
        void adjust_fanout();
}



void tree::addGate(string Name,string * Fanin,int Fanin_num)
{
}

void tree::adjust_fanout()
{
}

#endif
