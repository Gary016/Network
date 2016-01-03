#include "T2.h"
#include<fstream>
using std::ifstream;
using std::ofstream;
#include<string>
using std::string;
#include<iomanip>
using std::setw;
#include<queue>
using std::deque;
#include<list>
using std::list;
T2::T2()
{
	Node node00;
	Link link00;
	OD od000;
	Demand demand000;
	nodes.push_back(node00);
	links.push_back(link00);
	ods.push_back(od000);
	demands.push_back(demand000);
	bl.push_back(0);
	predNode.push_back(inf);
	predLink.push_back(inf);
	//----casual definition
	a_min=0.0;
	a_max=100.0;
}

//read network data
void T2::readNetwork(string file,long seekarg )
{
	ifstream inFile(file,seekarg); //put data into "inFile"

	if (!inFile)
	{
		cerr<<"File could not be opened"<<endl;
		exit(1);
	} //end if 
	//---read the number of nodes
	inFile>>nodesNum;
	cout<<"The number of nodes in the network is:"<<nodesNum<<endl;

	//---read the nodes attributes
	for(int i=1;i<nodesNum+1;i++)
	{
		int bID=0;
		int eID=0;
		Node no;
		inFile>>bID>>eID;
		no.beginLinkID=bID;
		no.endLinkID=eID;
		no.nodeID=i;
		nodes.push_back(no);
		//---initialize the pred vectors
		predNode.push_back(inf);
		predLink.push_back(inf);
	}
	//----read the links number
	inFile>>linksNum;
	cout<<"The number of links in the network is: "<<linksNum<<endl;

	//----read the links attributes
	for ( int i=1;i<linksNum+1;i++)
	{
		int fNode=0;
		int eNode=0;
		double capa=0;
		double len=0;
		double spe=0.0;
		double cost=0.0;
		Link li;
		inFile>>fNode>>eNode>>capa>>len>>spe>>cost;
		li.linkID=i;
		li.fromNode=fNode;
		li.toNode=eNode;
		li.capacity=capa;
		li.length=len;
		li.speed=spe;
		li.cost=cost;
		li.cTime=60*len/spe;
		links.push_back(li);
		bl.push_back(0);
	}
}
//--------read the demands data
void T2::readDemand(string file, long seekarg)
{
	ifstream inFile(file,seekarg); //put demand data into "inFile"

	if (!inFile)
	{
		cerr<<"File could not be opened"<<endl;
		exit(1);
	} //end if 

	//read the number of O
	inFile >> odsNum;
	cout<<" The Number of O is: "<<odsNum<<endl;
	//read the O into ods;
	for(int i=1; i<odsNum+1;i++)
	{
		int oid;
		int pid;
		OD od;
		inFile>>oid>>pid;
		od.orignID=oid;
		od.pairID=pid;
		ods.push_back(od);
	}
	//read the number of the pairs
	inFile>>demandsNum;
	cout<<"The number of OD pairs in the network is: "<<demandsNum<<endl;

	//read the demands into the vector demands
	for(int i=1;i<demandsNum+1;i++)
	{
		int destin;
		double dem;
		Demand d;
		inFile>>destin>>dem;
		d.pairID=i;
		d.destiID=destin;
		d.demand=dem;
		demands.push_back(d);
	}
	//---assign the index for the demands data
	for(int i=1;i<odsNum;i++)
	{
		ods[i].endPID=ods[i+1].pairID-1;
	}
	ods[odsNum].endPID=demandsNum;
	for(int di=1;di<=odsNum;di++)
	{
		for(int dj=ods[di].pairID;dj<=ods[di].endPID;dj++)
		{
			demands[dj].orignID=ods[di].orignID;
		}
	}
}
//-----------read the link toll 
//-----------this function can only read the specific format of toll data , see the dat "sf_tol.dat" in the network file of T2 algorithm
//-----------this function can only be used to update the link toll after the network was constructed
void T2::readToll(string file,long seekarg)
{
	ifstream inFile(file,seekarg);

	if(!inFile)
	{
		cerr<<"File could not be opend"<<endl;
		exit(1);
	}
	int totalLink;
	int lowBound;
	int upBound;
	inFile>>totalLink>>lowBound>>upBound;
	for(int i=1;i<=totalLink;i++)
	{
		int i_node;
		int j_node;
		double toll;
		inFile>>i_node>>j_node>>toll;
		for(int j=nodes[i_node].beginLinkID;j<=nodes[i_node].endLinkID;j++)
		{
			int lj=links[j].toNode;
			if(j_node==lj)
			{
				links[j].cost=toll;
			}
		}
	}

}

//-----------this is the function that reads demand data of Talex input
             //this function can only read the specific format of data, see the data "sf_trp_bak.dat" in the network file of T2 algorithm
            // and this function can only be used to update the demand data after the network was constructed.
void T2::readDemand2(string file,long seekarg,int totalZone)
{
	ifstream inFile(file,seekarg); //put data into "inFile"

	if (!inFile)
	{
		cerr<<"File could not be opened"<<endl;
		exit(1);
	} //end if 
	
	
	string line;
	string word;
	int zoneNum=totalZone;
	double totalTrip=0;
	int origin=0;
	while((origin<=zoneNum)&& !inFile.eof()){
		getline(inFile,line,'\n');
		//cout<<line<<endl;
		if(line.size()==20)
		{
			istringstream stream(line);
			stream>>word>>origin;
		}
		else
		{
			int number= line.size()/31;
			istringstream stream(line);
			for(int i=1;i<=5;i++)
			{
				int dest;
				double trip;
				char col;
				char semi;
				stream>>dest>>col>>trip>>semi;
				totalTrip+=trip;
				//cout<<dest<<setw(10)<<col<<setw(10)<<trip<<setw(10)<<semi<<endl;
				for(int j=ods[origin].pairID;j<=ods[origin].endPID;j++)
				{
					int d=demands[j].destiID;
					if(d==dest)
					{
						demands[j].demand=trip*0.001;
					}
				}
			}

		}
		
	}
}
//------This is a shortest path algorithm

void T2::shortPath(int orign,double* ptr_gup,double* ptr_Dnode,double* ptr_Snode)//gup is the general utility;dnode is the cost of node ; snode is 
{                                                                                // cost of node
	                                                                             //give the d_node and s_node for each link
	vector<double> d(nodesNum+1,inf);//general cost vector
	deque<int> Q;
	//initializa the variables
	for (int i=0;i<nodesNum+1;i++)
	{
		d[i]=inf;
        predNode[i]=inf;
		predLink[i]=inf;
		ptr_Dnode[i]=0.0;
		ptr_Snode[i]=0.0;
	}
	d[orign]=0.0;
	predNode[orign]=0;
	predLink[orign]=0;
	Q.push_back(orign);
	while(!Q.empty())
	{
		int searchNode=Q.front();
		Q.pop_front();
		for(int m=nodes[searchNode].beginLinkID;m<=nodes[searchNode].endLinkID;m++) // search every link which begin node is search node
		{
			int i=links[m].fromNode;// define the begin node of the link is i.
			int j=links[m].toNode;
			double t=ptr_gup[m];
			//denfine the end node of the link is j.
			if (d[j] > d[i] +t)//optimality condition: d(j)>d(i)+cij
			{
				int jexist=0;
				d[j] = d[i]+t;
				ptr_Dnode[j]=ptr_Dnode[i]+links[m].cost;
				ptr_Snode[j]=ptr_Snode[i]+links[m].cTime;
				predNode[j]=i; //record the previous node in the tree
				predLink[j]=m; // record the previous link in the tree
				//check if j has been in the Queue
				if(!Q.empty())
				{
				for(unsigned int n=0;n < Q.size();n++)
				{
					if (Q[n] == j) jexist=1;
				}
				}
				if(!jexist) 
				
				Q.push_back(j);
			}//end of checking optimality condition
		}// end of search link

	}//end of search algorithm
	
}

