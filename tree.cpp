//#include <vector>
//#include <iostream>
//#include <string>
#include "gate.h"
#include "tree.h"
using namespace std;
tree::tree()
{ 
gates.reserve(100);
gstack.reserve(100);
topologicalOrderGates.reserve(100);
expandtrees.reserve(100);
}
tree::~tree(){}

void tree::addGate(string Name,int logic,vector<string> Fanin)
{
	gate g(Name,logic,Fanin);
	gates.push_back(g);
	
	return;
}


void tree::addMissingGate()
{
	vector<gate> tempgates;
	for (vector<gate>::iterator iter=gates.begin() ; iter!=gates.end() ; ++iter)
	{
		vector<string>::iterator iter2;
		for (iter2 = iter->getFaninName().begin(); iter2!=iter->getFaninName().end() ; ++iter2)
		{
			string s = *iter2;
			gate* g = this->getByName(s);
			if (g==NULL)
			{
				vector<string> v;
				gate g(s,0,v);
				tempgates.push_back(g);
			}		
		}
	}
	for (vector<gate>::iterator iter=tempgates.begin() ; iter!=tempgates.end() ; ++iter)
	{
		gates.push_back(*iter);
	}
	
	return;
}

void tree::topologicalSort()
{	
	for (vector<gate>::iterator gptr=gates.begin() ; gptr!=gates.end() ; ++gptr)
	{
		gptr->waiting_count = gptr->getFanin().size();
		gptr->done = false;	
	}
	//cout<<this->name<<endl;
	
	while(1)
	{
		if(topologicalOrderGates.size()==gates.size())
			break;
		for (vector<gate>::iterator gptr=gates.begin() ; gptr!=gates.end() ; ++gptr)
		{
			if (!gptr->done && gptr->waiting_count==0)
			{	
				//cout<<gptr->getName()<<endl;
				topologicalOrderGates.push_back (&(*gptr));
				gptr->done = true;
				for(unsigned i=0;i<gptr->getFanout().size();i++)
				{
					gptr->getFanout()[i]->waiting_count -= 1;
					//cout<<"\t"<<gptr->getFanout()[i]->getName()<<endl;
				}
			}		
		}
	}
}

void tree::adjust_gate_link()
{
	this -> addMissingGate();
	for (vector<gate>::iterator iter=gates.begin() ; iter!=gates.end() ; ++iter)
	{
		vector<string>::iterator iter2;
		for (iter2 = iter->getFaninName().begin(); iter2!=iter->getFaninName().end() ; ++iter2)
		{
			string s = *iter2;
			gate* g = this->getByName(s);
			iter->addFanin(g);
			g -> addFanout(&(*iter));
			//cout<<s<<" ends here"<<endl;
		}
	}
	topologicalOrderGates.clear();
	this -> topologicalSort();
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
	out<<"NAME:"<<t.name<<"|DELAY:"<<t.celldelay<<endl;
	/*
	for (vector<gate>::iterator iter=t.gates.begin() ; iter!=t.gates.end() ; ++iter)
		out <<"----"<<endl<< *iter;
	*/	
	/**/
	for (unsigned i =0;i<t.topologicalOrderGates.size();i++)
		out <<"----"<<endl<< *(t.topologicalOrderGates[i]);
	
	out<<"----"<<endl;
	return out;
}


vector<tree*>& tree::expand()
{
	cout<<(topologicalOrderGates.size())<<endl;
	cout<<(this->name)<<endl;
	
	if(this->topologicalOrderGates[0]->getLogic() == 1)
	{
		tree * newt = new tree;
		tree * newt2 = new tree;
		
		gate * gptr = this->topologicalOrderGates[0];
		newt->addGate(gptr->getName(),gptr->getLogic(),gptr->getFaninName());
		newt2->addGate(gptr->getName(),gptr->getLogic(),gptr->getRevFaninName());
		newt->celldelay=this->celldelay;
    	newt->name=this->name; 
    	newt2->celldelay=this->celldelay;
    	newt2->name=this->name; 
 		expandtrees.push_back(newt);
 		expandtrees.push_back(newt2);
	}
	else if(this->topologicalOrderGates[0]->getLogic() == 2 ||this->topologicalOrderGates[0]->getLogic() == 0 )
	{
		tree * newt = new tree;
		gate * gptr = this->topologicalOrderGates[0];
		newt->addGate(gptr->getName(),gptr->getLogic(),gptr->getFaninName());
		newt->celldelay=this->celldelay;
    	newt->name=this->name; 
		expandtrees.push_back(newt);
	}

	for (unsigned index=1;index < this->topologicalOrderGates.size();index++)
	{
		if(this->topologicalOrderGates[index]->getLogic() == 1)//only for nand
		{
			
			gate * gptr = this->topologicalOrderGates[index];
			vector<tree*> temp;
			for(unsigned i=0;i<expandtrees.size();i++)
			{
				temp.push_back(expandtrees[i]->copy());
				expandtrees[i]->addGate(gptr->getName(),gptr->getLogic(),gptr->getFaninName());
			}
			for(unsigned i=0;i<temp.size();i++)
			{	
				temp[i]->addGate(gptr->getName(),gptr->getLogic(),gptr->getRevFaninName());///
				expandtrees.push_back(temp[i]);
			}
				
		}
		else if(this->topologicalOrderGates[index]->getLogic() == 2||this->topologicalOrderGates[0]->getLogic() == 0 )
		{
			gate * gptr = this->topologicalOrderGates[index];
			for(unsigned i=0;i<expandtrees.size();i++)
			{
				expandtrees[i]->addGate(gptr->getName(),gptr->getLogic(),gptr->getFaninName());
			}
		}
	}
	/*
	for (vector<tree*>::iterator it = expandtrees.begin(); it != expandtrees.end() ;++it)
 	{

 		(*it)->adjust_gate_link();
 		cout<<*(*it)<<endl;

 	}*/
 	
 	//cout<<"what"<<endl;
 	
 	
	return this->expandtrees;
}

tree* tree::copy()
{
	tree* t = new tree;
	for (vector<gate>::iterator iter=gates.begin() ; iter!=gates.end() ; ++iter)
		t->addGate(iter->getName(),iter->getLogic(),iter->getFaninName());
	t->celldelay=this->celldelay;
    t->name=this->name; //
	t->adjust_gate_link();
	return t;
}
