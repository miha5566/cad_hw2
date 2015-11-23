//#include <vector>
//#include <iostream>
//#include <string>
#include "gate.h"
#include "tree.h"
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;
tree::tree()
{ 
    gates.reserve(100);
    //gstack.reserve(100);
    topologicalOrderGates.reserve(100);
    expandtrees.reserve(100);
}
tree::~tree()
{   
    for (unsigned i=0;i<expandtrees.size();i++)
    {
        delete expandtrees[i];
    }
}

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
/*******************************************************/
///for cell expansion
vector<tree*>& tree::expand()
{
	//cout<<(topologicalOrderGates.size())<<endl;
	//cout<<(this->name)<<endl;
	
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
	
	for (vector<tree*>::iterator it = expandtrees.begin(); it != expandtrees.end() ;++it)
 	{

 		(*it)->adjust_gate_link();
 	//	cout<<*(*it)<<endl;

 	}
 	
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

gate* tree::getRoot()
{
    return topologicalOrderGates.back();
}
/*******************************************************/
///for match in main ckt
void tree::match_celllib(vector<tree*> celllib)
{
	gate* gptr;
	for(int i =0;i<int(topologicalOrderGates.size());i++)
    {
    	gptr = topologicalOrderGates[i];
		tree* tptr;
	
		for(unsigned i =0;i<celllib.size();i++)
		{
			gptr -> clearTmpCellFanin();
			tptr = celllib[i];
			this->match (gptr,tptr);
		}
	}
}
void tree::match(gate* gptr,tree* cell)
{       
        if (gptr-> identical(cell->getRoot()) )
        {
            int d = 0;

            
            for (int j=0; j< int(gptr->tempCellFanin.size());j++) //choose the max delay of inputs
            {
                string n = gptr->tempCellFanin[j]; //gate name
                int dvalue = this->getByName(n)->getDelay(); //delay
              	if (dvalue>d)
                {
                    d = dvalue;
                }
                
            }
            //cout<<gptr->getName()<<":"<<cell->name<<":"<<cell->celldelay + d<<<endl;
            gptr->addMatchCell(cell->celldelay + d,gptr->getName(),cell->name,gptr->tempCellFanin);
            
        }

}


int stoi(string s)
{
	int ans;
	stringstream convert ;
	convert << s;
	convert >> ans;
	return ans;
}

bool cmp_str(const string& a,const string& b)
{
	if(a[0]>b[0]) //P comes first
		return true;	
	else if (a[0]==b[0])
		return stoi(a.substr(1,a.length()-1))< stoi(b.substr(1,b.length()-1) );
	else 
		return false;
	 //smaller comes first
}

void tree::output(char* outfileName)
{
    ofstream outfile;
    outfile.open(outfileName);
    
    vector<gate*> gstack;
    vector<gate*> output_list;
    
//    gate* gptr=this->getRoot();

	vector <gate*> all_node = this->all_out_node();
	int outdelay=0;
	for(vector <gate*>::iterator it = all_node.begin();it!=all_node.end();++it)
	{
		gstack.push_back (*it);
		if ((*it)->match_case[0].delay>outdelay)
			outdelay = (*it)->match_case[0].delay;
	}
	outfile<<outdelay<<endl<<endl;
    //outfile<<this->getRoot()->match_case[0].delay<<endl<<endl;

    while (gstack.size()>0)
    {
        gate* gptr = gstack.back();
        gstack.pop_back();
        if (gptr->getLogic()!=0)
        {
        	bool inside=false;
        	for(vector <gate*>::iterator it = output_list.begin();it!=output_list.end();++it)
        		if (gptr->getName() == (*it)->getName())
        		{
        			inside = true;
        			break;
        		}
        	if(!inside)
        		output_list.push_back(gptr);
            //outfile <<gptr->match_case[0].logic_name<<' '
            //        <<gptr->match_case[0].name<<' ';
            for (unsigned i=0;i<gptr->match_case[0].FaninNames.size();i++)
            {
                
                string gatename = gptr->match_case[0].FaninNames[i];
                gstack.push_back(this->getByName(gatename));   
                //outfile<<gatename<<' ';
            }
            //outfile<<endl;
        }
    } 
    sort(output_list.begin(),output_list.end());
    
    for(vector <gate*>::iterator it = output_list.begin();it!=output_list.end();++it)
    {
    	//sort((*it)->match_case[0].FaninNames.begin(),(*it)->match_case[0].FaninNames.end());
    	sort((*it)->match_case[0].FaninNames.begin(),(*it)->match_case[0].FaninNames.end(),cmp_str);
    	outfile <<(*it)->match_case[0].logic_name<<' '
            	<<(*it)->match_case[0].name<<' ';
        for (unsigned i=0;i<(*it)->match_case[0].FaninNames.size();i++)
        {  
            string gatename = (*it)->match_case[0].FaninNames[i];
            //gstack.push_back(this->getByName(gatename));   
            outfile<<gatename<<' ';
        }
        outfile<<endl;
    }       
    
    
    
    outfile.close();
}

vector <gate*> tree::all_out_node()
{
	vector <gate*> ans;
	for (vector<gate>::iterator iter=gates.begin() ; iter!=gates.end() ; ++iter)
	{
		if (iter->getFanout().size()!=1)
			ans.push_back(&(*iter));
	}
	return ans;
}














