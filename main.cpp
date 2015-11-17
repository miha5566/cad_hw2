#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


bool indentical_tree(tree)
{
}

tree* parsing_graph(iftream & infile)
{

    tree ckt_graph;
    
    int input_num;
    int gate_num,gate_num_count;
    if(!ifstream.eof())
    {
        ifstream>>input_num;
        ifstream>>gate_num;
    }
    
    for(int i=0;i<input_num;i++)
    {
    
        ckt_graph.add(,0,)
    }
    
    int no_gate=1;
    string line,word;
    stringstream linePipe;
    while (!infile.eof())
    {
        string gateName;
        getline(infile,line);
        linePipe << line;
        linePipe >> gateLogic;
        if (gateLogic=="INV")
        {
            string input;
            linePipe >> input;
        }
        else
        {
            string input1,input2;
            linePipe >> input1 >> input2;
            
        }
        
        
        
    }
    cout<<"no_gate:"<<no_gate<<endl;
    cout<<"gate_num given:"<<gate_num<<endl;
}

vector<tree*> parsing_cell(iftream & infile)
{
    vector<tree*> celllib;
    
}



int main(int argc ,char * []argv)
{
    if (argc<2):
        cout<<"Missing argument."<<endl;
        return 0;
    
    ifstream infile_graph;
    infile_graph.open(argv[1]);
    ifstream infile_cell;
    infile_cell.open(argv[2]);
    
    
    
       
}
