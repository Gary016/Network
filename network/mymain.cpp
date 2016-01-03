
#include "network.h"  //这里不要重复引用，如果network.h 里包含了link.h，这里就不要再引用了
#include "T2.h"
void A(bush * ss)
{
	cout<<"This is the output: "<<ss->linkFlow[1]<<endl;
	ss->linkFlow[2]=3;
}

int main()
{



	//T2 model
	T2 mynet;
	mynet.readNetwork("sf_t2_1.1",ios::in);
	mynet.readDemand("sf_t2_1.2",ios::in);
	mynet.T2_GFW(1e-6);
	//
	/*
	Network mynet;
	mynet.readNet("sf_t2_1.1",ios::in);
	mynet.readDemand("sf_t2_1.2",ios::in);
	mynet.mba4(10e-10);
	*/

	/*Network mynet;
	mynet.readNet("sf_t2_1.1",ios::in);
	mynet.readDemand("sf_t2_1.2",ios::in);
	mynet.mba4(10e-6);*/

	/*
	//test
	bush bb;
	bb.linkFlow.push_back(11);
	A(&bb);
	cout<<"This the return output: "<<bb.linkFlow[2]<<endl;
	*/

	
	//creat a network object
	//Network mynetwork;
	
	//read a network into the mynetwork object
	//mynetwork.readData("sf.1",ios::in);

	//read the demand data
	//mynetwork.readDemand("sf.2",ios::in);

	// use the Frank-Wolf algorithm
	//mynetwork.shortPath(1);
	//mynetwork.fw();
	
	//*/

	//------------------
	/*
	Network an;
	an.readData("sf.1",ios::in);

	an.readDemand("sf.2",ios::in);

	an.GP(10e-5);
	// an.showSPath();
	//*/
	
	//---------------dba algorithm
	/*
	Network dba;
	dba.readData("sf.1",ios::in);
	dba.readDemand("sf.2",ios::in);
	dba.dba(10e-12);
	//dba.fw();
	dba.writeLink("dba_sfLink.txt",ios::out);
	*/
	//

	//Algorithm LUCE
	//Network myn;
	/*myn.readData("cs.1",ios::in);
	myn.readDemand("cs.2",ios::in);*/
	//this sf.3 is used for the demand input of algorithm LUCE
	//myn.convertOD("cs.od",ios::out);
	//myn.convertNet("cs.nl",ios::out);
	//myn.readLuceNet("an.nl",ios::in);
	//myn.readLuceDemand("an.od",ios::in);
	//myn.LUCE(10e-12);
	//myn.writeLink("LUCE_anLINK.txt",ios::out);
	//myn.dba(10e-8);
	/*
	cout<<myn.nodesNum<<endl;
	for(int ni=1;ni<=myn.nodesNum;ni++)
	{
		cout<<myn.nodes[ni].beginLinkID<<setw(10)<<myn.nodes[ni].endLinkID<<endl;
	}
	system("PAUSE");
	for(int li=1;li<=myn.linksNum;li++)
	{
		cout<<myn.links[li].fromNode<<setw(10)<<myn.links[li].toNode<<setw(10)<<myn.links[li].capacity<<setw(10)<<myn.links[li].length<<setw(10)<<myn.links[li].speed<<endl;
	}
	*/
    return 0;
}