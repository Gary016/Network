#pragma once
// this class define the demand for each pair of OD
// there are four variables, pairID, orignID, destiID and demand;
#include "path.h"
class Demand
{
public:
	int pairID;
	int orignID;
	int destiID;
	double demand;
	vector<path> paths;
	int spLabel;
	Demand();
};