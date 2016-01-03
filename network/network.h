#include<iostream>
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::fixed;
using std::ios;
using std::left;
using std::right;
using std::showpoint;

#include "node.h"
#include "link.h"
#include "OD.h"
#include "Demand.h"
//#include "bush.h"

#include<vector>
using std::vector;

#include<fstream>
using std::ifstream;

#include<iomanip>
using std::setw;
using std::setprecision;

#include<string>
using std::string;

#include<cstdlib>
using std::exit;

#include<queue>
using std::deque;
#define inf 10e+15

class Network
{
public:
	vector<Node> nodes;
	vector<Link> links;
	vector<OD> ods;
	vector<Demand> demands;
	
	vector<int> pred;//pred node
	vector<int> predLink;//pred link
	vector<double> travelTime;	
	vector<double> y;
	vector<double> dir;
	vector<double> v; // this vector is used in the FWP methods to store the FW flow
	vector<double> pre_x;// this is the vector of x(n-2) used in the FWP methods
	vector<bush> bushs;
	double vot_max;
	double vot_min;
	vector<double> nodeCost;
	vector<double> nodeTime;
	double totalTime;
	int nodesNum;
	int linksNum;
	int odNum;
	int pairNum;
	//algorith 3
	vector<int> bl;
	deque<int> D;
	deque<int> H;


	//---construction function

	Network();
	Network(int nodenumber, int linknumber);

	//get 
	int getNodesNum();
	int getLinksNum();
	int getONum();
	int getPairNum();

	//set
	void setNodesNum(int);
	void setLinksNum(int);
	void setONum(int);
	void setPairNum(int);

	//

	double calCTime(double,double,double,double);
	double calFTime(double,double);
	//----Read function
	void readData(string ,long );
	void readDemand(string,long);
	//--- read funciton for the multiclass
	void readNet(string, long);
	//---- shortest path function
	void shortPath(int);
	void shortPathvot(int);
	//----assign all the demands on the link function
	void assignDemand();

	//----initialize the x0
	void initialize();
	//
	void inidba();

	//----calculate the derivate
	double calDerive(double,double*,Link*);

	//----calculate the derivation function of Partan methods
	double calPDerive(double,double*,double*);

	//-----calculate the total cost
	double calTotalCost(Link*);
	//-----calculate the objective function of UE
	double calObjective(Link*);

	//----calculate the absolute gap
	double calabGap(double*,Link*);
	//----give the larger value 
	double giveMAX(double,double);

	//----line search
	double biSearch(double*,Link*);

	//----Partan line search

	double plSearch(double ,double*,double*);

	//calculate the shortest and longest path
	void calTree(int,int);
	//
	void calTopo(int,int);
	// renew the u and U of bush after flow is assigned
	void renewBushTime(int,int);

	//renew the flow of the total network
	void renewFlow();

	//expand bush function
	int expandBush(int,int);

	//equilibrium the bush
	double eqBush(int,int,int);
	double eqBush(int,int);
	// reduce the bush
    void reduceBush(int,int);

	//calculate the UE traffic pattern
	void fw();
	// Gradient Projection Algorithm
	void GP(double error);
	void inigp();
	void updateGP();
	void updateGPi();
	void directF();
	void moveGP();
	double converge();
	void deletePath();
	double setP(double);
	//calculate the UE with FWP algorithm
	void fwp();




	//calculate the UE using the Dial bush based algorithm

	void dba(double);



	//----new bush based multiclass assignment
	void BMA(double,double,int);
	void inibma();
	void renewBush2(int,int);
	void expandBush2(int,int);
	int lastPO(int,int,int);
	double eqBush2(int ,int );
	//calculate the shortest tree and longest tree

	//display function
	void showSPath();
	//
	void writeLink(string,long);
	//


	//new algorithem two: multi bush for one origin
	void mba3(double);//this is the algorithm 3-----The main algorithm
	void mba4(double);//another method
	void ini4();
	void creatbush4(int);
	int  eqbush4(double);
	int recreatbush4();//all the origins and update the flow and expand for all the bushs
	int combineTree(bush*);

	//algorithm 5

	void mba5(double);

	void oba(int);//this is the one origin algorithm
	void shortpath(int, double*, double* ,double*);
	void min(int ,double*,double*,double*,double*);
	void ptu(int ,double*,double*,double*,double*,double*);
	void lta(int ,double*,double*,double*,double*,double*,double*,double*,int*,int*,double);
	void topoSort(int*,int*);
	void mpa(int, double*,double*);
	void creatBush(int,bush*,double ,double,double*,double* );
	void reBushTime3(bush*);
	int expandBush3(bush*);
	void calTopo3(bush*,int);
	void calTree3(bush*,int);
	double eqBush3(bush*, int,int,bush*);
	void reduceBush3(bush*,int);
	void ini3();

	//------------LUCE algorithm 

	void LUCE(double);
	void iniLuce();//creat the bushs for each destination and assign the ini flow to the shortest tree
	void convertOD(string,long);
	void convertNet(string,long);
	void readLuceNet(string,long);
	void readLuceDemand(string,long);
	void shortPathLuce(int b);
	void updatelinks();
	void stopCriterion(int ,int ,double*,double*,double*,double*);
	void reverseOrder(int );
	void updateBush1(int );
	void updateBush2(int );
	void updateBush3(int );

	void directionLuce(int );
	void luceBushShort(int );
	void makeBush(int);
	double calObjective(double*);
	~Network();

};