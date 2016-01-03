#include<vector>
#include<list>
using std::list;
using std::vector;

class path
{
public:
	int beginNode;
	int endNode;
	double pathFlow;
	double pathTime;
	double pathDTime;
	vector<int> traceNode;
	vector<int> traceLink;
	path();
	~path();

};