#include<vector>
using std::vector;
#include<deque>
using std::deque;
#include<list>
using std::list;

class bushNode
{
public:
	int nodeID;
	int accendID;
	double t;//shortest trave toll from origin
	double T;//longest travel toll from origin
	double u;//shortest travel time from origin
	double U;//longest travel time from origin
	double sgCost;
	double lgCost;
	int p;//shortest path arc in this node
	int P; // longest path arc in this node
	deque<int> clinks;//links which stem from the node
	deque<int> rlinks;//links which are into the node
	int mark;//to check if the node is deleted in the topo cal
	//for the multiclass assignment
	list<double> votDis;
	list<double> flowDis;
	double nodeFlow;//the total flow into this node
	double aveToll;//average  toll for this node
	double aveTime;//average time for this node
	double geCost;//genearl cost for the node : sum((time*vot+cost)*linkflow)/nodeFlow
	double tFlow;//total flow into this node
	bushNode();
	//----for LUCE variables
	double LW;
	double LU;
	double fi;
	double Cd;//the average cost 
	double Gd;//the average derivative 
	double ei;//the auxiliary node flow
	double Vi;//local equibrium cost 

	~bushNode();
};