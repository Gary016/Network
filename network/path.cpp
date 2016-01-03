#include"path.h"

 path::path()
{
	beginNode=0;
	endNode=0;
	traceNode.push_back(0);
	traceLink.push_back(0);
	pathFlow=0.0;
	pathTime=0.0;
	pathDTime=0.0;
}
 path::~path()
 {}