//This is the algrorithm T2-MIN
void T2::T2_MIN(int o, double* d_node,double* s_node, double a_up, double* ptr_alb,double* a)
{
	deque<int> a_list;
	vector<double> g_up(linksNum+1,0.0);
//----update the generalized cost for each link with respect to a_up
	for(int i=1;i<=linksNum;i++)
    {
		g_up[i]=links[i].cost+links[i].cTime*a_up;
	}
	//----shortest path algorithm
	shortPath(o,&g_up[0],d_node,s_node);

	//----set bl as 0
	for(int i=0;i<=linksNum;i++)
	{
		bl[i]=0;
	}
	//----check if link e is in the tree
	for(int i=1;i<=nodesNum;i++)
	{
		if(i==o)
		{
			continue;
		}
		int ln=predLink[i];
		bl[ln]=1;
	}
	//-----calculate a_e for each link which is not in the tree, give the a_list which contains the link index which has an a value
	for(int i=1;i<=linksNum;i++)
	{
		int inode=links[i].fromNode;
		int jnode=links[i].toNode;
		if(bl[i]==0)
		{
			a_list.push_back(i);
			if(s_node[inode]+links[i].cTime==s_node[jnode])
			{
				a[i]=a_min;
			}
			else
			{
				a[i]=setP((d_node[jnode]-d_node[inode]-links[i].cost)/(s_node[inode]+links[i].cTime-s_node[jnode]));
			}
		}
	}
	//-------calculate the low bound of the a: *ptr_alb
	//-----set the *ptr_alb to zero
	*ptr_alb=a_min;

	for(unsigned listi=0;listi<a_list.size();listi++)
	{
		if( (a[a_list[listi]]>=*ptr_alb) && ( a[a_list[listi]]<a_up))
		{
			*ptr_alb=a[a_list[listi]];
		}
	}
}

// This is the algorithm T2-PTU( o, d',s',a(ub) )
void T2::T2_PTU(int o,double* d_node,double* s_node,double* a_up,double* ptr_alb,double* a)
{
	deque<int> a_list;

	//----define the variables used in this algorithm


	//----first step: Initialize
	for(int i=1;i<=linksNum;i++)
	{
		if(bl[i]==0)
		{
			a_list.push_back(i);
		}
	}
	//-----set the D and H
	D.clear();
	H.clear();
	vector<int> dl(nodesNum+1,0);
	vector<int> hl(nodesNum+1,0);//set a label variables to indicate whether the node i is included in the H or D
	//set the H
	for(unsigned listi=0;listi<a_list.size();listi++)
	{
		if(a[a_list[listi]]==*ptr_alb)
		{
			int inode=links[a_list[listi]].fromNode;
			H.push_back(inode);
			hl[inode]=1;
		}
	}
	//-----begin the loop

	while(!H.empty())
	{
		//-----step 2 take the node
		int scanNode = H.front();
		H.pop_front();
		hl[scanNode]=0;
		if(dl[scanNode]==0)
		{
		    D.push_back(scanNode);
		    dl[scanNode]=1;
		}
		//-----step 3 Scan the node scanNode

		//---scan every link stem from the scan node
		for(int link_e=nodes[scanNode].beginLinkID;link_e<=nodes[scanNode].endLinkID;link_e++)
		{
			int jnode=links[link_e].toNode;
			double d_star=d_node[scanNode]+links[link_e].cost;
			double s_star=s_node[scanNode]+links[link_e].cTime;
			double g_star=d_star + *ptr_alb * s_star;//the *ptr_alb here is still is the low bound of the current tree
			double g_j=d_node[jnode]+ *ptr_alb * s_node[jnode];

			if( (g_star<g_j)||((g_star==g_j)&&(d_star<d_node[jnode])) || ((g_star==g_j) && ( d_star==d_node[jnode]) && (s_star<s_node[jnode])) || (( g_star==g_j) && ( predLink[jnode]==link_e) &&( (s_star!=s_node[jnode]) || (d_star!=d_node[jnode]))))
			{
				d_node[jnode]=d_star;
				s_node[jnode]=s_star;
				predLink[jnode]=link_e;
				predNode[jnode]=scanNode;
				if((jnode!=o)&&(hl[jnode]==0))
				{
				    H.push_back(jnode);
				}
			}
		}

	}

	//-----Final: update the a vector and  a_lowbound and a_upbound

	//----set bl as 0
	for(int i=0;i<=linksNum;i++)
	{
		bl[i]=0;
	}
	//----check if link e is in the tree
	for(int i=1;i<=nodesNum;i++)
	{
		if(i==o)
		{
			continue;
		}
		int ln=predLink[i];
		bl[ln]=1;
	}
	/*
	//test
	cout<<"The value of BL:"<<endl;
	for(int i=1;i<=linksNum;i++)
	{
		cout<<setw(10)<<bl[i];
	}
	system("PAUSE");
	*/
	//-----calculate a_e for each link which is not in the tree, give the a_list which contains the link index which has an a value
	for(int i=1;i<=linksNum;i++)
	{
		a[i]=0;
		int inode=links[i].fromNode;
		int jnode=links[i].toNode;
		if(bl[i]==0)
		{
			a_list.push_back(i);
			if(s_node[inode]+links[i].cTime==s_node[jnode])
			{
				a[i]=a_min;
			}
			else
			{
				a[i]=setP((d_node[jnode]-d_node[inode]-links[i].cost)/(s_node[inode]+links[i].cTime-s_node[jnode]));
			}
		}
	}
	//-------calculate the low bound of the a: *ptr_alb
	//-----set the *ptr_alb to zero
	*a_up=*ptr_alb;
	*ptr_alb=a_min;

	for( unsigned listi=0;listi<a_list.size();listi++)
	{
		if( (a[a_list[listi]]>=*ptr_alb) && ( a[a_list[listi]]<*a_up))
		{
			*ptr_alb=a[a_list[listi]];
		}
	}
}

//--------------Lazy traffic assignment algorithm

