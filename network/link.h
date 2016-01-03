#pragma once
/*
Define a link class and have the following variables :
LinkID---The ID of this link;
fromID---The ID of node from which this link stems;
toID ----The ID of node where this link end
capacity--The capacity of this link;
length---The length of this link(mile);
speed----The free flow speed of this link; 
*/
#include<iostream>
using std::cout;
using std::endl;

class Link
{
public:
	int status;
	int linkID;
	int fromNode;
	int toNode;
	double vot;//value of time in this time 
	double gcost;//general cost in this a
	double cost;
	double capacity;
	double length;
	double speed;
	double a;
	double u;
	double flow;
	double cTime;
	double dTime;//define the derivative of the performance function result


	//construction function
	Link();
	~Link();
	void setCTime();
	void setDTime();
	double calDTime(double);
	double calTime(double);
};