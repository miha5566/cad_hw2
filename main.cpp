#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "gate.h"
#include "tree.h"
using namespace std;

int string_to_int(string s)
{
	int ans;
	stringstream convert ;
	convert << s;
	convert >> ans;
	return ans;
}
string int_to_string(int num)
{
	string ans;
	stringstream convert ;
	convert << num;
	convert >> ans;
	return ans;
}

tree* parsing_graph(string graph_name,ifstream & infile)
{
    tree * ckt_graph;
    ckt_graph = new tree;
    
    int input_num;
    int gate_num;
    if(!infile.eof())
    {
        infile>>input_num;
        infile>>gate_num;
    }
    else
    {
    	return NULL;
    }
    
    for(int i=1;i<=input_num;i++) //add input gate
    {
    	string gateName = "P";
        gateName = gateName+int_to_string(i);
        vector<string> faninName;
        ckt_graph->addGate(gateName,0,faninName);
    }
    int no_gate;
    string line,gateLogic;
    //stringstream linePipe;
    //while (!infile.eof()) 

    for (int index=0;index <gate_num;index++)
    {
    	no_gate = index +1 ;
        string gateName = "G";
        gateName = gateName+int_to_string(no_gate);
        
//        getline(infile,line);
        
//        linePipe << line;
//        linePipe >> gateLogic;
		infile>>gateLogic;
        
        if (gateLogic=="INV")// for 1 input cells (INV)
        {
            string input;
            infile >> input;  // linePipe>>input is wrong but don't know why
            vector<string> faninName;
            faninName.push_back(input);
            ckt_graph->addGate(gateName,2,faninName); // only for inv
            
        }
        else// for 2 input cells (NAND2 and others)
        {
            string input1,input2;
            infile >> input1 >> input2;  //
            vector<string> faninName;
            faninName.push_back(input1);
            faninName.push_back(input2);
            ckt_graph->addGate(gateName,1,faninName); // only for nand2
            
        }
        
        
        //linePipe.str(std::string()); // clear stringstream
        //linePipe.clear();

    }
    
    cout<<"no_gate:"<<no_gate<<endl;
    cout<<"gate_num given:"<<gate_num<<endl;
	
	ckt_graph->name = graph_name;
	ckt_graph->adjust_gate_link();

	
	return ckt_graph;
}


vector<tree*>* parsing_cell(char* infileName)
{	
	ifstream infile;
	infile.open(infileName);//,ios::binary);
	
    vector<tree*>* celllib = new vector<tree*>;
    stringstream linePipe;
    string line;    

    string cellName;
    int celldelay;
    
    tree* t = new tree;
	int no_line = 0;
    while(getline(infile,line))
    {
    	no_line += 1;
	    linePipe.str("");
		linePipe.clear();
		line = line.substr(0,line.size()-1);
		
		if (no_line == 1)
		{
			cellName = line;
		}
		else if(no_line == 2)
    	{
	    	celldelay = string_to_int(line);
    	}
    	else
    	{
    		int no_gate = no_line-2;
    		if (line =="" && line.size() ==0)
			{
				t->celldelay = celldelay;
		    	t->name = cellName;
		    	t->adjust_gate_link();
		    	

		    	vector<tree*> expanded_trees = t->expand();

		    	for(unsigned i=0;i<expanded_trees.size();i++)
					celllib->push_back(expanded_trees[i]);
				t = new tree;
				//reset all parameters
				no_line = 0;
				//break;
			}
			else
			{
				string gateLogic;
				string gateName = "G";
		        gateName = gateName+int_to_string(no_gate);
				linePipe<<line;	
				linePipe>>gateLogic;
	    		
	    		if (gateLogic=="INV")
	    		{
	    			string input;
	    			linePipe >> input;
		            vector<string> faninName;
    		        faninName.push_back(input);
    		        t->addGate(gateName,2,faninName); // only for inv
	    		}
	    		else 
	    		{
	    			string input1,input2;
		            linePipe >> input1 >> input2;  
    		        vector<string> faninName;
    		        faninName.push_back(input1);
    		        faninName.push_back(input2);
        		    t->addGate(gateName,1,faninName); // only for nand2
	    		}
			}
    	}
    	
    	//cout<<"line="<<"\""<<line<<"\""<<line.size()<<endl;
		//cout<<"linesize="<<line.size()<<endl;
    }
    infile.close();
    return celllib;
}

int main(int argc ,char * argv[])
{
    if (argc<2)
    {
        cout<<"Missing argument."<<endl;
        return 0;
    }
    
    ifstream infile_graph;
    infile_graph.open(argv[1]);


	tree * treeptr = parsing_graph("main_graph",infile_graph);
    //cout<<*treeptr<<endl;
 	
 	vector<tree*>* celllib = parsing_cell(argv[2]);
 	
 	/*
 	for (vector<tree*>::iterator it = celllib->begin(); it != celllib->end() ;++it)
 	{
 		cout<<*(*it)<<endl;
 	}
 	*/
 	
 	for(vector<tree*>::iterator it = celllib->begin(); it != celllib->end() ;++it)
        treeptr->match(*it)
        
    treeptr->output(argv[3]);
 	
    delete treeptr;
    for (vector<tree*>::iterator it = celllib->begin(); it != celllib->end() ;++it)
 		delete *it;
 	delete celllib;
    
    infile_graph.close();

    return 0;

    
       
}
