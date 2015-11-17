//gate.h

#ifndef _GATE_H_
#define _GATE_H_


class gate:
{
    friend class tree;
    private:
        int logic;
        /*
            0:input 
            1:nand 
            2:nor
            3:inv
            4:and
            5:or
        */
        vector<gate*> fanin;
        vector<gate*> fanout;
        string Name;
    public:
        gate();
        ~gate();
        void addFanin(vector<gate*> gates);
        void addFanin(gate* gate);
}



#endif
