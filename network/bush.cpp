#include "bush.h"

bush::bush()
{
	bushNode a;
	nodes.push_back(a);
	linkStatus.push_back(0);
	linkFlow.push_back(0.0);
	yluce.push_back(0.0);
	eluce.push_back(0.0);
	bushlink l;
	links.push_back(l);
	aveVot=0.0;
	a_lb=0.0;
	a_ub=0.0;
}
bush::~bush()
{};