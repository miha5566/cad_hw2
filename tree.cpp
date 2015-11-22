#include <vector>
#include <iostream>
#include "gate.h"
#include "tree.h"
using namespace std;
tree::tree(){}
tree::~tree(){}

void tree::addGate(string Name,int logic,vector<string> Fanin)
{
	gate g(Name,logic,Fanin);
	gates.push_back(g);
	return;
}

void tree::adjust_gate_link()
{
	for (vector<gate>::iterator iter=gates.begin() ; iter!=gates.end() ; ++iter)
	{
		vector<string>::iterator iter2;
		for (iter2 = iter->getFaninName().begin(); iter2!=iter->getFaninName().end() ; ++iter2)
		{
			string s = *iter2;
			gate* g = this->getByName(s);
			iter->addFanin(g);
			g -> addFanout(&(*iter));
		}
	}
}


gate* tree::getByName(string name)
{
	for (vector<gate>::iterator iter=gates.begin() ; iter!=gates.end() ; ++iter)
	{
		if (iter->getName() == name)
		{
			return &(*iter);
		}
	}
	// if not found
	return NULL;
}

ostream& operator<< (ostream &out, tree &t)
{
	for (vector<gate>::iterator iter=t.gates.begin() ; iter!=t.gates.end() ; ++iter)
		out <<"----"<<endl<< *iter;
	out<<"----"<<endl;
	return out;
}
