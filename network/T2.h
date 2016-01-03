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
#include "dataStruct.h"

#include<vector>
using std::vector;
#include<fstream>
using std::ifstream;

#include<sstream>
using std::istringstream;
using std::stringstream;
#include<iomanip>
using std::setw;
using std::setprecision;
#include<string>
using std::string;

#include<cstdlib>
using std::exit;
//#include "gurobi_c++.h" //invoke the function of gurobi to solve the linear programming

#define inf 10000000000000000000

class T2
{
private:
	vector<Node> nodes;
	vector<Link> links;
	vector<OD> ods;
	vector<Demand> demands;
	

	vector<int> predNode;
	vector<int> predLink;
	vector<int> bl;
	double a_min;
	double a_max;
	deque<int> D;
	deque<int> H;

	int nodesNum;
	int linksNum;
	int odsNum;
	int demandsNum;
public:
	T2();

	//------read data function
	void readNetwork(string,long);
	void readDemand(string,long);
    void readDemand2(string,long,int);
	void readToll(string,long);
	//----set the float precision

	double setP(double);
	//---shrotest path algorithm
	void shortPath(int,double*,double*,double*);

	//-----calculate the topology order
	void topoSort(int*,int*);

	//------T2 model's functions

	void T2_RSD(double,double);

	void T2_MPA(int,double*,double*);
	void T2_MPA2(int,double*,double*);
	void T2_MPA3(int);
	string num2str(int i);

	void T2_LTA(int,double*,double*,double*,double*,double*,double*,double*,int*,int*,double);

	void T2_PTU(int,double*,double*,double*,double*,double*);
	void T2_MIN(int,double*,double*,double,double*,double*);
	void T2_RSD(double);
	void T2_ini(double*,double*);
	void T2_sub(double*,double*);
	void T2_sub2(double*,double*);
	void T2_sub3();
	//void T2_SMP(int,deque<double>::iterator,deque<double*>::iterator,deque<double*>::iterator);
	void T2_G(int,vector<double>::iterator,double*,double*,deque<double*>::iterator,deque<double*>::iterator);
	void T2_DG(int,deque<double*>::iterator,double*,double*,deque<double*>::iterator,deque<double*>::iterator);
	void T2_GFW(double error);
	void T2_Assign(int,double,double,double*);
	double T2_L(double,double*,double*,double*);
	double T2_BiLa(double,double*,double*);
	double calObjective();
	void T2_write(string,long);
	void writeDemand(string,long);
	void writeLink(string,long);
};
