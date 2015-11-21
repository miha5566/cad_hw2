#include "gate.h"

gate::gate(){}

gate::gate(string gate_name,int gate_logic,vector<string> faninName)
{
	this->Name = gate_name;
	this->logic = gate_logic;
	FaninNames = faninName;
}

gate::~gate(){}
int gate::getLogic(){return logic;}
string gate::getName(){return Name;}
vector<gate*> gate::getFanin(){return fanin;}
vector<string> gate::getFaninName(){return FaninNames;}

bool gate::not_inside(vector<gate*>& v,gate* gptr)
{
	for (vector<gate*>::iterator iter=v.begin() ; iter!=v.end() ;++iter)
	{
		if (*iter == gptr)
			return false;
	}
	return true;
}

void gate::addFanin(gate* gptr)
{
	if (this->not_inside(fanin,gptr))
		fanin.push_back(gptr);
}
void gate::addFanout(gate* gptr)
{
	if (this->not_inside(fanout,gptr))
		fanout.push_back(gptr);
}

ostream& operator<< (ostream &out, gate &g)
{
	out<<"Logic:"<<g.logic<<";Name:"<<g.Name<<endl;
	out<<"Fanin:";
	for (vector<string>::iterator iter=g.FaninNames.begin() ; iter!=g.FaninNames.end() ;++iter)
	{
		out<<*iter<<"|";
	}
	out<<endl;
	return out;
}
