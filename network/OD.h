#pragma once
// this class define the OD pairs in the form of O list
// define two variables, the O ID and the begin OD-pair ID
#include<vector>
using std::vector;
#include "bush.h"
class OD
{
public:
	int destiID;
	int orignID;
	int pairID;
	int endPID;
	vector<double> OBLflow;
	vector<bush> obushs;//bushs for each origin used in the multiclass bush based algorithm
	int bushNum;
	OD();
	~OD();
};