void T2::T2_LTA(int o,double* x_node, double* u_node,double* x_link,double* u_link,double* a_node,double* d_x,double* d_u, int* ppredNode,int* ppredLink,double a_lb)
{
	for(int i=1;i<=nodesNum;i++)
	{
		d_x[i]=0.0;
		d_u[i]=0.0;
	}
	//initialize the LTA
	if( a_lb==a_min)// set the D as the N and sort its topology
	{
		vector<int> topo(nodesNum+1,-1);
		topoSort(&topo[0],&predNode[0]);
		D.clear();
		for(int i=2;i<=nodesNum;i++)
		{
			D.push_back(topo[i]);//larger number is in the back of D
		}
	}
	else//set the topology of D
	{
		vector<int> to(nodesNum+1,-1);
		vector<unsigned> mark(D.size(),0);
		topoSort(&to[0],&ppredNode[0]);
		//give the element of D a number indicating the position
		for(unsigned i=0;i<D.size();i++)
		{
			for(int j=1;j<=nodesNum;j++)
			{
				if(D[i]==to[j])
				{
					mark[i]=j;
				}
			}
		}
		//sort the order of D
		for(unsigned i=0;i<D.size()-1;i++)
	    {
			for(unsigned j=0;j<D.size()-1-i;j++)
		{
			if(mark[j]>mark[j+1])
			{
				int t1=mark[j+1];
				int t2=D[j+1];
				mark[j+1]=mark[j];
				D[j+1]=D[j];
				mark[j]=t1;
				D[j]=t2;
			}
		}
	    }
		/*
		//print the D
		cout<<"print the D"<<endl;
		for(unsigned i=0;i<D.size();i++)
		{
			cout<<D[i]<<setw(10);
		}
		cout<<endl;
		//*/
	}
	//begin the loop


	while(!D.empty()) // when D is not empty then do the loop
	{
		//----                      step 3
		int j_node=D.back();
		//cout<<"j_node is : "<<j_node<<endl;
		D.pop_back();
		//----                      step 4
		double dx=0;
		double du=0;
		double dm=0;
		//-------find the demands for the o and j_node
		for(int i=ods[o].pairID;i<=ods[o].endPID;i++)
		{
			if(demands[i].destiID==j_node)
			{
				dm=demands[i].demand;
				break;
			}
		}
		//cout<<"The dm for : "<< o<<"--->"<<j_node<<" is "<< dm<<endl;
		//system("PAUSE");
		//----cal the dx and du
		// Uniform
		//dx=dm*0.01*(a_node[j_node]-a_lb);

		//du=dm*0.5*0.01*(a_node[j_node]*a_node[j_node]-a_lb*a_lb);
		//linear distribution
		dx=dm*0.0001*(a_node[j_node]*a_node[j_node]-a_lb*a_lb);
		du=dm*(1.00/15000.0)*(pow(a_node[j_node],3)-pow(a_lb,3));

		//cout<<"The dx and du are: "<<endl;
		//cout<<setw(10)<<dm<<setw(10)<<du<<endl;
		//system("PAUSE");
		d_x[j_node]=dx;
		d_u[j_node]=du;
		//---------------step 5
		int e=ppredLink[j_node];
		int i_node=ppredNode[j_node];
		x_link[e]=x_link[e]+x_node[j_node]+dx;
		u_link[e]=u_link[e]+u_node[j_node]+du;
		x_node[i_node]=x_node[i_node]+x_node[j_node]+dx;
		u_node[i_node]=u_node[i_node]+u_node[j_node]+du;
		x_node[j_node]=0.0;
		u_node[j_node]=0.0;
		//--------------step 6
		a_node[j_node]=a_lb;
	}
}

//------------cal topology sort
void T2::topoSort(int* order,int* predNode)
{
	vector<int> or(nodesNum+1,0);//set the node id
	vector<int> in(nodesNum+1,0);//set the order of node
	//----cal the topology
	for(int i=1;i<=nodesNum;i++)
	{
		in[i]=i;
		int mark=0;
		int pred=predNode[i];
		while(pred)
		{
		    mark+=1;
			pred=predNode[pred];
	    }
		or[i]=mark;
	}
	//----sort the order
	for(int i=1;i<nodesNum;i++)
	{
		for(int j=1;j<=nodesNum-i;j++)
		{
			if(or[j]>or[j+1])
			{
				int t1=or[j+1];
				int t2=in[j+1];
				or[j+1]=or[j];
				in[j+1]=in[j];
				or[j]=t1;
				in[j]=t2;
			}
		}
	}
	for(int i=1;i<=nodesNum;i++)
	{
		order[i]=in[i];
	}
}
//------------------T2_MPA algorithm

void T2::T2_MPA(int o,double* xl,double* ul)//xl and xu is return value for x and u on link
{
	double a_ub=a_max;
	double a_lb=-1;
	vector<double> d_node(nodesNum+1,0.0);
	vector<double> s_node(nodesNum+1,0.0);
	vector<double> x_node(nodesNum+1,0.0);//LTA variables
	vector<double> u_node(nodesNum+1,0.0);//LTA variables
	vector<double> x_link(linksNum+1,0.0);//LTA variables
	vector<double> u_link(linksNum+1,0.0);//LTA variables
	vector<double> a_node(nodesNum+1,a_max);//LTA variables
	vector<double> d_x(nodesNum+1,0.0);//LTA network node labels
	vector<double> d_u(nodesNum+1,0.0);//LTA network node labels
	vector<double> a(linksNum+1,-1.0);//PTU variables
	vector<int> order(linksNum+1,-1);//toplSort variables
	vector<int> ppredNode(nodesNum+1,0);//LTA variables
	vector<int> ppredLink(linksNum+1,0);//LTA variables
	double pa_ub;
	double pa_lb;//LTA variables
	//initialize the shortest tree
	T2_MIN(o,&d_node[0],&s_node[0],a_ub,&a_lb,&a[0]);
	/*
	//test
	cout<<"The a values are: "<<endl;
	for(int i=1;i<=linksNum;i++)
	{
		cout<<setw(10)<<a[i];
	}
	system("PAUSE");
	*/
	//----execute the PTU
	do
	{

		//record the last tree and the low bound and upbound
		for(int i=1;i<=nodesNum;i++)
		{
			ppredNode[i]=predNode[i];
			ppredLink[i]=predLink[i];
		}
		pa_ub=a_ub;
		pa_lb=a_lb;
		//----step1 : calculate the shortest paht according the a_ub
		
		T2_PTU(o,&d_node[0],&s_node[0],&a_ub,&a_lb,&a[0]);
		
		/*
		cout<<"The a values are: "<<endl;
	for(int i=1;i<=linksNum;i++)
	{
		cout<<setw(10)<<a[i];
	}
	system("PAUSE");
		*/
		//-----step2: assign the demands

		
		T2_LTA(o,&x_node[0],&u_node[0],&x_link[0],&u_link[0],&a_node[0],&d_x[0],&d_u[0],&ppredNode[0],&ppredLink[0],pa_lb);
		
		/*
		cout<<"This tree is for a :"<<pa_lb<<"---"<<pa_ub<<endl;
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<ppredNode[i]<<"<----"<<i<<setw(10);
		}
		cout<<endl;
		
		// print the node labels
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<"Node"<<i<<"is:"<<x_node[i]<<setw(10)<<u_node[i]<<setw(10)<<"<---"<<a_node[i]<<setw(10)<<endl;
		}
		cout<<endl;
		//print the arc labels
		for(int i=1;i<=linksNum;i++)
		{
			cout<<links[i].fromNode<<"--->"<<links[i].toNode<<":  "<<x_link[i]<<setw(10)<<u_link[i]<<setw(10)<<endl;
		}
		cout<<endl;
		
		system("PAUSE");
		//*/

	}
	while(a_lb!=a_min);

	//assign the last tree
	T2_LTA(o,&x_node[0],&u_node[0],&x_link[0],&u_link[0],&a_node[0],&d_x[0],&d_u[0],&predNode[0],&predLink[0],a_lb);
	/*
	cout<<"This tree is for a :"<<a_lb<<"---"<<a_ub<<endl;
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<predNode[i]<<"<----"<<i<<setw(10);
		}
		cout<<endl;
		
		// print the node labels
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<"Node"<<i<<"is:"<<x_node[i]<<setw(10)<<u_node[i]<<setw(10)<<"<---"<<a_node[i]<<setw(10)<<endl;
		}
		cout<<endl;
		//print the arc labels
		for(int i=1;i<=linksNum;i++)
		{
			cout<<links[i].fromNode<<"--->"<<links[i].toNode<<":  "<<x_link[i]<<setw(10)<<u_link[i]<<setw(10)<<endl;
		}
		cout<<endl;
		system("PAUSE");
		//*/
	    //set return value
		for(int i=1;i<=linksNum;i++)
		{
			xl[i]=x_link[i];
			ul[i]=u_link[i];
		}
	
}

