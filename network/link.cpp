#include "link.h"

//---construction function
Link::Link()
{
	status=-1;
	linkID=0;
	fromNode=0;
	toNode=0;
	gcost=0.0;
	vot=0.0;
	a=0.0;
	capacity=0.0;
	length=0.0;
	cost=0.0;
	speed=0;
	flow=0;
	u=0;
	cTime=0;
	dTime=0.0;
}
void Link::setCTime()
{
	cTime=(length/speed)*(1.0+0.15*pow((flow/capacity),4));
}
void Link::setDTime()
{
	dTime=0.6*(length/(speed*capacity))*pow((flow/capacity),3);
}
double Link::calDTime(double fl)
{
	return 0.6*(length/(speed*capacity))*pow((fl/capacity),3);
}
double Link::calTime(double fl)
{
	return (length/speed)*(1.0+0.15*pow((fl/capacity),4));
}


Link::~Link()
{

}