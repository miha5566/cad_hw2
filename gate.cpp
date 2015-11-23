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
vector<gate*>& gate::getFanin(){return fanin;}
vector<gate*>& gate::getFanout(){return fanout;}
vector<string>& gate::getFaninName(){return FaninNames;}



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
	out<<"FaninNames:";
	for (vector<string>::iterator iter=g.FaninNames.begin() ; iter!=g.FaninNames.end() ;++iter)
		out<<*iter<<"|";
	out<<endl<<"Fanin:";
	for (vector<gate*>::iterator iter=g.fanin.begin() ; iter!=g.fanin.end() ;++iter)
		out<<(*iter)->getName()<<"|";
	out<<endl<<"Fanout:";
	for (vector<gate*>::iterator iter=g.fanout.begin() ; iter!=g.fanout.end() ;++iter)
		out<<(*iter)->getName()<<"|";
	out<<endl;
	return out;
}


bool gate::identical_structure(gate* gptr)
{
	if (this->getLogic() == gptr->getLogic())
	{
		if(gptr->getLogic() == 0) // INPUT
		{
			return true;
		}
		else if(gptr->getLogic() == 2) // INV
			return this->getFanin()[0]->identical_structure(gptr->getFanin()[0]);
		else if(gptr->getLogic() == 1) // NAND2
			return 
				(this->getFanin()[0]->identical_structure(gptr->getFanin()[0])
				&&this->getFanin()[1]->identical_structure(gptr->getFanin()[1])) ;
				//||(this->getFanin()[0]->identical_structure(gptr->getFanin()[1])
				//&&this->getFanin()[1]->identical_structure(gptr->getFanin()[0])) ;
	}
	else
	{
		return false;
	}
}

void gate::swapfanin()
{
	if (logic ==1)
	{
		gate* tmp = this->fanin[0];
		this->fanin[0] = this->fanin[1];
		this->fanin[1] = tmp;
		
		string temp = this->FaninNames[0];
		this->FaninNames[0] = this->FaninNames[1];
		this->FaninNames[1] = temp;
	}
}


/*
bool gate::isLeaf()
{
	if(logic==1)
	{	
		return fanin[0]->getLogic()==0 && fanin[1]->getLogic()==0 ;
	}
	else if(logic==2)
	{
		return fanin[0]->getLogic()==0 ;
	}
	else	
}*/

vector<string> gate::getRevFaninName()
{

	vector<string> ans;
	//cout<<FaninNames.size()<<endl;
	for(int i=int(FaninNames.size()-1);i>=0;i--)
	{
		//cout<<"yolo3"<<i<<endl;
		ans.push_back(FaninNames[i]);
		}
//cout<<"yolo2"<<endl;
	return ans;
}
