#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "gate.h"
#include "tree.h"
using namespace std;

//而我聽見下雨的聲音
//想起你用唇語說愛情
//熱戀的時刻最任性
//不顧一切的給約定
//終於聽見下雨的聲音
//於是我的世界被吵醒
//發現你始終很靠近
//默默的陪在我身邊 態度堅定
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
    stringstream linePipe;
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
        //cout<<gateLogic<<" ";
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
        
        
        linePipe.str(std::string()); // clear stringstream
        linePipe.clear();

    }
    
    cout<<"no_gate:"<<no_gate<<endl;
    cout<<"gate_num given:"<<gate_num<<endl;

	ckt_graph->name = graph_name;
	ckt_graph->adjust_gate_link();

	return ckt_graph;
}



vector<tree*> parsing_cell(ifstream & infile)
{
    vector<tree*> celllib;
    //for 
    
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
    ifstream infile_cell;
    infile_cell.open(argv[2]);

	tree * treeptr = parsing_graph("main_graph",infile_graph);
    cout<<*treeptr<<endl;
    
    delete treeptr;
    return 0;

    
       
}
