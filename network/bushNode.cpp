#include "bushNode.h"

bushNode::bushNode()
{
	nodeID=0;
	accendID=0;
	t=0.0;
	T=0.0;
	u=0.0;
	U=0.0;
	sgCost=0.0;
	lgCost=0.0;
	p=0;
	P=0;
	mark=1;
	aveToll=0.0;
	aveTime=0.0;
	nodeFlow=0.0;
	geCost=0.0;
	tFlow=0.0;
	//LUCE variables
	LW=0.0;
	LU=0.0;
	fi=0.0;
	Cd=0.0;
	Gd=0.0;
	ei=0.0;
}
bushNode::~bushNode()
{}