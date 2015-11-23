#include "gate.h"

gate::gate()
{
    fanin.reserve(50);
    fanout.reserve(50);
    FaninNames.reserve(50);        
    match_case.reserve(50);
    tempCellFanin.reserve(50);
}

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
	if (this->getLogic() == gptr->getLogic() )
	{
		if(gptr->getLogic() == 0) // INPUT
		{
		    this->tempCellFanin.push_back(this->getName());
			return true;
			
		}
		else if(gptr->getLogic() == 2 && this->fanout.size()<=1 ) // INV
			return this->getFanin()[0]->identical_structure(gptr->getFanin()[0]);
		else if(gptr->getLogic() == 1 && this->fanout.size()<=1 ) // NAND2
			return    this->getFanin()[0]->identical_structure(gptr->getFanin()[0])
				    &&this->getFanin()[1]->identical_structure(gptr->getFanin()[1]) ;
		else 
		    return false;
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
	return ans;
}

void gate::clearTmpCellFanin()
{
    this->tempCellFanin.clear();
}

void gate::addMatchCell(int delay,string cellName,string logicName,vector<string> Fanin)
{
    CELL c = {delay,cellName,logicName,Fanin};
    bool insert= true;
    for (vector<CELL>::iterator it=this->match_case.begin();it!=this->match_case.end();++it)
    {
        if(it->delay>c.delay) 
        {
            match_case.erase(it);
        }
        else if(it->delay<c.delay) 
        {
            insert = false;
        } 
    }
    if (insert)
        match_case.push_back(c);
}

int gate::getDelay()
{
    if (logic==0)
        return 0;
    else
        return match_case[0].delay;
}