//

string T2::num2str(int i)
{
        stringstream ss;
        ss<<i;
        return ss.str();
}
//
void T2::T2_MPA2(int o,double* xl,double* ul)//xl and xu is return value for x and u on link
{
	string file="w_sf_demands_"+num2str(o)+".txt";
	ofstream outFile(file,ios::out);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	outFile<<setw(10)<<"Origin"<<setw(10)<<o<<"\n";

	//define a struct
	struct multi
	{
		vector<double> bound;
		vector<double> trip;
	};
	//define the inner struct
	vector<double> alinkflow(linksNum+1,0.0);
	struct inner
	{
		vector<double> alinkFlow;
	};
	//define the outer struct
	struct outer
	{
		vector<inner> desti;
	};
	vector<outer> origins(25);
	int dNum=ods[o].endPID-ods[o].pairID+1;
	vector<multi> multis(25);
	vector<int> cNum(nodesNum+1,0);
	double a_ub=a_max;
	double a_lb=-1;
	vector<double> d_node(nodesNum+1,0.0);
	vector<double> s_node(nodesNum+1,0.0);
	vector<double> x_node(nodesNum+1,0.0);//LTA variables
	vector<double> u_node(nodesNum+1,0.0);//LTA variables
	vector<double> x_link(linksNum+1,0.0);//LTA variables
	vector<double> u_link(linksNum+1,0.0);//LTA variables
	vector<double> a_node(nodesNum+1,a_max);//LTA variables
	vector<double> d_x(nodesNum+1,0.0);//LTA network node labels
	vector<double> d_u(nodesNum+1,0.0);//LTA network node labels
	vector<double> a(linksNum+1,-1.0);//PTU variables
	vector<int> order(linksNum+1,-1);//toplSort variables
	vector<int> ppredNode(nodesNum+1,0);//LTA variables
	vector<int> ppredLink(linksNum+1,0);//LTA variables
	vector<double> prea_node(nodesNum+1,a_max);
	double pa_ub;
	double pa_lb;//LTA variables
	int classNum=0;
	//initialize the shortest tree
	T2_MIN(o,&d_node[0],&s_node[0],a_ub,&a_lb,&a[0]);
	//----execute the PTU
	do
	{
		classNum+=1;
		//record the last tree and the low bound and upbound
		for(int i=1;i<=nodesNum;i++)
		{
			ppredNode[i]=predNode[i];
			ppredLink[i]=predLink[i];
		}
		pa_ub=a_ub;
		pa_lb=a_lb;
		//----step1 : calculate the shortest paht according the a_ub
		
		T2_PTU(o,&d_node[0],&s_node[0],&a_ub,&a_lb,&a[0]);
		
		
		//-----step2: assign the demands

		
		T2_LTA(o,&x_node[0],&u_node[0],&x_link[0],&u_link[0],&a_node[0],&d_x[0],&d_u[0],&ppredNode[0],&ppredLink[0],pa_lb);



		outFile<<"Class "<<classNum<<"'s a bounds are:"<<pa_lb<<"------>"<<pa_ub<<"\n";
		outFile<<setw(10)<<"Destination "<<setw(10)<<" Low Bound "<<setw(10)<<" Up Bound "<<setw(10)<<"Demands of this class"<<"\n";
		for(int i=1;i<=nodesNum;i++)
		{
			outFile<<setw(10)<<i<<setw(10)<<a_node[i]<<setw(10)<<prea_node[i]<<setw(10)<<d_x[i]<<"\n";
			if(d_x[i]>0)
			{
				multis[i].trip.push_back(d_x[i]);
				multis[i].bound.push_back(a_node[i]);
				multis[i].bound.push_back(prea_node[i]);
			}
			if((d_x[i]!=0)&&(a_node[i]!=prea_node[i]))
			{
				cNum[i]+=1;
			}
			prea_node[i]=a_node[i];
		}
		/*
		cout<<"This tree is for a :"<<pa_lb<<"---"<<pa_ub<<endl;
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<ppredNode[i]<<"<----"<<i<<setw(10);
		}
		cout<<endl;
		
		// print the node labels
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<"Node"<<i<<"is:"<<x_node[i]<<setw(10)<<u_node[i]<<setw(10)<<"<---"<<a_node[i]<<setw(10)<<endl;
		}
		cout<<endl;
		//print the arc labels
		for(int i=1;i<=linksNum;i++)
		{
			cout<<links[i].fromNode<<"--->"<<links[i].toNode<<":  "<<x_link[i]<<setw(10)<<u_link[i]<<setw(10)<<endl;
		}
		cout<<endl;
		
		system("PAUSE");
		//*/

	}
	while(a_lb!=a_min);

	//assign the last tree
	T2_LTA(o,&x_node[0],&u_node[0],&x_link[0],&u_link[0],&a_node[0],&d_x[0],&d_u[0],&predNode[0],&predLink[0],a_lb);
	
	    outFile<<"Class "<<classNum<<"'s a bounds are:"<<a_lb<<"------>"<<a_ub<<"\n";
		outFile<<setw(10)<<"Destination"<<setw(10)<<" Low Bound "<<setw(10)<<" Up Bound "<<setw(10)<<"Demands of this class"<<"\n";
		for(int i=1;i<=nodesNum;i++)
		{
			outFile<<setw(10)<<i<<setw(10)<<a_node[i]<<setw(10)<<prea_node[i]<<setw(10)<<d_x[i]<<"\n";
			if(d_x[i]>0)
			{
				multis[i].trip.push_back(d_x[i]);
				multis[i].bound.push_back(a_node[i]);
				multis[i].bound.push_back(prea_node[i]);
			}
			if((d_x[i]!=0)&&(a_node[i]!=prea_node[i]))
			{
				cNum[i]+=1;
			}
		}
		// summary of this O
		outFile<<"\n";
		outFile<<"--------------Summary for Origin "<<o<<" --------------------- \n";
		outFile<<"\n";
		outFile<<setw(10)<<"Destination"<<setw(20)<<"Class Number"<<setw(30)<<"Range of calss: Trips "<<"\n";
		for(int i=1;i<=nodesNum;i++)
		{
			outFile<<setw(10)<<i<<setw(20)<<cNum[i]<<setw(20);
			for(int j=1;j<=cNum[i];j++)
			{
				double bBound=multis[i].bound.back();
				multis[i].bound.pop_back();
				double aBound=multis[i].bound.back();
				multis[i].bound.pop_back();
				double eTrip=multis[i].trip.back();
				multis[i].trip.pop_back();
				outFile<<aBound<<"-->"<<bBound<<" : "<<eTrip<<setw(20);
			}
			outFile<<"\n";
		}
		outFile.close();
	/*
	cout<<"This tree is for a :"<<a_lb<<"---"<<a_ub<<endl;
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<predNode[i]<<"<----"<<i<<setw(10);
		}
		cout<<endl;
		
		// print the node labels
		for(int i=1;i<=nodesNum;i++)
		{
			cout<<"Node"<<i<<"is:"<<x_node[i]<<setw(10)<<u_node[i]<<setw(10)<<"<---"<<a_node[i]<<setw(10)<<endl;
		}
		cout<<endl;
		//print the arc labels
		for(int i=1;i<=linksNum;i++)
		{
			cout<<links[i].fromNode<<"--->"<<links[i].toNode<<":  "<<x_link[i]<<setw(10)<<u_link[i]<<setw(10)<<endl;
		}
		cout<<endl;
		system("PAUSE");
		//*/
	    //set return value
		for(int i=1;i<=linksNum;i++)
		{
			xl[i]=x_link[i];
			ul[i]=u_link[i];
		}
	
}

