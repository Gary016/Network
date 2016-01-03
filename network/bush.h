#include<vector>
#include "bushNode.h"
using std::vector;
#include"bushLink.h"

class bush
{
public:
	vector<bushNode> nodes;
	vector<int> linkStatus;
	vector<double> linkFlow;
	vector<int> topology;
	vector<bushlink> links;
	double aveVot;
	double a_ub;
	double a_lb;

	//LUCE variables
	vector<double> yluce;
	vector<double> eluce;
	bush();
	~bush();
};