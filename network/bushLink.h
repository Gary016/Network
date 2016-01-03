#include<vector>
using std::vector;
#include<list>
using std::list;

class bushlink
{
public:
	list<double> votDis;
	list<double> flowDis;
	int classNum;
	double aveToll;//route toll 
	double aveTime;//route time
	double totalFlow;
	double geCost;//link ge cost
	double aveGU;//route ge cost 
	bushlink();
	~bushlink();
};