//
void T2::T2_MPA3(int o)//xl and xu is return value for x and u on link
{
	//string file="w_sf_Origin_Link"+num2str(o)+".txt";
	string file="w_sf_Origin_Link.txt";
	/*
	struct sData
	{
		vector<double> linksFlow;
	};
	vector<sData> sDatas;
	sData data11;
	for(int di=0;di<=linksNum;di++)
	{
		data11.linksFlow.push_back(0.0);
	}
	sDatas.push_back(data11);
	*/
	//define a data struct to keep the link flow for different classes
	dataStruct data00;
	vector<dataStruct> datas;
	for(int li=0;li<=linksNum;li++)
	{
		data00.linksFlow.push_back(0.0);
	}
	datas.push_back(data00);
	int classNum=0;
	vector<double> aBound;
	double a_ub=a_max;
	double a_lb=0;

	vector<double> d_node(nodesNum+1,0.0);
	vector<double> s_node(nodesNum+1,0.0);
	vector<double> x_node(nodesNum+1,0.0);//LTA variables
	vector<double> u_node(nodesNum+1,0.0);//LTA variables
	vector<double> x_link(linksNum+1,0.0);//LTA variables
	vector<double> u_link(linksNum+1,0.0);//LTA variables
	vector<double> a_node(nodesNum+1,a_max);//LTA variables
	vector<double> d_x(nodesNum+1,0.0);//LTA network node labels
	vector<double> d_u(nodesNum+1,0.0);//LTA network node labels
	vector<double> a(linksNum+1,-1.0);//PTU variables
	vector<int> order(linksNum+1,-1);//toplSort variables
	vector<int> ppredNode(nodesNum+1,0);//LTA variables
	vector<int> ppredLink(linksNum+1,0);//LTA variables
	vector<double> prea_node(nodesNum+1,a_max);

	double *pLink = new double[linksNum+1];
	//initialize the shortest tree
	T2_MIN(o,&d_node[0],&s_node[0],a_ub,&a_lb,&a[0]);

	//assign the demand of this class
	T2_Assign(o,a_lb,a_ub,pLink);
	//generate a new class column fro this bounds
	datas.push_back(data00);
	//assign the link volumn into this vector
	for(int li=1;li<=linksNum;li++)
	{
		datas[1].linksFlow[li]=pLink[li];
	}

	delete[] pLink;
	//class number +1
	classNum+=1;
	aBound.push_back(a_ub);
	aBound.push_back (a_lb);
	

	//----execute the Cycle
	while(a_lb!=a_min)
	{
		//----step1 : calculate the shortest paht according the a_ub
		
		T2_PTU(o,&d_node[0],&s_node[0],&a_ub,&a_lb,&a[0]);
		
		//-----step2: assign the demands
		double *sLink = new double[linksNum+1];
		
		T2_Assign(o,a_lb,a_ub,sLink);
		//
		
		datas.push_back(data00);
		//
		classNum+=1;
		aBound.push_back(a_lb);

		for(int li=1;li<=linksNum+1;li++)
		{
			datas[classNum].linksFlow[li]=sLink[li];
		}

		delete[] sLink;
		

	}
	//------outPut

	
	///*
	//outFile<<setw(10)<<odsNum<<"\n";
	ofstream outFile1;
	outFile1.open(file,ios::out|ios::app);
	if(!outFile1)
	{
		cerr<<"error: unable to write the file: "<<outFile1<<endl;
	}
	//write the Origin and Class Number
	outFile1<<setw(10)<<o<<setw(15)<<classNum<<"\n";
	//write the a bounds for all the classes
	for(int ai=0;ai<=classNum;ai++)
	{
		outFile1<<setw(10)<<aBound[ai]<<setw(15);
	}
	outFile1<<"\n";
	//write the link flow for each class
	//column is link index, row is class index
	for(int li=1;li<=linksNum;li++)
	{
		outFile1<<setw(10)<<li<<setw(20);
		for(int ci=1;ci<=classNum;ci++)
		{
			outFile1<<datas[ci].linksFlow[li]<<setw(15);
		}
		outFile1<<"\n";
	}
	//outFile1<<"\n";
    outFile1.close();
	//*/

	
}
//-------T2_Assign

void T2::T2_Assign(int o,double a_lb,double a_ub,double *pLink)
{
	for(int li=0;li<=linksNum;li++)
	{
		pLink[li]=0.0;
	}
	//search every od steming from thi origin
	for(int di=ods[o].pairID;di<=ods[o].endPID;di++)
	{
		int destiNode=demands[di].destiID;
		double dem=demands[di].demand;
		//uniform
		//double assignTrip=dem*0.01*(a_ub-a_lb);
		//linear distribution
		double assignTrip=dem*0.0001*(a_ub*a_ub-a_lb*a_lb);
		while(predLink[destiNode])
		{
			int sLink=predLink[destiNode];
			int sNode=predNode[destiNode];
			pLink[sLink]+=assignTrip;
			destiNode=sNode;
		}
		
	}

}
//-------------
void T2::T2_sub3()
{
	ofstream outFile1;
	outFile1.open("w_sf_Origin_Link.txt",ios::out);
	if(!outFile1)
	{
		cerr<<"error: unable to write the file: "<<outFile1<<endl;
	}
	outFile1<<setw(10)<<odsNum<<setw(15)<<linksNum<<"\n";
	outFile1.close();
	for(int oi1=1;oi1<=odsNum;oi1++)
	{
		int o=ods[oi1].orignID;
		T2_MPA3(o);
		
	}
}
//--------T2_GFW
void T2::T2_GFW(double error)
{
	//------step one: initialization
	int iter=0;
	vector<double> new_x(linksNum+1,0.0);
	vector<double> new_u(linksNum+1,0.0);
	T2_ini(&new_x[0],&new_u[0]);
	//cout<<"ini"<<endl;
	//system("PAUSE");
	//update the links
	for(int li=1;li<=linksNum;li++)
	{
		links[li].flow=new_x[li];
		links[li].u=new_u[li];
		links[li].setCTime();
	}
	while(iter<10000)
	{
		//------decent dirction
		vector<double> dx(linksNum+1,0.0);
		vector<double> du(linksNum+1,0.0);
		T2_sub(&new_x[0],&new_u[0]);
		//cout<<"solve the sub problem"<<endl;
		//system("PAUSE");
		for(int li=1;li<=linksNum;li++)
		{
			dx[li]=new_x[li]-links[li].flow;
			du[li]=new_u[li]-links[li].u;
		}

		//------termination test
		double lup=T2_L(0,&dx[0],&du[0],&dx[0]);
		cout<<"lup is: "<<lup<<endl;
		double ldo=T2_L(0,&new_x[0],&new_u[0],&dx[0]);
		double gap=abs(lup/ldo);
		cout<<"lup is: "<<ldo<<endl;
		cout<<"gap is: "<<gap<<endl;
		if(gap<error)
		{
			break;
		}
		
		//------step Size
		double lambda=0;
		double dt1=T2_L(1,&dx[0],&du[0],&dx[0]);
		if(dt1<=0)
		{
			lambda=1;
		}
		else
		{
			lambda=T2_BiLa(0.001,&dx[0],&du[0]);
		}
		//cout<<"step size"<<endl;
		//system("PAUSE");

		//-------update the links
		for(int li=1;li<=linksNum;li++)
		{
			links[li].flow=links[li].flow+lambda*dx[li];
			links[li].u=links[li].u+lambda*du[li];
			links[li].setCTime();
		}
		//
		iter+=1;
		cout<<"This is the main iteration: ";
		cout<<setw(10)<<iter<<endl;
		
		//cout<<"The links flow are; "<<endl;
		//for(int li=1;li<=linksNum;li++)
		//{
		//	cout<<links[li].fromNode<<"--->"<<links[li].toNode<<setw(10)<<links[li].flow<<endl;
		//}
		
		cout<<"The T2 User Equilibrium is : ";
	   cout<<setw(10)<<calObjective()<<endl;
		cout<<"The main gap is : "<<gap<<endl;
		//system("PAUSE");
	}
	cout<<"The T2 User Equilibrium is found: ";
	cout<<setw(10)<<calObjective()<<endl;

	
	//record the different class users
	T2_sub2(&new_x[0],&new_u[0]);
	//------
	cout<<"The total number of OD is :"<<odsNum<<endl;
	T2_sub3();
	cout<<"This is the end"<<endl;
	system("PAUSE");
	

}
//
double T2::calObjective()
{
	double objective=0.0;
	for(int i=1;i<=linksNum;i++)
	{
		double x=links[i].flow;
		double t0=links[i].calTime(0.0);
		double ta=t0*links[i].flow+((0.03*t0)/(pow(links[i].capacity,4)))*pow(links[i].flow,5);
		objective+=ta;
	}
	return objective;
}
//------------cal the lambda using binary method
double T2::T2_BiLa(double empson,double * dx, double * du)
{
	int biter=0;
	double lamb=1;
	double up=1.0;
	double lo=0.0;
	while(biter<100)
	{
		double Li=T2_L(lamb,dx,du,dx);
		if (abs(Li-0)<empson)
		{
			break;
		}
		else if(Li>0)
		{
			up=lamb;
			lamb=0.5*(up+lo);
		}
		else
		{
			lo=lamb;
			lamb=0.5*(up+lo);
		}
		biter+=1;
	}
	return lamb;
}
//------------cal the L for GFW
double T2::T2_L(double lambda,double * x,double * u,double * dx)
{
	double L=0.0;
	vector<double> nx(linksNum+1,0.0);
	vector<double> nu(linksNum+1,0.0);
	for(int li=1;li<=linksNum;li++)
	{
		double fl=links[li].flow+lambda*dx[li];
		L+=(links[li].cost*x[li]+links[li].calTime(fl)*u[li]);
	}
	return L;
}
// -----------T2_TSD algorithm
void T2::T2_RSD(double error)
{
	//initialize the function
	int n=1;
	double reGap=1;
	deque<double*> X;
	deque<double*> U;
	deque<double> lambda;
	lambda.push_back(1);
	double* new_x=new double[linksNum+1]();
	double* new_u=new double[linksNum+1]();
	T2_ini(new_x,new_u);
	X.push_back(new_x);//ini the X
	U.push_back(new_u);//ini the U
	//update the links
	for(int li=1;li<=linksNum;li++)
	{
		links[li].flow=new_x[li];
		links[li].u=new_u[li];
		links[li].setCTime();
	}
	double Total=0;
	//begin the loop
	while(n<500)
	{
		//-----second step: solve subproblem
		double* nx=new double[linksNum+1]();
		double* nu=new double[linksNum+1]();
		T2_sub(nx,nu);
		X.push_back(nx);
		U.push_back(nu);
		//set the initialized lambda for master problem
		lambda.push_back(0);
		int num=lambda.size();
		for(int lmi=0;lmi<num;lmi++)
		{
			lambda[lmi]=lambda[lmi]*0.8;
		}
		lambda[num-1]=0.2;
		

		//------Third step: cal the convergent criteria
		double uValue=0;
		double dValue=0;
		for(int li=1;li<=linksNum;li++)
		{
			uValue+=(links[li].cost*(nx[li]-links[li].flow)+links[li].cTime*(nu[li]-links[li].u));
			dValue+=(links[li].cost*links[li].flow+links[li].cTime*links[li].u);
		}
		reGap=uValue/dValue;
		Total=dValue;
		//test
		cout<<"The reGap is: "<<reGap<<endl;
		system("PAUSE");
		if(abs(reGap)<error)
		{
			break;
		}
		//-----fourth step: solve master problem
		double lam=0.01;
		deque<double*>::iterator xbegin=X.begin();
		deque<double*>::iterator ubegin=U.begin();
		deque<double>::iterator lamBeg=lambda.begin();
		//T2_SMP(num,lamBeg,xbegin,ubegin);
		cout<<"After SMP the lambda are: "<<endl;
		for(int di=0;di<num;di++)
		{
			cout<<lambda[di]<<endl;
		}
		system("PAUSE");
		
			

		//---update the x and u in the links and the time and the cost
		for(int li=1;li<=linksNum;li++)
		{
			links[li].flow=0;
			links[li].u=0;
			for(int mi=0;mi<num;mi++)
			{
				double * ptr_x=X[mi];
				double * ptr_u=U[mi];
				links[li].flow+=ptr_x[li]*lambda[mi];
				links[li].u+=ptr_u[li]*lambda[mi];
			}
			links[li].setCTime();
		}

		//------Five step: manage the memor

		double * s_x=X.front();
		double * s_u=U.front();
		X.pop_front();
		U.pop_front();
		delete[] s_x;
		delete[] s_u;
		for(int li=1;li<=linksNum;li++)
		{
			X[0][li]=links[li].flow;
			U[0][li]=links[li].u;
		}
		lambda.clear();
		lambda.push_back(1);
		// update the n
		n+=1;
		cout<<"The iteration number is : "<<n<<endl;
		system("PAUSE");
		
	}
	// delete the X and U
	int dnum=X.size();
	for(int di=0;di<dnum;di++)
	{
		double * ptr_dx=X.front();
		double * ptr_du=U.front();
		X.pop_front();
		U.pop_front();
		delete[] ptr_dx;
		delete[] ptr_du;
	}
	//print the result
	cout<<"The Total iteration is : "<<n<<endl;
	cout<<"The UE for multiclass is found: "<<Total<<endl;
	

}
//----------solve master problem
/*
void T2::T2_SMP(int num,deque<double>::iterator lbeg,deque<double*>::iterator xbeg,deque<double*>::iterator ubeg)
{
	//cal the XLambda
	vector<double> cx(linksNum+1);
	vector<double> cu(linksNum+1);
	for(int li=1;li<=linksNum;li++)
	{
		cx[li]=0;
		cu[li]=0;
	}
	for(int i=0;i<num;i++)
	{
		double lami=*(lbeg+i);
		double * ptr_x=*(xbeg+i);
		double * ptr_u=*(ubeg+i);
		cout<<"the links flow for X "<<i<<"are:"<<endl;
		for(int li=1;li<=linksNum;li++)
		{
			cout<<setw(20)<<ptr_x[li];
		}
		cout<<endl<<"the lami is : "<<lami<<endl;
		//system("PAUSE");
		for(int li=1;li<=linksNum;li++)
		{
			cx[li]+=ptr_x[li]*lami;
			cu[li]+=ptr_u[li]*lami;
		}
	}

    vector<double> G(num,0);

	double minG;
	vector<double>::iterator gbeg=G.begin();
	T2_G(num,gbeg,&cx[0],&cu[0],xbeg,ubeg);
	minG=*(gbeg);
	for(int gi=0;gi<num;gi++)
	{
		cout<<setw(10)<<*(gbeg+gi);
		if(minG>=*(gbeg+gi))
		{
			minG=*(gbeg+gi);
		}
	}
	cout<<"The minG is: "<<minG<<endl;
	//system("PAUSE");
	while(minG<-5)
	{
		
		//----------call the gurobi to cal the lambda
		
		//creat the environment
		GRBEnv env = GRBEnv();

		//creat the model
		GRBModel model=GRBModel(env);

		//creat the variables
		vector<GRBVar> var_lam(num);
		vector<GRBVar> var_w(num);
		for(int vi=0;vi<num;vi++)
		{
			var_lam[vi]=model.addVar(0.0,1.0,1,GRB_CONTINUOUS);
			var_w[vi]=model.addVar(0.0,1000000,1,GRB_CONTINUOUS);
		}

		//update the model
		model.update();

		//----add constraints
		//cal the DG
		deque<double*> DG(num);
		for(int mi=0;mi<num;mi++)
		{
			double * ddg= new double[num];
			DG[mi]=ddg;
		}
		for(int mi=0;mi<num;mi++)
		{
			for(int ni=0;ni<num;ni++)
			{
				DG[mi][ni]=0;
				cout<<setw(10)<<DG[mi][ni];
			}
			cout<<endl;
		}
		deque<double*>::iterator dgbeg=DG.begin();
		T2_DG(num,dgbeg,&cx[0],&cu[0],xbeg,ubeg);
		cout<<"end DG"<<endl;
		for(int di=0;di<num;di++)
		{
			double * ptr_mDG=DG[di];
			for(int dj=0;dj<num;dj++)
			{
				cout<<setw(10)<<ptr_mDG[dj];
			}
		}
		//system("PAUSE");
		//
		GRBLinExpr lhs;//define the left formulation
		for(int ci=0;ci<num;ci++)
		{
			double rhs;
			double * DGM=DG[ci];
			lhs=0;
			rhs=0;
			for(int cj=0;cj<num;cj++)
			{
				lhs=lhs+DGM[cj]*var_lam[cj];
				rhs=rhs+DGM[cj]*(*(lbeg+cj));
			}

			lhs=lhs+var_w[ci];
			rhs=rhs-G[ci];
			model.addConstr(lhs+var_w[ci]>=rhs-G[ci]);
		}
		//add another constr
		lhs=0;
		for(int ci=0;ci<num;ci++)
		{
			lhs=lhs+var_lam[ci];
		}

		model.addConstr(lhs==1);

		//optimize the model
		model.optimize();
		cout<<"The lambda are:"<<endl;
		for(int lli=0;lli<num;lli++)
		{
			cout<<setw(10)<<var_lam[lli].get(GRB_DoubleAttr_X);
		}
		//system("PAUSE");
		//--------update the solution
		
		for(int vi=0;vi<num;vi++)
		{
			*(lbeg+vi)=var_lam[vi].get(GRB_DoubleAttr_X);
		}
		
		//delete the DG
		for(int mi=0;mi<num;mi++)
		{
			double * dld=DG.front();
			DG.pop_front();
			delete[] dld;
		}
		//update the cx and cu
		for(int li=1;li<=linksNum;li++)
		{
			cx[li]=0;
			cu[li]=0;
		}
		for(int i=0;i<num;i++)
	    {
		    double * ptr_x=*(xbeg+i);
		    double * ptr_u=*(ubeg+i);
		    double lami=*(lbeg+i);
			cout<<"lami is: "<<lami<<endl;
			//system("PAUSE");
		    for(int j=1;j<=linksNum;j++)
		    {
			    cx[j]+=ptr_x[j]*lami;
			    cu[j]+=ptr_u[j]*lami;
		    }
	    }
		//update the G to 
		//clear T2_G
		for(int gi=0;gi<num;gi++)
		{
			G[gi]=0;
		}
		gbeg=G.begin();
		T2_G(num,gbeg,&cx[0],&cu[0],xbeg,ubeg);
		minG=G[0];
	    for(int gi=0;gi<num;gi++)
	    {
		    if(minG>=G[gi])
		    {
			    minG=G[gi];
		    }
	    }
		cout<<"The G are:"<<endl;
		for(int gi=0;gi<num;gi++)
		{
			cout<<setw(10)<<G[gi];
		}
		//system("PAUSE");
		//end the while loop
	}

}
*/
//------------cal G
void T2::T2_G(int m,vector<double>::iterator dbeg,double * cx,double * cu,deque<double*>::iterator xbeg,deque<double*>::iterator ubeg)
{
	for(int i=0;i<m;i++)
	{
		double * xm=*(xbeg+i);
		double * um=*(ubeg+i);
		cout<<"The xm is: "<<endl;
		for(int xi=1;xi<=linksNum;xi++)
		{
			cout<<setw(20)<<xm[xi];
		}
		cout<<"The cx is: "<<endl;
		for(int xi=1;xi<=linksNum;xi++)
		{
			cout<<setw(20)<<cx[xi];
		}
		//system("PAUSE");
		for(int j=1;j<=linksNum;j++)
		{
			double de=links[j].cost;
			double se=links[j].calTime(cx[j]);
			*(dbeg+i)+=(de*(xm[j]-cx[j])+se*(um[j]-cu[j]));
		}
		cout<<"The G"<<i<<"is :"<<*(dbeg+i)<<endl;
		//system("PAUSE");
	}
}


void T2::T2_DG(int m,deque<double*>::iterator dgbeg,double * cx,double * cu,deque<double*>::iterator xbeg,deque<double*>::iterator ubeg)
{
	for(int mi=0;mi<m;mi++)
	{
		
		double * ptr_um=*(xbeg+mi);//m th column in the U
		double * ptr_xm=*(ubeg+mi);//m th column in the X
		double * ptr_mDG=*(dgbeg+mi);
		for(int ni=0;ni<m;ni++)
		{
			double * ptr_un=*(xbeg+ni);// n th column in the U
			double * ptr_xn=*(ubeg+ni);// n th column in the X
			for(int j=1;j<=linksNum;j++)
			{
				ptr_mDG[ni]+=((links[j].calDTime(cx[j])*(ptr_um[j]-cu[j])-links[j].cost)*ptr_xn[j]-links[j].calTime(cx[j])*ptr_un[j]);
				 
			}
			cout<<"The ptr_mDG "<<ni<<" is :"<<ptr_mDG[ni]<<endl;
			//system("PAUSE");
		}
	}
}

//ini
void T2::T2_ini(double* new_x,double* new_u)
{
	for(int oi=1;oi<=odsNum;oi++)
	{
		vector<double> xl(linksNum+1,0);//link u
		vector<double> ul(linksNum+1,0);//link x

		int o=ods[oi].orignID;
		T2_MPA(o,&xl[0],&ul[0]);
		for(int li=1;li<=linksNum;li++)
		{
			new_x[li]+=xl[li];
			new_u[li]+=ul[li];
		}
	}
}
//solve the subproblem
void T2::T2_sub(double* new_x,double* new_u)
{
	for(int li=1;li<=linksNum;li++)
	{
		new_x[li]=0;
		new_u[li]=0;
	}
	for(int oi=1;oi<=odsNum;oi++)
	{
		vector<double> xl(linksNum+1,0);
		vector<double> ul(linksNum+1,0);

		int o=ods[oi].orignID;
		T2_MPA(o,&xl[0],&ul[0]);
		for(int li=1;li<=linksNum;li++)
		{
			new_x[li]+=xl[li];
			new_u[li]+=ul[li];
		}
	}
}
//
void T2::T2_sub2(double* new_x,double* new_u)
{
	for(int li=1;li<=linksNum;li++)
	{
		new_x[li]=0;
		new_u[li]=0;
	}
	for(int oi=1;oi<=odsNum;oi++)
	{
		vector<double> xl(linksNum+1,0);
		vector<double> ul(linksNum+1,0);

		int o=ods[oi].orignID;
		T2_MPA2(o,&xl[0],&ul[0]);
		for(int li=1;li<=linksNum;li++)
		{
			new_x[li]+=xl[li];
			new_u[li]+=ul[li];
		}
	}
}
//------write the OD and demands into file
void T2::writeDemand(string file,long seekarg)
{
	ofstream outFile(file,seekarg);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	outFile<<setw(10)<<odsNum<<"\n";
	for(int i=1;i<=odsNum;i++)
	{
		outFile<<setw(10)<<ods[i].orignID<<setw(10)<<ods[i].pairID<<"\n";
	}
	outFile<<setw(10)<<demandsNum<<"\n";
	for(int i=1;i<=demandsNum;i++)
	{
		outFile<<setw(10)<<demands[i].destiID<<setw(10)<<demands[i].demand<<"\n";
	}
	outFile.close();
}

//----write the link to the file
void T2::writeLink(string file,long seekarg)
{
	ofstream outFile(file,seekarg);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	outFile<<setw(10)<<nodesNum<<"\n";
	for(int i=1;i<=nodesNum;i++)
	{
		outFile<<setw(10)<<nodes[i].beginLinkID<<setw(10)<<nodes[i].endLinkID<<"\n";
	}
	outFile<<setw(10)<<linksNum<<"\n";
	for(int j=1;j<=linksNum;j++)
	{
		outFile<<setw(10)<<links[j].fromNode<<setw(10)<<links[j].toNode<<setw(10)<<links[j].capacity<<setw(10)<<links[j].length<<setw(10)<<links[j].speed<<setw(10)<<links[j].cost<<"\n";
	}
	outFile.close();
}
//-----write the file 
void T2::T2_write(string file,long seekarg)
{
	ofstream outFile(file,seekarg);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	//outFile.clear();
	outFile<<"---------This is the output of algorithm T2--------"<<"\n";
	outFile<<"\n"<<"\n"<<"\n";
	outFile<<"---------------------------------------------------------------------------------------------------"<<"\n";
	outFile<<left<<setw(15)<<"FromNode"<<setw(15)<<"ToNode"<<setw(15)<<"Flow"<<setw(15)<<"Time"<<setw(15)<<"Cost"<<"\n";
	for(int li=1;li<=linksNum;li++)
	{
		outFile<<left<<setw(15)<<links[li].fromNode<<setw(15)<<links[li].toNode<<setw(15)<<links[li].flow<<setw(15)<<links[li].cTime*60<<setw(15)<<links[li].cost<<"\n";
	}
	outFile.close();
}
//-----set float precision
double T2::setP(double fl)
{
	double b;
	b=(int)((fl*10000))/10000.0;
	return b;
}