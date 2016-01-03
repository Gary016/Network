#include "network.h"
#include<fstream>
using std::ifstream;
using std::ofstream;
#include<string>
using std::string;
#include<iomanip>
using std::setw;
#include<ctime>
//----construction function
Network::Network()
{
	//set the vot values
	vot_max=60.0;
	vot_min=0.0;

	
	
	
	setNodesNum(0);
	setLinksNum(0);
	setONum(0);
	setPairNum(0);
	nodeCost.push_back(0.0);
	nodeTime.push_back(0.0);
	bl.push_back(0);
}

//---get
int Network::getNodesNum()
{
	return nodesNum;
}
int Network::getLinksNum()
{
	return linksNum;
}

int Network::getONum()
{
	return odNum;
}
int Network::getPairNum()
{
	return pairNum;
}


//---set
void Network::setNodesNum(int n)
{
	nodesNum=n;
}
void Network::setLinksNum(int m)
{
	linksNum=m;
}

void Network::setONum(int o)
{
	odNum = o;
}
void Network::setPairNum(int pn)
{
	pairNum = pn;
}

//
void Network::writeLink(string file,long seekarg)
{
	ofstream outFile(file,seekarg);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	//outFile.clear();
	outFile<<"---------This is the output of algorithm DBA--------"<<"\n";
	outFile<<"\n"<<"\n"<<"\n";
	outFile<<"---------------------------------------------------------------------------------------------------"<<"\n";
	outFile<<left<<setw(15)<<"FromNode"<<setw(15)<<"ToNode"<<setw(15)<<"Flow"<<setw(15)<<"Time"<<setw(15)<<"Cost"<<"\n";
	for(int li=1;li<=linksNum;li++)
	{
		outFile<<left<<setw(15)<<links[li].fromNode<<setw(15)<<links[li].toNode<<setw(15)<<links[li].flow<<setw(15)<<links[li].cTime<<setw(15)<<links[li].cost<<"\n";
	}
	outFile.close();
}
//---Read function
//---read network for the multiclass
void Network::readNet(string file, long seekarg)
{
	//ini the nodes and links
	nodes.clear();
	links.clear();
	Node n00;
	Link l00;
	nodes.push_back(n00);
	links.push_back(l00);

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

	}
	//----read the links number
	inFile>>linksNum;
	cout<<"The number of links in the network is: "<<linksNum<<endl;

	//----read the links attributes
	for ( int i=1;i<linksNum+1;i++)
	{
		int fNode=0;
		int eNode=0;
		double cap=0.0;
		double len=0.0;
		double spe=0.0;
		double toll=0.0;
		Link li;
		inFile>>fNode>>eNode>>cap>>len>>spe>>toll;
		li.linkID=i;
		li.fromNode=fNode;
		li.toNode=eNode;
		li.capacity=cap;
		li.length=len;
		li.speed=spe;
		li.cost=toll;
		li.cTime=len/spe;
		li.vot=vot_max;
		li.gcost=li.cost+vot_max*li.cTime;


		links.push_back(li);
		bl.push_back(i);
	}
	//after read the network, then give the dimention of pred and travel time 
	pred.clear();
	predLink.clear();
	nodeCost.clear();
	nodeTime.clear();
	travelTime.clear();
		for (int i=0;i<nodesNum+2;i++)
	{
        pred.push_back(inf);
		predLink.push_back(inf);
		nodeCost.push_back(0.0);
		nodeTime.push_back(0.0);
		travelTime.push_back(0.0);
	}
		//
		y.clear();
		dir.clear();
		v.clear();
		pre_x.clear();
	// give the dimention of y and dir
	for(int i=0;i<linksNum+1;i++)
	{
		y.push_back(0.0);
		dir.push_back(0.0);
		v.push_back(0.0);
		pre_x.push_back(0.0);

	}
}
//read network data
void Network::readData(string file,long seekarg )
{
	//ini the nodes and links
	nodes.clear();
	links.clear();
	Node n00;
	Link l00;
	nodes.push_back(n00);
	links.push_back(l00);

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

	}
	//----read the links number
	inFile>>linksNum;
	cout<<"The number of links in the network is: "<<linksNum<<endl;

	//----read the links attributes
	for ( int i=1;i<linksNum+1;i++)
	{
		int fNode=0;
		int eNode=0;
		double cap=0.0;
		double len=0.0;
		double spe=0.0;
		Link li;
		inFile>>fNode>>eNode>>cap>>len>>spe;
		li.linkID=i;
		li.fromNode=fNode;
		li.toNode=eNode;
		li.capacity=cap;
		li.length=len;
		li.speed=spe;
		li.cTime=len/spe;
		links.push_back(li);
	}
	//after read the network, then give the dimention of pred and travel time 
	pred.clear();
	predLink.clear();
	travelTime.clear();
		for (int i=0;i<nodesNum+2;i++)
	{
        pred.push_back(inf);
		predLink.push_back(inf);
		travelTime.push_back(0.0);
	}
	// give the dimention of y and dir
		y.clear();
		dir.clear();
		v.clear();
		pre_x.clear();
	for(int i=0;i<linksNum+1;i++)
	{
		y.push_back(0.0);
		dir.push_back(0.0);
		v.push_back(0.0);
		pre_x.push_back(0.0);

	}
}
//read demand data
void Network::readDemand(string file, long seekarg)
{
	ods.clear();
	demands.clear();
	bushs.clear();
	OD od00;
	Demand demand00;
	bush bush00;
	ods.push_back(od00);
	demands.push_back(demand00);
	bushs.push_back(bush00);

	ifstream inFile(file,seekarg); //put demand data into "inFile"

	if (!inFile)
	{
		cerr<<"File could not be opened"<<endl;
		exit(1);
	} //end if 

	//read the number of O
	inFile >> odNum;
	vector<double> ll(linksNum+1,0.0);
	cout<<" The Number of O is: "<<odNum<<endl;
	//read the O into ods;
	for(int i=1; i<odNum+1;i++)
	{
		int oid;
		int pid;
		OD od;
		inFile>>oid>>pid;
		od.orignID=oid;
		od.pairID=pid;
		od.OBLflow=ll;
		ods.push_back(od);
	}
	//---ini bush and path
	path p0;
	bush b0;
	bushNode bn0;
	bushlink bl0;
	for(int i=1;i<=nodesNum;i++)
	{
		b0.nodes.push_back(bn0);// ini the bush nodes
		
		p0.traceNode.push_back(0);//set the path traceNode size to nodesNum+1
		p0.traceLink.push_back(0);//set the path traceLink size to linksNum+1
	}
	for(int i=1;i<=linksNum;i++)
	{
		b0.linkStatus.push_back(0);
		b0.linkFlow.push_back(0.0);
		b0.links.push_back(bl0);//ini the bush links
		//for luce 
		//b0.yluce.push_back(0.0);//ini the luce proportion variables
		//b0.eluce.push_back(0.0);
	}
	for(int i=1;i<=odNum;i++)
	{
		bushs.push_back(b0);
	}
	//-----
	for(int i=1;i<odNum;i++)
	{
		ods[i].endPID=ods[i+1].pairID-1;
	}

	//read the number of the pairs
	inFile>>pairNum;
	cout<<"The number of OD pairs in the network is: "<<pairNum<<endl;
	//read the demands into the vector demands
	for(int i=1;i<pairNum+1;i++)
	{
		int destin;
		double dem;
		Demand d;
		inFile>>destin>>dem;
		d.pairID=i;
		d.destiID=destin;
		d.demand=dem;
		d.paths.push_back(p0);//set the 0 of paths as p0
		demands.push_back(d);
	}
	ods[odNum].endPID=pairNum;
	for(int di=1;di<=odNum;di++)
	{
		for(int dj=ods[di].pairID;dj<=ods[di].endPID;dj++)
		{
			demands[dj].orignID=ods[di].orignID;
		}
	}
}
//---------------------------------algorithm 5 : 
void Network::mba5(double error)
{
	int Iter=0;
	ini4();

	double maingap=1.0;
	while((Iter<500)&&(maingap>error))
	{
		double dgap=1.0;
	    int mm=0;
	    double T1=0.0;
	    double T2=0.0;
	    while((dgap>error)&&(mm<50))
		{

		}
	  }

}


//--------------------------------algorithm 4: another multi bush-based algorithm
void Network::mba4(double error)
{
	int Iter=0;
	//ini the algorithm
	//cout<<"befor the ini "<<endl;
	//system("PAUSE");
	ini4();
	//cout<<"afte the ini "<<endl;
	//system("PAUSE");
	double maingap=1.0;
	
	while((Iter<500)&&(maingap>error))
	{
		//re creat the bush 
		//cout<<"before the recreatbush"<<endl;
		//system("PAUSE");
		int dd=recreatbush4();
		//cout<<"before the eq "<<endl;
		//system("PAUSE");
		if(dd)
		{
			int num=eqbush4(10e-8);
			//cout<<"The diff eq num is : "<<num<<endl;
			//cout<<"The dd is : "<<dd <<endl;
		}
		else
		{
			int num=eqbush4(error);
			//cout<<"The same eq num is : "<<num<<endl;
			if(num<49)
			{
				//cout<<"The eq has found "<<endl;
				continue;

			}
		}
		//cout<<"AFTER eq "<<endl;
		//system("PAUSE");
		

	}
	double T4=calObjective(&links[0]);
	cout<<"The objective is : "<<T4<<endl;
}

//

int Network::recreatbush4()
{
	int diff=0;
	//creat the bushs for each origin
	for(int oi=1;oi<=odNum;oi++)
	{
		int temp=ods[oi].bushNum;
		creatbush4(oi);
		diff+=abs(temp-ods[oi].bushNum);
		//expand the bushs
		for(int bi=0;bi<ods[oi].bushNum;bi++)
		{
			reBushTime3(&ods[oi].obushs[bi]);
			expandBush3(&ods[oi].obushs[bi]);
		}
		//eq the bushs for the o
		for(int bj=0;bj<ods[oi].bushNum;bj++)
		{
				int nn=1;
				int re=1;
				while((nn<10)&&(re==1))
				{
					nn+=1;
					calTopo3(&ods[oi].obushs[bj],oi);
					int oo=0;
					double iigap=1.0;
					while((oo<10)&&(iigap>0.01))
					{
						oo+=1;
						calTree3(&ods[oi].obushs[bj],oi);
						iigap=eqBush3(&ods[oi].obushs[bj],oi,ods[oi].bushNum,&ods[oi].obushs[0]);
					}
					reduceBush3(&ods[oi].obushs[bj],oi);
					calTopo3(&ods[oi].obushs[bj],oi);
					calTree3(&ods[oi].obushs[bj],oi);
					re=expandBush3(&ods[oi].obushs[bj]);
				}
		}
	}

	

	return diff;
}
//--------------------ini4()
//creat the bushs and eq them
void Network::ini4()
{
	//creat the bushs for each origin

	for(int oi=1;oi<=odNum;oi++)
	{
		creatbush4(oi);
		//
		for(int bi=0;bi<ods[oi].bushNum;bi++)
		{
			reBushTime3(&ods[oi].obushs[bi]);
			expandBush3(&ods[oi].obushs[bi]);
		}
		//
		//eq the bushs for the o
		for(int bj=0;bj<ods[oi].bushNum;bj++)
		{
				int nn=1;
				int re=1;
				while((nn<10)&&(re==1))
				{
					nn+=1;
					calTopo3(&ods[oi].obushs[bj],oi);
					int oo=0;
					double iigap=1.0;
					while((oo<10)&&(iigap>0.01))
					{
						oo+=1;
						calTree3(&ods[oi].obushs[bj],oi);
						iigap=eqBush3(&ods[oi].obushs[bj],oi,ods[oi].bushNum,&ods[oi].obushs[0]);
					}
					reduceBush3(&ods[oi].obushs[bj],oi);
					calTopo3(&ods[oi].obushs[bj],oi);
					calTree3(&ods[oi].obushs[bj],oi);
					re=expandBush3(&ods[oi].obushs[bj]);
				}
		}
	}

	

	//eq the bushs
	int eqnum =eqbush4(0.000001);
	cout<<"The ini eq num is : "<<eqnum<<endl;

}
//-------------eqbush4
int Network::eqbush4(double er)
{
	double dgap=1.0;
	int mm=0;
	double T1=0.0;
	double T2=0.0;
	while((dgap>er)&&(mm<50))
	{
		mm+=1;
		T1=calObjective(&links[0]);
		//begin the loop
		for(int b=1;b<=odNum;b++)
		{
			for(int bj=0;bj<ods[b].bushNum;bj++)
			{
				int nn=1;
				int re=1;
				while((nn<10)&&(re==1))
				{
					nn+=1;
					calTopo3(&ods[b].obushs[bj],b);
					int oo=0;
					double iigap=1.0;
					while((oo<10)&&(iigap>0.01))
					{
						oo+=1;
						calTree3(&ods[b].obushs[bj],b);
						iigap=eqBush3(&ods[b].obushs[bj],b,ods[b].bushNum,&ods[b].obushs[0]);
					}
					reduceBush3(&ods[b].obushs[bj],b);
					calTopo3(&ods[b].obushs[bj],b);
					calTree3(&ods[b].obushs[bj],b);
					re=expandBush3(&ods[b].obushs[bj]);
				}
			}
		}
		T2=calObjective(&links[0]);
		dgap=abs(1-T1/T2);
		cout.precision(15);
		cout<<"The dgap is : "<<dgap<<endl;
		cout<<"The objective is : "<<T2<<endl;
		
	}
	return mm;
}

//-------------------------recreat bushs for mba4

void Network::creatbush4(int b)
{
	//clear the obushs
	ods[b].obushs.clear();
	ods[b].bushNum=0;
	//
	double alb=vot_min;
	double aub=vot_max;
	vector<double> dnode(nodesNum+1,0.0);
	vector<double> snode(nodesNum+1,0.0);
	vector<double> a(linksNum+1,-1.0);//PTU variables

	//creat the bush b0
	bush b0;
	bushNode bn0;
	bushlink bl0;
	//cout<<"Befor creat Bushs"<<endl;
	//system("PAUSE");

	for(int ni=1;ni<=nodesNum;ni++)
	{
		b0.nodes.push_back(bn0);
	}
	for(int li=1;li<=linksNum;li++)
	{
		b0.links.push_back(bl0);
		b0.linkFlow.push_back(0.0);
		b0.linkStatus.push_back(0);
	}
	//---end of the bush b0

	
	
	//--------------creat the bushs for the origin
	ods[b].obushs.push_back(b0);
	ods[b].bushNum+=1;
	min(b,&dnode[0],&snode[0],&alb,&a[0]);


	creatBush(b,&ods[b].obushs[0],aub,alb,&dnode[0],&snode[0]);

	//creat the sbush
	bush sbush=ods[b].obushs[0];
	//cout the tree
	///*
	calTopo3(&sbush,b);
	cout<<"The topology order of tree is : "<<endl;
	for(int ni=0;ni<nodesNum;ni++)
	{
		cout<<"order "<<ni<<" is "<<sbush.topology[ni]<<endl;
	}
	system("PAUSE");
	//*/
	//creat the following bush
	while(alb)
	{
		ods[b].obushs.push_back(b0);
		ods[b].bushNum+=1;
		
		ptu(b,&dnode[0],&snode[0],&aub,&alb,&a[0]);

		creatBush(b,&ods[b].obushs[ods[b].bushNum-1],aub,alb,&dnode[0],&snode[0]);

		//combine all the tree into the sbush
		combineTree(&sbush);
		
	}
	//use the calTopo to test the if the sbush exist a cycle
	calTopo3(&sbush,b);
	cout<<"cout the sbush : "<<endl;
	for(int ni=1;ni<=nodesNum;ni++)
	{
		cout<<"node "<<ni<<" cout nodes are : ";
		for(int nj=0;nj<sbush.nodes[ni].clinks.size();nj++)
		{
			int sl=sbush.nodes[ni].clinks[nj];
			int jnode=links[sl].toNode;
			cout<<setw(10)<<jnode;
		}
		cout<<endl;

	}
	system("PAUSE");
	cout<<"The topology order of sbush is : "<<endl;
	for(int ni=0;ni<nodesNum;ni++)
	{
		cout<<"order "<<ni<<" is "<<sbush.topology[ni]<<endl;
	}
	system("PAUSE");
	//update the bush flow onto the origin flow
	for(int li=1;li<=linksNum;li++)
	{
		double olflow=0.0;
		for(int bi=0;bi<ods[b].bushNum;bi++)
		{
			olflow+=ods[b].obushs[bi].linkFlow[li];
		}
		ods[b].OBLflow[li]=olflow;
	}
	//update the flow
	for(int li=1;li<=linksNum;li++)
	{
		double lflow=0.0;
		for(int oi=1;oi<=odNum;oi++)
		{
			lflow+=ods[oi].OBLflow[li];
		}
		links[li].flow=lflow;
		links[li].setCTime();
		links[li].setDTime();
	}
}

//----------------combine the shortest tree to a network
int Network::combineTree(bush* sbush)
{
	for(int ni=1;ni<=nodesNum;ni++)
	{
		int pl=predLink[ni];
		if(pl)
		{
			int inode=links[pl].fromNode;
			int jnode=ni;
			int te=0;
			//check if link pl is in the sbush 
			if(sbush->linkStatus[pl]==1)
			{
				te=1;
			}
			/*
			for(int li=0;li<sbush->nodes[jnode].rlinks.size();li++)
			{
				if(sbush->nodes[jnode].rlinks[li]==pl)
				{
					te=1;
				}
			}
			*/
			//if not, put the link into sbush
			if(!te)
			{
				sbush->linkStatus[pl]=1;
				sbush->nodes[inode].clinks.push_back(pl);
				sbush->nodes[jnode].rlinks.push_back(pl);
			}
		}
	}
	return 0;
}
//--------------------------------algorithm 3: Multi bush-based multiclass algorithm
void Network::oba(int b)
{
	cout<<"Begin a new Oba algorithm"<<endl;
	int o=ods[b].orignID;//b is the index of ods and o is the origin ID
	int Iter=0;
	int bushNum=0;
	//creat the bushs for this origin
	double a_lb=vot_min;
	double a_ub=vot_max;
	vector<double> d_node(nodesNum+1,0.0);
	vector<double> s_node(nodesNum+1,0.0);
	vector<double> a(linksNum+1,-1.0);//PTU variables
	//define the bushs 
	vector<bush> obushs;
	//creat the bush b0
	bush b0;
	bushNode bn0;
	bushlink bl0;
	cout<<"Befor creat Bushs"<<endl;
	system("PAUSE");

	for(int ni=1;ni<=nodesNum;ni++)
	{
		b0.nodes.push_back(bn0);
	}
	for(int li=1;li<=linksNum;li++)
	{
		b0.links.push_back(bl0);
		b0.linkFlow.push_back(0.0);
		b0.linkStatus.push_back(0);
	}
	//---end of the bush b0
	obushs.push_back(b0);//creat the first bush into the bushs
	bushNum+=1;
	cout<<"Before Min"<<endl;
	system("PAUSE");
	min(b,&d_node[0],&s_node[0],&a_lb,&a[0]);
	cout<<"The bush bounds are : "<<endl;
	cout<<a_ub<<"--->"<<a_lb<<setw(10);
	
	cout<<"befor creat bush " <<endl;
	system("PAUSE");
	creatBush(b,&obushs[0],a_ub,a_lb,&d_node[0],&s_node[0]);
	///*
	cout<<"Bush : "<<bushNum<<endl;
	cout<<"The vot bounds are : "<<a_ub<<setw(10)<<a_lb<<endl;
	
	cout<<"the "<<bushNum<<" bush "<<endl;
		cout<<"the bush flow are: "<<endl;
		for(int lj=1;lj<=linksNum;lj++)
		{
			cout<<links[lj].fromNode<<"-->"<<links[lj].toNode<<setw(10)<<obushs[bushNum-1].linkFlow[lj]<<endl;
		}
		system("PAUSE");
		//*/
	//creat the following bush
	while(a_lb)
	{
		obushs.push_back(b0);
		bushNum+=1;
		ptu(b,&d_node[0],&s_node[0],&a_ub,&a_lb,&a[0]);

		cout<<a_ub<<"--->"<<a_lb<<setw(10);

		creatBush(b,&obushs[bushNum-1],a_ub,a_lb,&d_node[0],&s_node[0]);
		/*
		cout<<"Bush : "<<bushNum<<endl;
	    cout<<"The vot bounds are : "<<a_ub<<setw(10)<<a_lb<<endl;

		cout<<"the "<<bushNum<<" bush "<<endl;
		cout<<"the bush flow are: "<<endl;
		for(int lj=1;lj<=linksNum;lj++)
		{
			cout<<links[lj].fromNode<<"-->"<<links[lj].toNode<<setw(10)<<obushs[bushNum-1].linkFlow[lj]<<endl;
		}
		system("PAUSE");
		*/
	}
	cout<<endl;
	system("PAUSE");


	//cout<<"After creat all the bushs"<<endl;
	//system("PAUSE");
	//------------update the flow and Ctime and Dtime
	//vector<double> oflow(linksNum+1,0.0);
	for(int li=1;li<=linksNum;li++)
	{
		double olflow=0.0;
		for(int bi=0;bi<bushNum;bi++)
		{
			olflow+=obushs[bi].linkFlow[li];
		}
		ods[b].OBLflow[li]=olflow;
	}
	for(int li=1;li<=linksNum;li++)
	{
		double lflow=0.0;
		for(int oi=1;oi<=odNum;oi++)
		{
			lflow+=ods[oi].OBLflow[li];
		}
		links[li].flow=lflow;
		links[li].setCTime();
		links[li].setDTime();
	}
	/*
	//cout the link flows
	cout<<"The link flow before eq are: "<<endl;
		for(int li=1;li<=linksNum;li++)
		{
			cout<<li<<"--->"<<links[li].flow<<endl;
		}
		system("PAUSE");
		*/
	

	//expand the bush
	for(int bi=0;bi<bushNum;bi++)
	{
		reBushTime3(&obushs[bi]);
		

		expandBush3(&obushs[bi]);


	}

	int eqIter=0;
	//begin eq the o
	//cout<<"Begin the loop "<<endl;
	while(eqIter<5)
	{
		eqIter+=1;
		for(int bj=0;bj<bushNum;bj++)
		{
			//cout<<"Begin the bush "<<bj+1<<endl;
			//system("PAUSE");
			int inIter=0;
			//begin the cycle of the bush
			int re=1;
			while((inIter<10)&&(re==1))
			{
				inIter+=1;
				
				calTopo3(&obushs[bj],b);
				/*
				//cout the topo of bush
				cout<<"the topo are : "<<endl;
				for(int ni=0;ni<nodesNum;ni++)
				{
					cout<<obushs[bj].topology[ni]<<setw(10);
					cout<<endl;
				}
				system("PAUSE");
				*/
				
				int inum=0;
				double ingap=1.0;
				while((inum<10)&&(ingap>0.001))
				{
					inum+=1;
					calTree3(&obushs[bj],b);
					/*
					//cout the bush
					cout<<"cout the bush"<<endl;
					for( int ni=1;ni<=nodesNum;ni++)
			        {
				         cout<<ni<<" : ";
				         for(int ci=0;ci<obushs[bj].nodes[ni].clinks.size();ci++)
				         {
					        int l=obushs[bj].nodes[ni].clinks[ci];
					         int jnode=links[l].toNode;
					       cout<<setw(10)<<jnode;
				          }
				          cout<<endl;
				
			         }
			          system("PAUSE");
					*/
					/*
					//cout the shortest tree
					cout<<"The shortest tree is : "<<endl;
					for(int ni=1;ni<=nodesNum;ni++)
					{
						int l=obushs[bj].nodes[ni].p;
						cout<<ni<<"<----"<<links[l].fromNode<<setw(10)<<links[l].toNode<<endl;
					}
					cout<<"The longest tree is : "<<endl;
					for(int ni=1;ni<=nodesNum;ni++)
					{
						int l=obushs[bj].nodes[ni].P;
						cout<<ni<<"<----"<<links[l].fromNode<<setw(10)<<links[l].toNode<<endl;

					}
					system("PAUSE");
					*/
					//cout<<"After cal Tree"<<endl;
					//system("PAUSE");
					ingap=eqBush3(&obushs[bj],b,bushNum,&obushs[0]);
					//cout<<"After eqBush"<<endl;
					//system("PAUSE");
				}
				//
				/*
				//cout the bush
					cout<<"cout the bush before the reduce"<<endl;
					for( int ni=1;ni<=nodesNum;ni++)
			        {
				         cout<<ni<<" : ";
				         for(int ci=0;ci<obushs[bj].nodes[ni].clinks.size();ci++)
				         {
					        int l=obushs[bj].nodes[ni].clinks[ci];
					         int jnode=links[l].toNode;
					       cout<<setw(10)<<jnode;
				          }
				          cout<<endl;
				
			         }
			          system("PAUSE");
					  */
				reduceBush3(&obushs[bj],b);

				/*
				//cout the bush
					cout<<"cout the bush after the reduce"<<endl;
					for( int ni=1;ni<=nodesNum;ni++)
			        {
				         cout<<ni<<" : ";
				         for(int ci=0;ci<obushs[bj].nodes[ni].clinks.size();ci++)
				         {
					        int l=obushs[bj].nodes[ni].clinks[ci];
					         int jnode=links[l].toNode;
					       cout<<setw(10)<<jnode;
				          }
				          cout<<endl;
				
			         }
			          system("PAUSE");
			*/
				calTopo3(&obushs[bj],b);
				calTree3(&obushs[bj],b);
			/*
			cout<<"The bush before expand bush are"<<endl;
			for( int ni=1;ni<=nodesNum;ni++)
			{
				cout<<ni<<" : ";
				for(int ci=0;ci<obushs[bj].nodes[ni].clinks.size();ci++)
				{
					int l=obushs[bj].nodes[ni].clinks[ci];
					int jnode=links[l].toNode;
					cout<<setw(10)<<jnode;
				}
				cout<<endl;
				
			}
			system("PAUSE");
			*/
				re=expandBush3(&obushs[bj]);

				/*
				cout<<"The bush after expand bush are"<<endl;
			for( int ni=1;ni<=nodesNum;ni++)
			{
				cout<<ni<<" : ";
				for(int ci=0;ci<obushs[bj].nodes[ni].clinks.size();ci++)
				{
					int l=obushs[bj].nodes[ni].clinks[ci];
					int jnode=links[l].toNode;
					cout<<setw(10)<<jnode;
				}
				cout<<endl;
				
			}
			system("PAUSE");
			//*/

			}

		}
		/*
		//cout the link flows
	    cout<<"The link flow after eq are: "<<endl;
		for(int li=1;li<=linksNum;li++)
		{
			cout<<li<<"--->"<<links[li].flow<<endl;
		}
		system("PAUSE");
		*/
	}

	
}
//--------------------------------end of the oba

//---------------------Reduce Bush 3 algorithm
void Network::reduceBush3(bush* ptrBush,int b )
{
	for(int i=1;i<=linksNum;i++)
	{
		int i_node=links[i].fromNode;
		int j_node=links[i].toNode;
		if((ptrBush->linkStatus[i]==1)&&(ptrBush->linkFlow[i]==0)&&(ptrBush->nodes[j_node].rlinks.size()>1))
		{
			//delete the link
			ptrBush->linkStatus[i]=0;
			//delete the link from the clinks
			while(!ptrBush->nodes[i_node].clinks.empty())
			{
				int dLink=ptrBush->nodes[i_node].clinks.front();
				ptrBush->nodes[i_node].clinks.pop_front();
				if(dLink==i)
				{
					break;
				}
				else
				{
					ptrBush->nodes[i_node].clinks.push_back(dLink);
				}
			}
			//delete the link from the rlink
			while(!ptrBush->nodes[j_node].rlinks.empty())
			{
				int dLink=ptrBush->nodes[j_node].rlinks.front();
				ptrBush->nodes[j_node].rlinks.pop_front();
				if(dLink==i)
				{
					break;
				}
				else
				{
					ptrBush->nodes[j_node].rlinks.push_back(dLink);
				}
			}
		}
	}
}


//---------------------
//------------eq the bush 3
double Network::eqBush3(bush *ptrBush, int b,int bushNum,bush* ptrB0)
{
	double aveVot=ptrBush->aveVot;

	double ingap=0;
	for(int s=nodesNum-1;s>0;s--)//---search every node according to the decending pass
	{
		deque<int> S;
		deque<int> L;		
		//find the shortest path links and put them into a vector S
		int k=ptrBush->topology[s];// define the k as the s decending element 
		while(ptrBush->nodes[k].p)
		{
			int te=ptrBush->nodes[k].p;
			int teNode=links[te].fromNode;
			S.push_back(te);
			k=teNode;

		}
		/*
		//cout the S
		cout<<"Ths S is : "<<endl;
		for(int si=0;si<S.size();si++)
		{
			int l=S[si];
			int ip=links[l].fromNode;
			int jp=links[l].toNode;
			cout<<ip<<"--->"<<jp<<endl;
		}
		system("PAUSE");
		*/
		//find the longest path links and put them into a vector L
		k=ptrBush->topology[s];
		while(ptrBush->nodes[k].P)
		{
			int te=ptrBush->nodes[k].P;
			int teNode=links[te].fromNode;
			L.push_back(te);
			k=teNode;

		}
		/*
		cout<<"The L is : "<<endl;
		for(int si=0;si<L.size();si++)
		{
			int l=L[si];
			int ip=links[l].fromNode;
			int jp=links[l].toNode;
			cout<<ip<<"--->"<<jp<<endl;
		}
		system("PAUSE");
		*/
		// if i==j, then jump to next j
		if(S.front()==L.front()) continue;
	    //find the first set of different element
		deque<int> S1;
		deque<int> L1;
		int mernode=0;
		for(unsigned sfi=0;sfi<S.size();sfi++)
		{
			int sfnode=links[S[sfi]].fromNode;
			int label=0;
			for(unsigned lfi=0;lfi<L.size();lfi++)
			{
				if(links[L[lfi]].fromNode==sfnode)
				{
					label=1;
					mernode=sfnode;
					break;
				}
			}
			if(label) break;
		  }
		// find the S1 and L1
		int senode=links[L.front()].fromNode;
		do
		{
			int sflink=S.front();
			S1.push_back(sflink);
			S.pop_front();
			S.push_back(sflink);
		    senode=links[sflink].fromNode;
			//senode=links[S.front()].fromNode;
		}
		while(senode!=mernode);

		int lenode=links[L.front()].fromNode;
		do
		{
			int lflink=L.front();
			L1.push_back(lflink);
			L.pop_front();
			L.push_back(lflink);
			lenode=links[lflink].fromNode;
		}
		while(lenode!=mernode);

		/*
		cout<<"The S1 is : "<<endl;
		for(int si=0;si<S1.size();si++)
		{
			int l=S1[si];
			int ip=links[l].fromNode;
			int jp=links[l].toNode;
			cout<<ip<<"--->"<<jp<<endl;
		}
		system("PAUSE");
		*/
		/*
		cout<<"The L1 is : "<<endl;
		for(int si=0;si<L1.size();si++)
		{
			int l=L1[si];
			int ip=links[l].fromNode;
			int jp=links[l].toNode;
			cout<<ip<<"--->"<<jp<<endl;
		}
		system("PAUSE");		
		*/
		// cal the combine set of S1 and L1
		deque<int> SL(S1);
		for(unsigned li=0;li<L1.size();li++)
		{
			SL.push_back(L1[li]);
		}
		/*
		cout<<"The SL is : "<<endl;
		for(int si=0;si<SL.size();si++)
		{
			int l=SL[si];
			int ip=links[l].fromNode;
			int jp=links[l].toNode;
			cout<<ip<<"--->"<<jp<<endl;
		}
		system("PAUSE");
		*/
		//calculate the g,h and dx : see ( Yu Nie, 2010)
		//cal g
				
		double gl=0.0;
		double gs=0.0;
		for(unsigned l1=0;l1<L1.size();l1++)
		{
			gl+=links[L1[l1]].cTime*aveVot+links[L1[l1]].cost;
		}
		//cout<<"the gl is : "<<gl<<endl;
		for(unsigned s1=0;s1<S1.size();s1++)
		{
			gs+=links[S1[s1]].cTime*aveVot+links[S1[s1]].cost;
		}
		//cout<<"the gs is : "<<gs<<endl;
		double g=gl-gs;

		//cout<<"the g is : "<<g<<endl;
		//system("PAUSE");

		if(g<0) g=0;
		//cout<<"g is: "<<g<<endl;
		//cal h
		double h=0.0;
		for(unsigned sli=0;sli<SL.size();sli++)//cal h
		{
			h+=links[SL[sli]].dTime*aveVot;//the derivate of formulation: aveVot*ctime+cost
		}
		//cout<<"h is: "<<h<<endl;

		//find the smallest flow in the L1 of bush
		double slf=ptrBush->linkFlow[L1[0]];//define the smallest flow in the L
		for(unsigned li=0;li<L1.size();li++)
		{
			if(slf>=ptrBush->linkFlow[L1[li]])
			{
				slf=ptrBush->linkFlow[L1[li]];
			}
		}
		
		//cout<<"the slf is : "<<slf<<endl;

		double lamda=0.8;
	

		double dx=lamda*(g/h);
		//cout<<"the dx is : "<<dx<<endl;
		if(dx>slf) dx=slf;

		//cout<<"the real dx is : "<<dx<<endl;
		//system("PAUSE");
		//cout<<"dx is: "<<dx<<endl;
		k=ptrBush->topology[s];
		if(dx==0)
		{
			continue;
		}
		else
		{
			double gap=ptrBush->nodes[k].lgCost-ptrBush->nodes[k].sgCost;
			if(ingap<gap)
			{
				ingap=gap;
				//cout<<"the ingap is : "<<ingap<<endl;
				//system("PAUSE");
			}
		}
		// change the flow on the S1 and L1
		for(unsigned si=0;si<S1.size();si++)
		{
			ptrBush->linkFlow[S1[si]]=ptrBush->linkFlow[S1[si]] + dx;
		}
		for(unsigned li=0;li<L1.size();li++)
		{
			ptrBush->linkFlow[L1[li]]=ptrBush->linkFlow[L1[li]] - dx;
		}
		//update the flow and time of the links in SL
		///*
		for(unsigned sli=0;sli<SL.size();sli++)
		{
			int sllink=SL[sli];
			//update the O link flow
			double bbf=0.0;
			for(int bbi=0;bbi<bushNum;bbi++)
			{
				bbf+=ptrB0[bbi].linkFlow[sllink];
			}
			ods[b].OBLflow[sllink]=bbf;//update the O link
			//update the links flow
			double ttf=0;
			for(int oi=1;oi<=odNum;oi++)
			{
				ttf+=ods[oi].OBLflow[sllink];
			}
			links[sllink].flow=ttf;
			links[sllink].setCTime();
			links[sllink].setDTime();
		}
		//*/
		//update the u and U
		//renewBushTime(o,b);		
		//system("PAUSE");	
	}
	//system("PAUSE");
	return ingap;
}

//-------------------

//--------------cal Tree 3
void Network::calTree3(bush* ptrBush, int b)
{
	int orign=ods[b].orignID;
	double aveVot=ptrBush->aveVot;
	vector<double> sd(nodesNum+1,inf);
	vector<double> ld(nodesNum+1,0.0);
	sd[orign]=0.0;
	ld[orign]=0.0;

	for(int ti=0;ti<nodesNum;ti++)
	{
		int searchNode=ptrBush->topology[ti];

		for(unsigned m=0;m<ptrBush->nodes[searchNode].clinks.size() ;m++) // search every link which begin node is search node
		{
			int linkID=ptrBush->nodes[searchNode].clinks[m];
			int i=links[linkID].fromNode;// define the begin node of the link is i.
			int j=links[linkID].toNode;
			double tl=links[linkID].cTime*aveVot+links[linkID].cost;
			//------build the shortest tree
			if (sd[j] >= sd[i] +tl)//optimality condition: d(j)>d(i)+cij
			{
				sd[j] = sd[i]+tl;
				ptrBush->nodes[j].sgCost=sd[j];
				ptrBush->nodes[j].u=ptrBush->nodes[i].u+links[linkID].cTime;//update the u
				ptrBush->nodes[j].t=ptrBush->nodes[i].t+links[linkID].cost;//update the t
				ptrBush->nodes[j].p=linkID;  // record the previous link in the tree
				
			}//end of checking optimality condition

			//build the longest tree
			if (ld[j] <= ld[i] +tl)//optimality condition: d(j)>d(i)+cij
			{
				ld[j] = ld[i]+tl;
				ptrBush->nodes[j].lgCost=ld[j];
				ptrBush->nodes[j].U=ptrBush->nodes[i].U+links[linkID].cTime;
				ptrBush->nodes[j].T=ptrBush->nodes[i].T+links[linkID].cost;
				bushs[b].nodes[j].P=linkID;  // record the previous link in the tree
			}//end of checking optimality condition
			//}
		}// end of search link
		
	}// end search node

	
}//end cal tree


//--------------end tree 3
//----------cal the Topo
void Network::calTopo3(bush * ptrBush,int b )
{
	int orign=ods[b].orignID;
	//clear the topology before the cla
	ptrBush->topology.clear();
	for(int ni=0;ni<nodesNum;ni++)
	{
		ptrBush->topology.push_back(-1);
	}
	bush tBush=*ptrBush;
	deque<int> H;
	H.push_back(orign);
	int num=0;
	// search the H
	while(!H.empty())
	{
		int searchNode=H.front();
		H.pop_front();
		//delete the search node
		//push the searchNode into the topology of orignal bush
		ptrBush->topology[num]=searchNode;
	    num+=1;
		// search the out links from searchNode 
		while(!tBush.nodes[searchNode].clinks.empty())
		{
			int dlink=tBush.nodes[searchNode].clinks.front();//delete the link
			int j_node=links[dlink].toNode;
			tBush.nodes[searchNode].clinks.pop_front();
			//delete the dlink from the rlinks of j_node
			while(!tBush.nodes[j_node].rlinks.empty())
			{
				int mlink=tBush.nodes[j_node].rlinks.front();
				tBush.nodes[j_node].rlinks.pop_front();
				if(mlink==dlink)
				{
					break;
				}
				else
				{
					tBush.nodes[j_node].rlinks.push_back(mlink);
				}
				
			}
			//push the j_node into H if the j_node's rlinks is empty;
		    if(tBush.nodes[j_node].rlinks.empty())
			{
				H.push_back(j_node);
			}
			
		}
		

	}
	
}


//-----------end the Topo
//-----------Expand the Bush
int Network::expandBush3(bush * ptrBush)
{
	double aveVot=ptrBush->aveVot;
	//get the links not in the bush o
	vector<int> nonLinks;
	vector<int> p1;
	vector<int> p2;
	vector<int> p;
	for(int i=1;i<=linksNum;i++)
	{
		if(ptrBush->linkStatus[i]==0)
		{
			nonLinks.push_back(i);
		}
	}
	//get the p1
	for(int i=0;i<nonLinks.size();i++)
	{
		int searchLink=nonLinks[i];
		int i_node=links[searchLink].fromNode;
		int j_node=links[searchLink].toNode;
		if(ptrBush->nodes[i_node].sgCost+links[searchLink].cTime*aveVot+links[searchLink].cost<ptrBush->nodes[j_node].sgCost)
		{
			p1.push_back(searchLink);
		}
	}
	//get the p2
	for(int i=0;i<nonLinks.size();i++)
	{
		int searchLink=nonLinks[i];
		int i_node=links[searchLink].fromNode;
		int j_node=links[searchLink].toNode;
		if(ptrBush->nodes[i_node].lgCost+links[searchLink].cTime*aveVot+links[searchLink].cost<ptrBush->nodes[j_node].lgCost)
		{
			p2.push_back(searchLink);
		}
	}
	//get the intersection of p1 and p2
	for(int i=0;i<p1.size();i++)
	{
		for(int j=0;j<p2.size();j++)
		{
			if(p2[j]==p1[i])
			{
				p.push_back(p1[i]);
				break;
			}
		}
	}
	if(p.empty())
	{
		p=p2;
	}
	//set the return value : bush changed re=1;else re=0
	int re=1;
	if(p.empty())
	{
		re=0;
	}

	//-----add the p into the bush
	for(int i=0;i<p.size();i++)
	{
		int addLink=p[i];
		int i_node=links[addLink].fromNode;
		int j_node=links[addLink].toNode;
		ptrBush->linkStatus[addLink]=1;
		ptrBush->nodes[i_node].clinks.push_back(addLink);
		ptrBush->nodes[j_node].rlinks.push_back(addLink);

	}

	//----return re
	 return re;
}


//---------------------



//renew bush time and general cost 
void Network::reBushTime3(bush* ptrBush)
{
	double aveVot=ptrBush->aveVot;
	//set the u and U and t and T to 0
	for(int i=1;i<=nodesNum;i++)
	{
		ptrBush->nodes[i].U=0.0;
		ptrBush->nodes[i].u=0.0;
		ptrBush->nodes[i].T=0.0;
		ptrBush->nodes[i].t=0.0;
		ptrBush->nodes[i].sgCost=0.0;
		ptrBush->nodes[i].lgCost=0.0;
	}
	for(int i=1;i<=nodesNum;i++)
	{
		//cal the u t and sgcost
		int pred_p=ptrBush->nodes[i].p;//get the link before the i of shortest tree
		//int pred_p=bushs[b].nodes[i].p;
		while(pred_p)
		{
			ptrBush->nodes[i].u+=links[pred_p].cTime;
			ptrBush->nodes[i].t+=links[pred_p].cost;
			ptrBush->nodes[i].sgCost+=links[pred_p].cTime*aveVot+links[pred_p].cost;
	
			//bushs[b].nodes[i].u+=links[pred_p].cTime;
			int i_node=links[pred_p].fromNode;
			pred_p=ptrBush->nodes[i_node].p;
			//pred_p=bushs[b].nodes[i_node].p;
		}
		//cal the U
		int pred_P=ptrBush->nodes[i].P;//get the link before the i of the longest tree
		//int pred_P=bushs[b].nodes[i].P;
		while(pred_P)
		{
			ptrBush->nodes[i].U+=links[pred_P].cTime;
			ptrBush->nodes[i].T+=links[pred_P].cost;
			ptrBush->nodes[i].lgCost+=links[pred_P].cTime*aveVot+links[pred_P].cost;
			//bushs[b].nodes[i].U+=links[pred_P].cTime;
			int i_node=links[pred_P].fromNode;
			pred_P=ptrBush->nodes[i_node].P;
			//pred_P=bushs[b].nodes[i_node].P;
		}
	}
}


//-------------------
//put the data into this bush, assign the demand into this bush
void Network::creatBush(int b,bush* ptrBush,double a_ub, double a_lb,double* d_node,double* s_node)
{
	//cal the average VOT for this bush
	ptrBush->a_lb=a_lb;
	ptrBush->a_ub=a_ub;
	ptrBush->aveVot=(a_lb+a_ub)/2.0;
	//put the links into the bush
	for(int ni=1;ni<=nodesNum;ni++)
	{
		int temp=predLink[ni];
		ptrBush->nodes[ni].nodeID=ni;
		ptrBush->nodes[ni].U=s_node[ni];
		ptrBush->nodes[ni].u=s_node[ni];//longest time
		ptrBush->nodes[ni].T=d_node[ni];
		ptrBush->nodes[ni].t=d_node[ni];
		ptrBush->nodes[ni].P=predLink[ni];
		ptrBush->nodes[ni].p=predLink[ni];
		
		if(temp)
		{
			int id=links[temp].fromNode;
			ptrBush->nodes[id].clinks.push_back(temp);
			ptrBush->nodes[ni].rlinks.push_back(temp);
			ptrBush->linkStatus[temp]=1;
		}
	}
	//cout<<"before the assing "<<endl;
	//system("PAUSE");
	//assign the demand to the bush
	for(int j=ods[b].pairID;j<=ods[b].endPID;j++)//search all the commodity stemming from o
		{
			int destiNode=demands[j].destiID;
			//cout<<"The destiID is : "<<destiNode<<endl;

			//cal the flow between the a_ub and a_lb

			//---------------cal the dx and du
			double dx=0;
			double dm=demands[j].demand;

		    // Uniform
		    //dx=dm*0.01*(a_node[j_node]-a_lb);

		    //du=dm*0.5*0.01*(a_node[j_node]*a_node[j_node]-a_lb*a_lb);

		    //linear distribution
		    dx=dm*(1.0/3600.0)*(a_ub*a_ub-a_lb*a_lb);

			//cout<<"The aub and alb are :"<<a_ub<<setw(10)<<a_lb<<endl;
			//cout<<"The dx is : "<<dx<<endl;
			
			while(pred[destiNode]) //find all the links in the shortest route of commodity pair
			{
				int preNode=pred[destiNode];
				int preLink=predLink[destiNode];
				
		        
				ptrBush->linkFlow[preLink]+=dx;//assign the flow on the bush ods[i].orignID
				destiNode=preNode; // update the search Node ID
			} // end searching the link on the shortest path 
		} // end searching the shortest path stemming from o

	   

}



//----------------------


//shortest path function 3

void Network::shortpath(int b,double* ptr_gup,double* ptr_Dnode,double* ptr_Snode)//gup is the general utility;dnode is the cost of node ; snode is 
{                                                                                // cost of node
	int orign=ods[b].orignID;                                                                             //give the d_node and s_node for each link
	vector<double> d(nodesNum+1,inf);//general cost vector
	deque<int> Q;
	//initializa the variables
	for (int i=0;i<nodesNum+1;i++)
	{
		d[i]=inf;
        pred[i]=inf;
		predLink[i]=inf;
		ptr_Dnode[i]=0.0;
		ptr_Snode[i]=0.0;
	}
	d[orign]=0.0;
	pred[orign]=0;
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
			if (d[j] >= d[i] +t)//optimality condition: d(j)>d(i)+cij
			{
				int jexist=0;
				d[j] = d[i]+t;
				ptr_Dnode[j]=ptr_Dnode[i]+links[m].cost;
				ptr_Snode[j]=ptr_Snode[i]+links[m].cTime;
				pred[j]=i; //record the previous node in the tree
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
//min function
void Network::min(int b, double* d_node,double* s_node,double* ptr_alb,double* a)
{
	int o=ods[b].orignID;
	deque<int> a_list;
	vector<double> g_up(linksNum+1,0.0);
	
//----update the generalized cost for each link with respect to a_up
	for(int i=1;i<=linksNum;i++)
    {
		g_up[i]=links[i].cost+links[i].cTime*vot_max;
	}
	//----shortest path algorithm
	shortpath(b,&g_up[0],d_node,s_node);

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
				a[i]=vot_min;
			}
			else
			{
				a[i]=setP((d_node[jnode]-d_node[inode]-links[i].cost)/(s_node[inode]+links[i].cTime-s_node[jnode]));
			}
		}
	}
	//-------calculate the low bound of the a: *ptr_alb
	//-----set the *ptr_alb to zero
	*ptr_alb=vot_min;

	for(unsigned listi=0;listi<a_list.size();listi++)
	{
		if( (a[a_list[listi]]>=*ptr_alb) && ( a[a_list[listi]]<vot_max))
		{
			*ptr_alb=a[a_list[listi]];
		}
	}
}
//PTU
void Network::ptu(int b,double* d_node,double* s_node,double* a_up,double* ptr_alb,double* a)
{
	int o=ods[b].orignID;
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
				pred[jnode]=scanNode;
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
				a[i]=vot_min;
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
	*ptr_alb=vot_min;

	for( unsigned listi=0;listi<a_list.size();listi++)
	{
		if( (a[a_list[listi]]>=*ptr_alb) && ( a[a_list[listi]]<*a_up))
		{
			*ptr_alb=a[a_list[listi]];
		}
	}
}



//----------------LTA
void Network::lta(int b,double* x_node, double* u_node,double* x_link,double* u_link,double* a_node,double* d_x,double* d_u, int* ppredNode,int* ppredLink,double a_lb)
{
	int o=ods[b].orignID;
	for(int i=1;i<=nodesNum;i++)
	{
		d_x[i]=0.0;
		d_u[i]=0.0;
	}
	//initialize the LTA
	if( a_lb==vot_min)// set the D as the N and sort its topology
	{
		vector<int> topo(nodesNum+1,-1);
		topoSort(&topo[0],&pred[0]);
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
		for(int i=ods[b].pairID;i<=ods[b].endPID;i++)
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
		dx=dm*(1.0/3600.0)*(a_node[j_node]*a_node[j_node]-a_lb*a_lb);
		du=dm*(1.00/5400.0)*(pow(a_node[j_node],3)-pow(a_lb,3));

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

//----------------end of LTA

//----------------TopoSort used in LTA

void Network::topoSort(int* order,int* predNode)
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

//end the TopoSort used for LTA



//---------------MBA for ini3()
void Network::mpa(int b,double* xl,double* ul)//xl and xu is return value for x and u on link
{
	int o=ods[b].orignID;
	double a_ub=vot_max;
	double a_lb=-1;
	vector<double> d_node(nodesNum+1,0.0);
	vector<double> s_node(nodesNum+1,0.0);
	vector<double> x_node(nodesNum+1,0.0);//LTA variables
	vector<double> u_node(nodesNum+1,0.0);//LTA variables
	vector<double> x_link(linksNum+1,0.0);//LTA variables
	vector<double> u_link(linksNum+1,0.0);//LTA variables
	vector<double> a_node(nodesNum+1,vot_max);//LTA variables
	vector<double> d_x(nodesNum+1,0.0);//LTA network node labels
	vector<double> d_u(nodesNum+1,0.0);//LTA network node labels
	vector<double> a(linksNum+1,-1.0);//PTU variables
	vector<int> order(linksNum+1,-1);//toplSort variables
	vector<int> ppredNode(nodesNum+1,0);//LTA variables
	vector<int> ppredLink(linksNum+1,0);//LTA variables
	double pa_ub;
	double pa_lb;//LTA variables
	//initialize the shortest tree
	min(b,&d_node[0],&s_node[0],&a_lb,&a[0]);
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
			ppredNode[i]=pred[i];
			ppredLink[i]=predLink[i];
		}
		pa_ub=a_ub;
		pa_lb=a_lb;
		//----step1 : calculate the shortest paht according the a_ub
		
		ptu(b,&d_node[0],&s_node[0],&a_ub,&a_lb,&a[0]);
		
		/*
		cout<<"The a values are: "<<endl;
	for(int i=1;i<=linksNum;i++)
	{
		cout<<setw(10)<<a[i];
	}
	system("PAUSE");
		*/
		//-----step2: assign the demands

		
		lta(b,&x_node[0],&u_node[0],&x_link[0],&u_link[0],&a_node[0],&d_x[0],&d_u[0],&ppredNode[0],&ppredLink[0],pa_lb);
		
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
	while(a_lb!=vot_min);

	//assign the last tree
    lta(b,&x_node[0],&u_node[0],&x_link[0],&u_link[0],&a_node[0],&d_x[0],&d_u[0],&pred[0],&predLink[0],a_lb);
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


//--------------end of MBA



//ini for the algorithm 3

void Network::ini3()
{
	 vector<double> new_x(linksNum+1,0.0);
	 vector<double> new_u(linksNum+1,0.0);
	 for(int oi=1;oi<=odNum;oi++)
	{
		vector<double> xl(linksNum+1,0);//link u
		vector<double> ul(linksNum+1,0);//link x

		int o=ods[oi].orignID;
		mpa(oi,&xl[0],&ul[0]);
		for(int li=1;li<=linksNum;li++)
		{
			ods[oi].OBLflow[li]=xl[li];//put the origin based flow into the OBLflow;
			new_x[li]+=xl[li];
			new_u[li]+=ul[li];
		}
	}
	 //update the network data
	 for(int li=1;li<=linksNum;li++)
	 {
		 links[li].flow=new_x[li];
		 links[li].u=new_u[li];
		 links[li].setCTime();
		 links[li].setDTime();
	 }

}

//-----------------end of ini 3


//----------------mba 3 algorithm
void Network::mba3(double error)
{
	int Iter=0;
	system("PAUSE");
	//ini3();
	cout<<"The link flow before oba are: "<<endl;
		for(int li=1;li<=linksNum;li++)
		{
			cout<<li<<"--->"<<links[li].flow<<endl;
		}
		system("PAUSE");
	//system("PAUSE");
	double T1=0.0;
	double T2=0.0;
	double gap=1.0;
	while((Iter<200)&&(gap>error))
	{
		Iter+=1;
		T1=calObjective(&links[0]);
		
		
		for(int ooi=1;ooi<=odNum;ooi++)
		{
			oba(ooi);
		}
		cout<<"The link flow after oba are: "<<endl;
		for(int li=1;li<=linksNum;li++)
		{
			cout<<li<<"--->"<<links[li].flow<<endl;
		}
		//system("PAUSE");
		T2=calObjective(&links[0]);
		gap=abs(1-(T1/T2));
		cout<<"The Gap is : "<<gap;
		cout<<"This is the Iteration : "<<Iter<<endl;
		cout<<"The Objective is : "<<T2<<endl;
		system("PAUSE");
	}
	
	
	cout<<"The Objective is : "<<T2<<endl;
	cout<<"The gap is : "<<gap<<endl;

}



//-----------------end of mba3
// shortest path function

void Network::shortPath(int orign)
{
	
	vector<double> d(nodesNum+1,inf);
	deque<int> Q;
	//initializa the variables
	for (int i=0;i<nodesNum+1;i++)
	{
		travelTime[i]=inf;//shortest travel time equal to d
		d[i]=inf;
        pred[i]=inf;
		predLink[i]=inf;
	}
	travelTime[orign]=0.0;
	d[orign]=0.0;
	pred[orign]=0;
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
			double t=links[m].cTime;
			//denfine the end node of the link is j.
			if (d[j] > d[i] +t)//optimality condition: d(j)>d(i)+cij
			{
				int jexist=0;
				d[j] = d[i]+t;
				//u[j]=d[j];
				travelTime[j] = d[j];
				pred[j]=i; //record the previous node in the tree
				predLink[j]=m; // record the previous link in the tree
				//check if j has been in the Queue
				predLink;
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

//-----------short path

void Network::shortPathvot(int orign)//gup is the general utility;dnode is the cost of node ; snode is 
{                                                                                // cost of node
	                                                                             //give the d_node and s_node for each link
	vector<double> d(nodesNum+1,inf);//general cost vector
	deque<int> Q;
	//initializa the variables
	for (int i=0;i<nodesNum+1;i++)
	{
		d[i]=inf;
        pred[i]=inf;
		predLink[i]=inf;
		nodeCost[i]=0.0;
		nodeTime[i]=0.0;
	}
	d[orign]=0.0;
	pred[orign]=0;
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
			double t=links[m].gcost;
			//denfine the end node of the link is j.
			if (d[j] > d[i] +t)//optimality condition: d(j)>d(i)+cij
			{
				int jexist=0;
				d[j] = d[i]+t;
				nodeCost[j]=nodeCost[i]+links[m].cost;
				nodeTime[j]=nodeTime[i]+links[m].cTime;
				pred[j]=i; //record the previous node in the tree
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

//-------------initialize the BMA 
void Network::inibma()
{
	//----creat a bush for each origin and put the flow on the shortest path; the shortest and longest 
	// path tree were generated, links were added into the bush
	for(int i=1;i<=odNum;i++) //----search all the origns
	{
		shortPathvot(ods[i].orignID);//calculate the shortest path tree of the orign ods[i].orignID

		//-------assign the demands of each od from o to bushs
		for(int j=ods[i].pairID;j<=ods[i].endPID;j++)//search all the commodity stemming from o
		{
			int destiNode=demands[j].destiID;
			while(pred[destiNode]) //find all the links in the shortest route of commodity pair
			{
				int preNode=pred[destiNode];
				int preLink=predLink[destiNode];
				
				
				bushs[i].linkFlow[preLink]+=demands[j].demand;//assign the flow on the bush ods[i].orignID
				destiNode=preNode; // update the search Node ID
			} // end searching the link on the shortest path 
		} // end searching the shortest path stemming from o
		//
		for(int li=1;li<=linksNum;li++)
		{
			bushs[i].links[li].flowDis.push_back(bushs[i].linkFlow[li]);
			bushs[i].links[li].totalFlow=bushs[i].linkFlow[li];
			//put the upbound and low bound for all the links, if the link flow is larger than zero, then there is a bounds, else tow bounds are zero 
			if(bushs[i].linkFlow[li]>0)
			{
			bushs[i].links[li].votDis.push_back(vot_max);
			bushs[i].links[li].votDis.push_back(vot_min);
			}
			else
			{
				bushs[i].links[li].votDis.push_back(vot_max);
				bushs[i].links[li].votDis.push_back(vot_min);
			}
		}
		//add the bush node values
		for(int ni=1;ni<=nodesNum;ni++)
		{
			if(pred[ni])
			{
				int pli=predLink[ni];
				bushs[i].nodes[ni].votDis.push_back(vot_max);
				bushs[i].nodes[ni].votDis.push_back(vot_min);
				bushs[i].nodes[ni].flowDis.push_back(bushs[i].linkFlow[pli]);
			}
		}
		//---------creat the bush for bush i
		for(int n=1;n<=nodesNum;n++)
		{
				int temp=predLink[n];
				bushs[i].nodes[n].nodeID=n;
				//bushs[i].nodes[n].u=travelTime[n];//assing the shortest time to the bush
				//bushs[i].nodes[n].U=travelTime[n];
				bushs[i].nodes[n].aveToll=nodeCost[n];
				bushs[i].nodes[n].aveTime=nodeTime[n];
				bushs[i].nodes[n].p=predLink[n];
				bushs[i].nodes[n].P=predLink[n];
				if(temp)
				{
				   int id=links[temp].fromNode;
				   bushs[i].nodes[id].clinks.push_back(temp);//creat the clinks for node id
				   bushs[i].nodes[n].rlinks.push_back(temp);//creat the rlinks for node n

				   

				   bushs[i].linkStatus[temp]=1;
				}
		}//---end the initialization of the bush
		//cout<<"Bush "<<i<<"is generated"<<endl;
	} // end searching all the os
	//renew flow and time
	for(int li=1;li<=linksNum;li++)
	{
		double sumf=0.0;
		for(int oi=1;oi<=odNum;oi++)
		{
			sumf+=bushs[oi].linkFlow[li];
		}
		links[li].flow=sumf;
		links[li].setCTime();
		links[li].setDTime();
	}
	//update the bush
	for(int oi=1;oi<=odNum;oi++)
	{
		int b=oi;
		int o=ods[oi].orignID;
		renewBush2(o,b);
	}
	

	//add the links
	for(int oi=1;oi<=odNum;oi++)
	{
		int b=oi;
		int o=ods[oi].orignID;
		expandBush2(o,b);
	}


}

//expand the bush function
void Network::expandBush2(int o,int b)
{
	//define the variables
	vector<int> nonLinks;
	vector<int> p;
	//put the links not on bush o into nonLinks
	for(int li=1;li<=linksNum;li++)
	{
		if(bushs[b].linkStatus[li]==0)
		{
			nonLinks.push_back(li);
		}
	}
	
	
	//get the p
	for( unsigned ni=0;ni<nonLinks.size();ni++)
	{
		int sLink=nonLinks[ni];
		bushs[b].links[sLink].geCost=(vot_max/2)*links[sLink].cTime+links[sLink].cost;
		int iNode=links[sLink].fromNode;
		int jNode=links[sLink].toNode;
		if(bushs[b].nodes[iNode].geCost+bushs[b].links[sLink].geCost<bushs[b].nodes[jNode].geCost)
		{
			p.push_back(sLink);
		}
	}

	// add the p into the bush
	for(int ai=0;ai<p.size();ai++)
	{
		int addLink=p[ai];
		int i_node=links[addLink].fromNode;
		int j_node=links[addLink].toNode;
		bushs[b].linkStatus[addLink]=1;
		bushs[b].nodes[i_node].clinks.push_back(addLink);
		bushs[b].nodes[j_node].rlinks.push_back(addLink);
	}
}
//renew the multilclass
void Network::renewBush2(int o,int b)
{
	//set the average time and average cost for the node as zero
	for(int ni=1;ni<=nodesNum;ni++)
	{
		bushs[b].nodes[ni].aveToll=0.0;
		bushs[b].nodes[ni].aveTime=0.0;
		bushs[b].nodes[ni].nodeFlow=0.0;//set zero
	}
	//forword pass to calculate the average time and average cost
	deque<int> searchNode;
	searchNode.push_back(o);
	while(!searchNode.empty())
	{
		int sNode=searchNode.front();
		searchNode.pop_front();
		//cal the 
		if(sNode!=o)
		{
			double totalcost=0.0;
			double ttime=0.0;
			double ttoll=0.0;
			double totalflow=0.0;
			for(unsigned sni=0;sni<bushs[b].nodes[sNode].rlinks.size();sni++)
			{
				int sslink=bushs[b].nodes[sNode].rlinks[sni];
				totalcost+=bushs[b].links[sslink].aveGU*bushs[b].links[sslink].totalFlow;//avegu*linkflow
				ttime+=bushs[b].links[sslink].aveTime*bushs[b].links[sslink].totalFlow;
				ttoll+=bushs[b].links[sslink].aveToll*bushs[b].links[sslink].totalFlow;
				totalflow+=bushs[b].links[sslink].totalFlow;
			}
			bushs[b].nodes[sNode].tFlow=totalflow;//update the node total flow
			bushs[b].nodes[sNode].geCost=totalcost/totalflow;
			bushs[b].nodes[sNode].aveTime=ttime/totalflow;
			bushs[b].nodes[sNode].aveToll=ttoll/totalflow;

		}
		//search the out link for sNode
		for(unsigned sli=0;sli<bushs[b].nodes[sNode].clinks.size();sli++)
		{
			int sLink=bushs[b].nodes[sNode].clinks[sli];//search link
			int jNode=links[sLink].toNode;// the j Node of the sLink
			searchNode.push_back(jNode);//put the j Node into the searchNode
			//cal the geCost for the link sLink
			double upBound=bushs[b].links[sLink].votDis.front();
			double lowBound=bushs[b].links[sLink].votDis.back();
			double lf=bushs[b].links[sLink].totalFlow;
			double time=links[sLink].cTime;
			double cost=links[sLink].cost;
			if(lf>0)//update the nonzero link in the bush
			{
			//uniform distribution 
			double LF=lf/(0.01*(upBound-lowBound));//total flow on this link

			//cal the geCost for the link
			bushs[b].links[sLink].geCost=(time*0.005*(upBound*upBound-lowBound*lowBound)+cost*0.01*(upBound-lowBound))*LF/bushs[b].links[sLink].totalFlow;

			//cal the aveGU for the link
			bushs[b].links[sLink].aveGU=bushs[b].nodes[sNode].geCost+bushs[b].links[sLink].geCost;
			//cal the aveToll and aveTime
			bushs[b].links[sLink].aveTime=bushs[b].nodes[sNode].aveTime+links[sLink].cTime;
			bushs[b].links[sLink].aveToll=bushs[b].nodes[sNode].aveToll+links[sLink].cost;
			}
			else //update the zero link in the bush
			{
				bushs[b].links[sLink].geCost=(vot_max/2)*time+cost;
				bushs[b].links[sLink].aveGU=bushs[b].nodes[sNode].geCost+bushs[b].links[sLink].geCost;

				//cal the aveToll and aveTime
			    bushs[b].links[sLink].aveTime=bushs[b].nodes[sNode].aveTime+links[sLink].cTime;
			    bushs[b].links[sLink].aveToll=bushs[b].nodes[sNode].aveToll+links[sLink].cost;
			}
			
		}
	}
}

//-------------find the last pseudo  origin

int Network::lastPO(int o,int b,int d)
{
	bush sBush=bushs[b];
	deque<int> H;
	H.push_back(d);
	do
	{
		int sNode=H.front();
		H.pop_front();
		//search all the links into the sNode
		while(!sBush.nodes[sNode].rlinks.empty())
		{
			int dLink=sBush.nodes[sNode].rlinks.front();
			int iNode=links[dLink].fromNode;
			sBush.nodes[sNode].rlinks.pop_front();
			//put the iNode into H if iNode is not in the H
			int label=0;
			for(unsigned i=0;i<H.size();i++)
			{
				if(H[i]==iNode)
				{
					label=1;
				}
			}
			if(label==0)
			{
				H.push_back(iNode);
			}

		}
	}
	while (H.size()>1);

	return H.front();
}
//-------------equilibrium the bush
double Network::eqBush2(int o, int b)
{
	for(unsigned s=nodesNum-1;s>0;s--)
	{
		int d=bushs[b].topology[s];
		//find the last pseudo origin
		int lastO=lastPO(o,b,d);
		
		//eq the bush

	}
	return 0;
}
//-------------new multiclass assignment algorithm
void Network::BMA(double aMax,double error,int distri)
{
	int iter=0;
	int MaxIter=2000;
	double dGap=1.0;
	//ini the algorithm
	inibma();
	//begin the main iteration
	while((dGap>error)&&(iter<MaxIter))
	{
		//begin the ods loop
		for(int i=1;i<=odNum;i++)
		{
			int o=ods[i].orignID;
			int b=i;

			calTopo(o,b);

			renewBush2(o,b);

			//equilibrium the flow 


		}
	}
}

//-----------assign demands to the link
void Network::assignDemand()
{
	//---reset the variables
	for(int i=1;i<=linksNum;i++)
	{
		y[i]=0.0;
	}
	for(int i=1;i<=odNum;i++) //----search all the origns
	{
		shortPath(ods[i].orignID);//calculate the shortest path tree of the orign ods[i].orignID
		//-------assign the demands of each od from o to y
		for(int j=ods[i].pairID;j<=ods[i].endPID;j++)//search all the commodity stemming from o
		{
			int destiNode=demands[j].destiID;
			while(pred[destiNode]) //find all the links in the shortest route of commodity pair
			{
				int preNode=pred[destiNode];
				int preLink=predLink[destiNode];
				y[preLink]+=demands[j].demand; // add the demands on the link
				destiNode=preNode; // update the search Node ID
			} // end searching the link on the shortest path 
		} // end searching the shortest path stemming from o
	} // end searching all the os
	/*
	for(int i=1;i<=linksNum;i++)
	{
		cout<<y[i]<<setw(10);
	}
	system("PAUSE");
	*/
}
//-----------initialize the the x0
void Network::initialize()
{
	//assign demands according to the free flow 
	assignDemand();
	//--------assign the y to the x
	for(int i=1; i<=linksNum; i++)
	{
		links[i].flow=y[i]; // assign the y to links as the initialization 
		links[i].setCTime();// update the travel time on links
		links[i].setDTime();
		//cout<<links[i].flow<<setw(10);
	}
	//system("PAUSE");
}// end the function

//------calDerive

double Network::calDerive(double a,double *yl,Link *li)
{
	double derivative=0.0;
	for(int i=1;i<=linksNum;i++)
	{
		double gap=yl[i]-li[i].flow;
		double xal=li[i].flow+a*gap;
		derivative+=li[i].calTime(xal)*gap;
	}
	return derivative;
}
//------line search

double Network::biSearch(double *yll,Link *lik)//pass the pointer of vector y and vector links
{
	double a=0.0;
	double b=1.0;
	int b_iter=0;
	double alpha=(a+b)/2;
	double berr=0.01;
	double derive=calDerive(alpha,yll,lik);
	while( ((abs(derive)>berr) && ( b_iter>100))|| (derive>0))
	{
		derive=calDerive(alpha,yll,lik);
		if(derive>0)
		{
			b=alpha;
		}
		else if(derive<0)
		{
			a=alpha;
		}
		alpha=(a+b)/2;
		b_iter+=1;
	}
	return alpha;
}
//------calculate the total travel cost of a network
double Network::calTotalCost(Link *li)
{
	double totalCost=0.0;
	for(int i=1;i<=linksNum ;i++)
	{
		totalCost+=li[i].cTime*li[i].flow;
	}
	return totalCost;
}

//------calculate the objective function of the UE function
double Network::calObjective(Link* li)
{
	double objective=0.0;
	for(int i=1;i<=linksNum;i++)
	{
		double x=links[i].flow;
		double t0=li[i].calTime(0.0);
		double ta=t0*li[i].flow+((0.03*t0)/(pow(li[i].capacity,4)))*pow(li[i].flow,5);
		objective+=ta;
	}
	return objective;
}

//--------calculate the absolute gap
double Network::calabGap(double* yl, Link* li)
{
	double agap=0.0;
	for(int i=1;i<=linksNum;i++)
	{
		agap+=li[i].cTime*(yl[i]-li[i].flow);
	}
	return agap;
}

//give the larger value function

double Network::giveMAX(double a, double b)
{
	if( a>b) return a;
	else return b;
}
//-------------The Gradient Projection Algorithm Jayakrishnan_1994
void Network::GP(double error)
{
	int iter=0;
	double gap=1.0;
	//initialization
	inigp();
	updateGPi();
	/*
	for(int li=1;li<=linksNum;li++)
	{
		cout<<setw(10)<<li<<setw(10)<<links[li].flow<<setw(10)<<links[li].cTime<<endl;
	
	}
	system("PAUSE");

	for(int pi=1;pi<=pairNum;pi++)
	{
		int n=demands[pi].paths.size();
		cout<<setw(10)<<pi<<setw(10)<<n<<setw(10)<<demands[pi].spLabel<<setw(10)<<demands[pi].paths[n-1].pathFlow<<setw(10)<<demands[pi].paths[1].pathTime<<endl;
	}
	system("PAUSE");
	*/
	//------------------begin the cycle
	while((iter<1000)&&(gap>error))
	{

	    //----------step 1: direction finding 
		directF();
		updateGPi();
		///*
		for(int pi=1;pi<=10;pi++)
	    {
		int n=demands[pi].paths.size();
		cout<<setw(10)<<pi<<setw(10)<<n<<setw(10)<<demands[pi].spLabel;
		
		cout<<setw(10)<<endl;
		for(int si=1;si<n;si++)
		{
			cout<<" Path "<<si<<" : "<<endl;
			int d=demands[pi].paths[si].endNode;
			cout<<setw(10)<<demands[pi].paths[si].pathFlow<<setw(15)<<demands[pi].paths[si].pathTime<<setw(15)<<demands[pi].paths[si].pathDTime<<endl;
			while(demands[pi].paths[si].traceNode[d])
			{
				int pnode=demands[pi].paths[si].traceNode[d];
				int plink=demands[pi].paths[si].traceLink[d];
				cout<<d<<"<---";
				d=pnode;

			}

			cout<<d<<endl;
		}
		system("PAUSE");
	    }
		cout<<"After direction"<<endl;
	    system("PAUSE");
		//*/
		//---------step 2: Move
		moveGP();
		updateGP();
		///*
		for(int pi=1;pi<=10;pi++)
	    {
		int n=demands[pi].paths.size();
		cout<<setw(10)<<pi<<setw(10)<<n<<setw(10)<<demands[pi].spLabel;
		
		cout<<setw(10)<<endl;
		for(int si=1;si<n;si++)
		{
			cout<<" Path "<<si<<" : "<<endl;
			int d=demands[pi].paths[si].endNode;
			cout<<setw(15)<<demands[pi].paths[si].pathFlow<<setw(15)<<demands[pi].paths[si].pathTime<<setw(15)<<demands[pi].paths[si].pathDTime<<endl;
			while(demands[pi].paths[si].traceNode[d])
			{
				int pnode=demands[pi].paths[si].traceNode[d];
				int plink=demands[pi].paths[si].traceLink[d];
				cout<<d<<"<---";
				d=pnode;

			}

			cout<<d<<endl;
		}
		system("PAUSE");
	    }
		cout<<"After move"<<endl;
	    system("PAUSE");
		//*/
		//-------- step 3: delete the 0 path
		
		//---------step 4: update
		
		//deletePath();
		//--------  step 5: convergence test
		gap=converge();
		

	    iter+=1;
		//----------
		
		cout<<"This is the "<<iter<<" iterations"<<endl;
		
		cout<<"The gap is : "<<gap<<endl;
		system("PAUSE");
	}

	double objective = calObjective(&links[0]);
	double total=calTotalCost(&links[0]);
	cout<<"The Objective is : "<<objective<<endl;
	cout<<"The Total Cost is : "<<total<<endl;
}
//-----------delete path with 0 flow
void Network::deletePath()
{
	for(int pi=1;pi<=pairNum;pi++)
	{
		vector<path>::iterator iter=demands[pi].paths.begin()+1;
		while(iter!=demands[pi].paths.end())
		{
			if((*iter).pathFlow==0.0)
			{
				iter=demands[pi].paths.erase(iter);
			}
			else
			{
				iter++;
			}

		}
	}
}
//-----------converge test
double Network::converge()
{
	double T1=0.0;
	for(int di=1;di<=pairNum;di++)
	{
		int shID=demands[di].spLabel;
		T1+=demands[di].paths[shID].pathTime*demands[di].demand;
	}
	double T2=0.0;
	for(int li=1;li<=linksNum;li++)
	{
		T2+=links[li].cTime*links[li].flow;
	}
	double T3=abs(1-T1/T2);
	return T3;
}
//-----------move of GP
void Network::moveGP()
{
	
	for(int oi=1;oi<=odNum;oi++)
	{
		for(int oj=ods[oi].pairID;oj<=ods[oi].endPID;oj++)
		{
			int shID=demands[oj].spLabel;
			path shPath=demands[oj].paths[shID];
			//fine the shortest path link
			
			double snPathFlow=0.0;
			//-----------compute the second derivative
			for(int pi=1;pi<demands[oj].paths.size();pi++)
			{
				if(pi!=shID)
				{
					vector<int> sPathLink;//short path link
			        int sDestNode=demands[oj].paths[shID].endNode;
		            while(demands[oj].paths[shID].traceNode[sDestNode])
			        {
				        int prNode=demands[oj].paths[shID].traceNode[sDestNode];
				        int prLink=demands[oj].paths[shID].traceLink[sDestNode];
				        sPathLink.push_back(prLink);
				        sDestNode=prNode;
			        }
					cout<<sPathLink.size()<<endl;
					vector<int> nPathLink;//non short path link
					
					int nDestNode=demands[oj].paths[pi].endNode;
					
					//find the non shortest path link
					while(demands[oj].paths[pi].traceNode[nDestNode])
					{
						int prNode=demands[oj].paths[pi].traceNode[nDestNode];
						int prLink=demands[oj].paths[pi].traceLink[nDestNode];
						nPathLink.push_back(prLink);
						nDestNode=prNode;
					}
					cout<<nPathLink.size()<<endl;
					//find the links on either k or ks, but not on both
					
					vector<int> comPathLink;
					//put the n short path link into comPathLink
					for(int vi=0;vi<nPathLink.size();vi++)
					{
						int lab=0;
						for(int vj=0;vj<sPathLink.size();vj++)
						{
							if(nPathLink[vi]==sPathLink[vj])
							{
								lab=1;

							}
						}
						if(lab==0)
						{
							comPathLink.push_back(nPathLink[vi]);
						}
					}
				
					//put the s shrot path lin into comPathLink
					for(int vi=0;vi<sPathLink.size();vi++)
					{
						int lab=0;
						for(int vj=0;vj<nPathLink.size();vj++)
						{
							if(sPathLink[vi]==nPathLink[vj])
							{
								lab=1;

							}
						}
						if(lab==0)
						{
							comPathLink.push_back(sPathLink[vi]);
						}
					}
					//cal the second derivative for the path pi
					demands[oj].paths[pi].pathDTime=0.0;
					cout<<"The O is : "<<oi<<endl;
					cout<<"The pair is: "<<oj<<endl;
					cout<<"The size of com is : "<<comPathLink.size()<<endl;
					for(int vi=0;vi<comPathLink.size();vi++)
					{
						int sslink=comPathLink[vi];
						demands[oj].paths[pi].pathDTime+=links[sslink].dTime;
					}//-----------here we get the sk for this path pi
					cout<<"The path D time is: ";
					cout<<demands[oj].paths[pi].pathDTime<<endl;
					
					//set the new flow for this path pi
					//---compute the new flow
					double nPFlow=0.0;
					double ts=demands[oj].paths[shID].pathTime;
					double tn=demands[oj].paths[pi].pathTime;
					double dt=tn-ts;
					double dx=0.8*dt/demands[oj].paths[pi].pathDTime;
					cout<<"The ts is: "<<ts;
					cout<<"The tn is: "<<tn;
					cout<<"The dx is : "<<dx<<endl;
					cout<<"The dt is : "<<dt<<endl;
					nPFlow=demands[oj].paths[pi].pathFlow-dx;
					if(nPFlow<0.0)
					{
						nPFlow=0.0;
					}
					//----set the path flow as nPFlow
					demands[oj].paths[pi].pathFlow=nPFlow;
					snPathFlow+=nPFlow;
					//system("PAUSE");
				}
			}

			//---assign the shortest path flow 
			demands[oj].paths[shID].pathFlow=demands[oj].demand-snPathFlow;


		}
	}
}
double Network::setP(double fl)
{
	double b;
	b=(int)((fl*1000))/1000.0;
	return b;
}
//-----------direction finding for the gp
void Network::directF()
{
	for(int oi=1;oi<=odNum;oi++)
	{
		int origin = ods[oi].orignID;
		shortPath(origin);
		//-------add new path if the new shortest path is different from the existed path
		for(int j=ods[oi].pairID;j<=ods[oi].endPID;j++)//search all the commodity stemming from o
		{
			int destiNode=demands[j].destiID;
			double shTime=travelTime[destiNode];
			//cout<<"travelTime is : "<<shTime<<endl;
			int slabel=0;
			for(int pi=1;pi<demands[j].paths.size();pi++)
			{
				if(setP(demands[j].paths[pi].pathTime)==setP(shTime))
				{
					demands[j].spLabel=pi;
					slabel=1;
				}
			}
			if(slabel==0)
			{
			    path newPath=demands[j].paths[0];
			    newPath.beginNode=origin;
			    newPath.endNode=destiNode;
			
			    while(pred[destiNode]) //find all the links in the shortest route of commodity pair
			    {
				    int preNode=pred[destiNode];
				    int preLink=predLink[destiNode];
 
				    //-----assign the trip to the path
				    newPath.traceLink[destiNode]=preLink;
				    newPath.traceNode[destiNode]=preNode;//record this path using node

				    destiNode=preNode; // update the search Node ID

				
			    } // end searching the link on the shortest path 

			    demands[j].paths.push_back(newPath); //put the new Path into the paths
				demands[j].spLabel=demands[j].paths.size()-1;
			}

		} // end searching the shortest path stemming from o
		
	}
}
//------------update the gp
void Network::updateGP()
{
	vector<double> lFlow(linksNum+1,0.0);
	//update the link flow and link time and d time from the path
	for(int oi=1;oi<=odNum;oi++)
	{
		for(int oj=ods[oi].pairID;oj<=ods[oi].endPID;oj++)
		{
			for(int pi=1;pi<demands[oj].paths.size();pi++)
			{
				
				int deNode=demands[pi].paths[pi].endNode;
				while(demands[oj].paths[pi].traceNode[deNode])
				{
					int preNode=demands[oj].paths[pi].traceNode[deNode];
					int preLink=demands[oj].paths[pi].traceLink[deNode];
					lFlow[preLink]+=demands[oj].paths[pi].pathFlow;
					deNode=preNode;
				}
			}
		}
	}
	//update
	for(int li=1;li<=linksNum;li++)
	{
		links[li].flow=lFlow[li];
		links[li].setCTime();
		links[li].setDTime();
	}
	//set the path time
	for(int di=1;di<=pairNum;di++)
	{
		for(int dj=1;dj<demands[di].paths.size();dj++)
		{
			//set the cost as 0 first
			demands[di].paths[dj].pathTime=0.0;
			int desiNode=demands[di].paths[dj].endNode;
			while(demands[di].paths[dj].traceNode[desiNode])
			{
				int pNode=demands[di].paths[dj].traceNode[desiNode];
				int pLink=demands[di].paths[dj].traceLink[desiNode];
				demands[di].paths[dj].pathTime+=links[pLink].cTime;
				desiNode=pNode;
			}
		}
	}
}
//------------update the gpi
void Network::updateGPi()
{
	
	//set the path time
	for(int di=1;di<=pairNum;di++)
	{
		for(int dj=1;dj<demands[di].paths.size();dj++)
		{
			//set the cost as 0 first
			demands[di].paths[dj].pathTime=0.0;
			int desiNode=demands[di].paths[dj].endNode;
			while(demands[di].paths[dj].traceNode[desiNode])
			{
				int pNode=demands[di].paths[dj].traceNode[desiNode];
				int pLink=demands[di].paths[dj].traceLink[desiNode];
				demands[di].paths[dj].pathTime+=links[pLink].cTime;
				desiNode=pNode;
			}
		}
	}
}
//-------------initialization of GP algorithm
void Network::inigp()
{
	
	for(int oi=1;oi<=odNum;oi++)
	{
		int origin = ods[oi].orignID;
		shortPath(origin);
		//-------assign the demands of each od from o to y
		for(int j=ods[oi].pairID;j<=ods[oi].endPID;j++)//search all the commodity stemming from o
		{
			int destiNode=demands[j].destiID;
			path newPath;
			newPath=demands[j].paths[0];
			newPath.beginNode=origin;
			newPath.endNode=destiNode;
			newPath.pathFlow=demands[j].demand;
			
			while(pred[destiNode]) //find all the links in the shortest route of commodity pair
			{
				int preNode=pred[destiNode];
				int preLink=predLink[destiNode];
				y[preLink]+=demands[j].demand; // add the demands on the link

				//-----assign the trip to the path
				newPath.traceLink[destiNode]=preLink;
				newPath.traceNode[destiNode]=preNode;//record this path using node

				destiNode=preNode; // update the search Node ID

				
			} // end searching the link on the shortest path 

			demands[j].paths.push_back(newPath); //put the new Path into the paths

		} // end searching the shortest path stemming from o
		
	}

	//set the flow and time and Dtime
	for(int li=1;li<=linksNum;li++)
	{
		links[li].flow=y[li];
		links[li].setCTime();
		links[li].setDTime();
	}
	
}
//------the Frank Wolfe UE algorithm

void Network::fw()
{
	double relGap=1.0;
	double abGap=0.0;
	double cerr=0.000001;//define the convergence precision
	double fw_alpha;
	double LBD=0.1;// define the LBD, see p97 of (Patriksson,1994)
	int iter=0;
	int maxIter=5000;
	double T=0.0;//define the previous objective function
	double T1=0.0; //define the new objective function
	double TL=0.0; // define the linear function ,as well as the lower bound
	double TT=0.0; // define the total travel time of the network

	//-------initialization
	initialize();
	//---begin the algorithm
	while( (relGap>cerr) && (iter<maxIter))
	{
		//--------------find the direction y
		assignDemand();

		//--------calculate the T Gap and TL
		
		T=calObjective(&links[0]);
		abGap=calabGap(&y[0],&links[0]);
        TL=T+abGap;
		//LBD=giveMAX(abs(LBD),abs(TL));
		//relGap=(T-LBD)/(LBD);
		//cout<<T<<setw(10)<<abGap<<setw(10)<<TL<<setw(10)<<relGap<<setw(10);
		//system("PAUSE");
		//if (relGap<cerr) break;
		
		//-------line search
		fw_alpha=biSearch(&y[0],&links[0]);

		//-------update the link flow and travel time 
		for(int i=1;i<=linksNum;i++)
		{
			links[i].flow=links[i].flow+fw_alpha*(y[i]-links[i].flow);
			links[i].setCTime();
		}

		//-------update the relGap again
		T1=calObjective(&links[0]);//calculate the objective function again
		TT=calTotalCost(&links[0]);
		relGap=abs(abGap/TT);
		iter+=1;
		cout<<iter<<setw(10)<<"The relative gap is: "<<relGap<<endl;

	}//end the algorithm
	//---calculate the total travel time
	
	//----printf the results
	cout<<"\n";
	cout<<"The Total Travel Time of the network is: "<<TT<<endl;
	cout<<"The Objective of the function is: "<<T1<<endl;
	cout<<"The Lower bound is: "<<TL<<endl;
	cout<<"The relative gap is: "<<relGap<<endl;
}

//----------FWP algorithm-----------(Leblanc,1985);(Arezki,1990)

void Network::fwp()
{
	double relGap=1.0;
	double abGap=0.0;
	double cerr=10e-6;//define the convergence precision
	double fwp_t=0.0; // define the fw step length
	double fwp_l=0.0; // define the Partan step length
	double LBD=0.1;// define the LBD, see p97 of (Patriksson,1994)
	int iter=0;
	int maxIter=20000;
	double T=0.0;//define the previous objective function
	double T1=0.0; //define the new objective function
	double TL=0.0; // define the linear function ,as well as the lower bound
	double TT=0.0; // define the total travel time of the network
	double pre_t=0.0;
	double pre_l=0.0;
	double l_min=0.0;
	double pre_lmin=0.0;

	//-------initialization
	initialize();


	//-----step 1.1: solve fw subproblem and get the y,define the x0
	assignDemand();

	//-----step 1.2: define x1
	fwp_t=biSearch(&y[0],&links[0]);

	for(int i=1;i<=linksNum;i++)
	{
		pre_x[i]=links[i].flow;//give the x0 to pre_x
		v[i]=links[i].flow+fwp_t*(y[i]-links[i].flow);
		links[i].flow=v[i];//get the x1
		links[i].setCTime();//update the travel time for each link
	}
	iter+=1;//set k to 1;
	//-----begin the iterative algorithm
	while( (relGap>cerr)&&(iter<maxIter) )
	{

		pre_t=fwp_t;
		pre_l=fwp_l;
		pre_lmin=l_min;
		//----- step k.1: fw line search and find the y
		assignDemand();

		//--------calculate the T Gap and TL
		
		T=calObjective(&links[0]);
		abGap=calabGap(&y[0],&links[0]);
        TL=T+abGap;

		//------step k.2: find the v
		fwp_t=biSearch(&y[0],&links[0]);

		for(int i=1;i<=linksNum;i++)
	   {
		   v[i]=links[i].flow+fwp_t*(y[i]-links[i].flow);
	   }
		//-----step k.3:
		//find the min fwp_l, see paper (Arezki,1990)
		if(pre_l<0)
		{
			l_min=1+1/( (1-pre_t)*fwp_t*(1-pre_l/pre_lmin)-1);
		}
		else
		{
			l_min=1+1/( (1-pre_t)*fwp_t*(1-pre_l)-1);

		}

		// do the Partan line search
		fwp_l=plSearch(l_min,&v[0],&pre_x[0]);


		// update the flow and pre_x
		for(int i=1;i<=linksNum;i++)
		{
			double temp;
			temp=links[i].flow;
			links[i].flow=v[i]+fwp_l*(pre_x[i]-v[i]);
			links[i].setCTime();//update the travel time 
			pre_x[i]=temp;
		}
		
		//-------update the relGap again
		T1=calObjective(&links[0]);//calculate the objective function again
		TT=calTotalCost(&links[0]);
		relGap=abs(abGap/TT);
		iter+=1;

		cout<<iter<<setw(10);
	}//end the iterative

	//----printf the results
	cout<<"\n";
	cout<<"The Total Travel Time of the network is: "<<TT<<endl;
	cout<<"The Objective of the function is: "<<T1<<endl;
	cout<<"The Lower bound is: "<<TL<<endl;
	cout<<"The relative gap is: "<<relGap<<endl;


}

// Dial's algorithm : (Dial,2006), (Nie,2010)
void Network::dba(double error)
{
	//define the parameters
	double derr=error;
	double dgap=1.0;
	int iter=0;
	int maxIter=2000;
	// initialization
	inidba();
	//begin the main iteration
	while((dgap>derr)&&(iter<maxIter))
	{
		//begin the ods loop
		for(int i=1;i<=odNum;i++)
		{
			int o=ods[i].orignID;
			int b=i;
			//cout<<"The O is now: "<<ods[i].orignID<<endl;
			double zeroNum=0;
			int inIter=0;
			while((zeroNum<3)&&(inIter<4))
			{
				double dif=1;
			    //step 1: cal topology for the bush
			    calTopo(o,b);
			    //step 2: cal the tree
				int inum=0;
				while((dif>0.1)&&(inum<4))
				{
			        calTree(o,b);
			
			        //step 3: equlibrium the bush
			        dif=eqBush(o,b);
			        if(dif<10e-3)
				    {
					    zeroNum+=1;
				    }
				    //
				    renewFlow();
					inum+=1;
				}

				//reduce the bush
				reduceBush(o,b);
				
				//refind the tree
				calTopo(o,b);
				
				calTree(o,b);
				//renewBushTime(o,b);

				//expand the tree
				expandBush(o,b);
				
				
				inIter+=1;
				//cout<<"The inIter now is: "<<inIter<<endl;
			}
			//calTree(o,b);
			//cout<<"Bush "<<i<<" is adjusted"<<endl;
		}
		//Stage Four: convergence
		double T3=0.0;
		double T4=0.0;
		double T5=0.0;
		for(int oi=1;oi<=odNum;oi++)
		{
			//calTree(ods[oi].orignID,oi);
			int begin_pair=ods[oi].pairID;
			int end_pair=ods[oi].endPID;
			for(int pj=begin_pair;pj<=end_pair;pj++)
			{
				int pdid=demands[pj].destiID;
				double drs=demands[pj].demand;
				double urs=bushs[oi].nodes[pdid].u;
				T3=T3+drs*urs;
			}
		}
		
		cout<<endl<<"T3 is: "<<T3;
		//system("PAUSE");
		for(int li=1;li<=linksNum;li++)
		{
			double xij=links[li].flow;
			double tij=links[li].cTime;
			T4+=xij*tij;
		}
		//cal  objective
		T5=calObjective(&links[0]);
		cout<<endl<<"The Total Cost is: "<<T4<<setprecision(16)<<endl;
		cout<<endl<<"The Objective is: "<<T5<<endl;
		//system("PAUSE");
		//cal dgap: the main stage convergence gap
		dgap=abs(1-(T3/T4));
		cout<<endl<<"The dgap is: "<<setprecision(16)<<dgap;
		iter+=1;
		cout<<"main: "<<iter<<endl;
		//system("PAUSE");
	}



}

//void Network::dba(double error)
//{
//	//define the parameters
//	double derr=error;
//	double dgap=1.0;
//	int iter=0;
//	int maxIter=2000;
//	// initialization
//	inidba();
//	//begin the main iteration
//	while((dgap>derr)&&(iter<maxIter))
//	{
//		//begin the ods loop
//		for(int i=1;i<=odNum;i++)
//		{
//			int o=ods[i].orignID;
//			int b=i;
//			//cout<<"The O is now: "<<ods[i].orignID<<endl;
//			double zeroNum=0;
//			int inIter=0;
//			while((zeroNum<3)&&(inIter<50))
//			{
//				double dif=1;
//			    //step 1: cal topology for the bush
//			    calTopo(o,b);
//			    //step 2: cal the tree
//				int inum=0;
//				while((dif>0.1)&&(inum<20))
//				{
//			        calTree(o,b);
//			
//			        //step 3: equlibrium the bush
//			        dif=eqBush(o,b,iter);
//			        if(dif<10e-3)
//				    {
//					    zeroNum+=1;
//				    }
//				    //
//				   // renewFlow();
//					inum+=1;
//					//cout<<"The inum is: "<<inum<<endl;
//					//system("PAUSE");
//				}
//
//				//reduce the bush
//				reduceBush(o,b);
//				
//				//refind the tree
//				calTopo(o,b);
//				
//				calTree(o,b);
//				//renewBushTime(o,b);
//
//				//expand the tree
//				expandBush(o,b);
//				
//				
//				inIter+=1;
//				//cout<<"The inIter now is: "<<inIter<<endl;
//				//system("PAUSE");
//			}
//			//cout<<"Bush "<<i<<" is adjusted"<<endl;
//		}
//		//Stage Four: convergence
//		double T3=0.0;
//		double T4=0.0;
//		double T5=0.0;
//		for(int oi=1;oi<=odNum;oi++)
//		{
//			int begin_pair=ods[oi].pairID;
//			int end_pair=ods[oi].endPID;
//			for(int pj=begin_pair;pj<=end_pair;pj++)
//			{
//				int pdid=demands[pj].destiID;
//				double drs=demands[pj].demand;
//				double urs=bushs[oi].nodes[pdid].u;
//				T3=T3+drs*urs;
//			}
//		}
//		/*
//		for(int oi=1;oi<=pairNum;oi++)
//		{
//			
//			int poid=demands[oi].orignID;
//			int pdid=demands[oi].destiID;
//			double drs=demands[oi].demand;
//			int b=poid-ods[1].orignID+1;
//			double urs=bushs[b].nodes[pdid].u;
//			T3=T3+drs*urs;
//			T3;
//		}
//		*/
//		cout<<endl<<"T3 is: "<<T3;
//		//system("PAUSE");
//		for(int li=1;li<=linksNum;li++)
//		{
//			double xij=links[li].flow;
//			double tij=links[li].cTime;
//			T4+=xij*tij;
//		}
//		//cal  objective
//		T5=calObjective(&links[0]);
//		cout<<endl<<"The Total Cost is: "<<T4<<setprecision(16)<<endl;
//		cout<<endl<<"The Objective is: "<<T5<<endl;
//		//system("PAUSE");
//		//cal dgap: the main stage convergence gap
//		dgap=abs(1-(T3/T4));
//		cout<<endl<<"The dgap is: "<<setprecision(16)<<dgap;
//		iter+=1;
//		cout<<"main: "<<iter<<endl;
//		//system("PAUSE");
//	}
//
//
//
//}
//----------reduce bush
void Network::reduceBush(int o,int b)
{
	for(int i=1;i<=linksNum;i++)
	{
		int i_node=links[i].fromNode;
		int j_node=links[i].toNode;
		if((bushs[b].linkStatus[i]==1)&&(bushs[b].linkFlow[i]==0)&&(bushs[b].nodes[j_node].rlinks.size()>1))
		{
			//delete the link
			bushs[b].linkStatus[i]=0;
			//delete the link from the clinks
			while(!bushs[b].nodes[i_node].clinks.empty())
			{
				int dLink=bushs[b].nodes[i_node].clinks.front();
				bushs[b].nodes[i_node].clinks.pop_front();
				if(dLink==i)
				{
					break;
				}
				else
				{
					bushs[b].nodes[i_node].clinks.push_back(dLink);
				}
			}
			//delete the link from the rlink
			while(!bushs[b].nodes[j_node].rlinks.empty())
			{
				int dLink=bushs[b].nodes[j_node].rlinks.front();
				bushs[b].nodes[j_node].rlinks.pop_front();
				if(dLink==i)
				{
					break;
				}
				else
				{
					bushs[b].nodes[j_node].rlinks.push_back(dLink);
				}
			}
		}
	}
}
// cal topology

void Network::calTopo(int orign,int b)
{
	//clear the topology before the cla
	bushs[b].topology.clear();
	bush tBush=bushs[b];
	deque<int> H;
	H.push_back(orign);
	// search the H
	while(!H.empty())
	{
		int searchNode=H.front();
		H.pop_front();
		//delete the search node
		//push the searchNode into the topology of orignal bush
		bushs[b].topology.push_back(searchNode);
		// search the out links from searchNode 
		while(!tBush.nodes[searchNode].clinks.empty())
		{
			int dlink=tBush.nodes[searchNode].clinks.front();//delete the link
			int j_node=links[dlink].toNode;
			tBush.nodes[searchNode].clinks.pop_front();
			//delete the dlink from the rlinks of j_node
			while(!tBush.nodes[j_node].rlinks.empty())
			{
				int mlink=tBush.nodes[j_node].rlinks.front();
				tBush.nodes[j_node].rlinks.pop_front();
				if(mlink==dlink)
				{
					break;
				}
				else
				{
					tBush.nodes[j_node].rlinks.push_back(mlink);
				}
				
			}
			//push the j_node into H if the j_node's rlinks is empty;
		    if(tBush.nodes[j_node].rlinks.empty())
			{
				H.push_back(j_node);
			}
			
		}
		

	}
	
}

//----------equilibrium the bush function

double Network::eqBush(int o,int b)
{
	double ingap=0;
	for(int s=nodesNum-1;s>0;s--)//---search every node according to the decending pass
	{
		deque<int> S;
		deque<int> L;		
		//find the shortest path links and put them into a vector S
		int k=bushs[b].topology[s];// define the k as the s decending element 
		while(bushs[b].nodes[k].p)
		{
			int te=bushs[b].nodes[k].p;
			int teNode=links[te].fromNode;
			S.push_back(te);
			k=teNode;

		}
		//find the longest path links and put them into a vector L
		k=bushs[b].topology[s];
		while(bushs[b].nodes[k].P)
		{
			int te=bushs[b].nodes[k].P;
			int teNode=links[te].fromNode;
			L.push_back(te);
			k=teNode;

		}			
		// if i==j, then jump to next j
		if(S.front()==L.front()) continue;
	    //find the first set of different element
		deque<int> S1;
		deque<int> L1;
		int mernode=0;
		for(unsigned sfi=0;sfi<S.size();sfi++)
		{
			int sfnode=links[S[sfi]].fromNode;
			int label=0;
			for(unsigned lfi=0;lfi<L.size();lfi++)
			{
				if(links[L[lfi]].fromNode==sfnode)
				{
					label=1;
					mernode=sfnode;
					break;
				}
			}
			if(label) break;
		  }
		// find the S1 and L1
		int senode=links[L.front()].fromNode;
		do
		{
			int sflink=S.front();
			S1.push_back(sflink);
			S.pop_front();
			S.push_back(sflink);
		    senode=links[sflink].fromNode;
			//senode=links[S.front()].fromNode;
		}
		while(senode!=mernode);

		int lenode=links[L.front()].fromNode;
		do
		{
			int lflink=L.front();
			L1.push_back(lflink);
			L.pop_front();
			L.push_back(lflink);
			lenode=links[lflink].fromNode;
		}
		while(lenode!=mernode);
				
		// cal the combine set of S1 and L1
		deque<int> SL(S1);
		for(unsigned li=0;li<L1.size();li++)
		{
			SL.push_back(L1[li]);
		}

		//calculate the g,h and dx : see ( Yu Nie, 2010)
		//cal g
				
		double gl=0.0;
		double gs=0.0;
		for(unsigned l1=0;l1<L1.size();l1++)
		{
			gl+=links[L1[l1]].cTime;
		}
		for(unsigned s1=0;s1<S1.size();s1++)
		{
			gs+=links[S1[s1]].cTime;
		}
		double g=gl-gs;
		
		//cal h
		double h=0.0;
		for(unsigned sli=0;sli<SL.size();sli++)//cal h
		{
			h+=links[SL[sli]].dTime;
		}
		//find the smallest flow in the L1 of bush
		double slf=bushs[b].linkFlow[L1[0]];//define the smallest flow in the L
		for(unsigned li=0;li<L1.size();li++)
		{
			if(slf>=bushs[b].linkFlow[L1[li]])
			{
				slf=bushs[b].linkFlow[L1[li]];
			}
		}
		//cal dx
		double lamda=0.8;
		double dx=lamda*(g/h);
		if(dx>slf) dx=slf;
		k=bushs[b].topology[s];
		if(dx==0)
		{
			continue;
		}
		else
		{
			double gap=bushs[b].nodes[k].U-bushs[b].nodes[k].u;
			if(ingap<gap)
			{
				ingap=gap;
				//cout<<"the ingap is : "<<ingap<<endl;
				//system("PAUSE");
			}
		}
		// change the flow on the S1 and L1
		for(unsigned si=0;si<S1.size();si++)
		{
			bushs[b].linkFlow[S1[si]]=bushs[b].linkFlow[S1[si]] + dx;
		}
		for(unsigned li=0;li<L1.size();li++)
		{
			bushs[b].linkFlow[L1[li]]=bushs[b].linkFlow[L1[li]] - dx;
		}
		//update the flow
		//renewFlow();
		/*for(unsigned sli=0;sli<SL.size();sli++)
		{
			int sllink=SL[sli];
			double ttf=0;
			for(int oi=1;oi<=odNum;oi++)
			{
				ttf+=bushs[oi].linkFlow[sllink];
			}
			links[sllink].flow=ttf;
			links[sllink].setCTime();
			links[sllink].setDTime();
		}*/

	}
	return ingap;
}
//double Network::eqBush(int o,int b,int iter)
//{
//	double ingap=0;
//	for(int s=nodesNum-1;s>0;s--)//---search every node according to the decending pass
//	{
//		deque<int> S;
//		deque<int> L;		
//		//find the shortest path links and put them into a vector S
//		int k=bushs[b].topology[s];// define the k as the s decending element 
//		while(bushs[b].nodes[k].p)
//		{
//			int te=bushs[b].nodes[k].p;
//			int teNode=links[te].fromNode;
//			S.push_back(te);
//			k=teNode;
//
//		}
//		//find the longest path links and put them into a vector L
//		k=bushs[b].topology[s];
//		while(bushs[b].nodes[k].P)
//		{
//			int te=bushs[b].nodes[k].P;
//			int teNode=links[te].fromNode;
//			L.push_back(te);
//			k=teNode;
//
//		}			
//		// if i==j, then jump to next j
//		if(S.front()==L.front()) continue;
//	    //find the first set of different element
//		deque<int> S1;
//		deque<int> L1;
//		int mernode=0;
//		for(unsigned sfi=0;sfi<S.size();sfi++)
//		{
//			int sfnode=links[S[sfi]].fromNode;
//			int label=0;
//			for(unsigned lfi=0;lfi<L.size();lfi++)
//			{
//				if(links[L[lfi]].fromNode==sfnode)
//				{
//					label=1;
//					mernode=sfnode;
//					break;
//				}
//			}
//			if(label) break;
//		  }
//		// find the S1 and L1
//		int senode=links[L.front()].fromNode;
//		do
//		{
//			int sflink=S.front();
//			S1.push_back(sflink);
//			S.pop_front();
//			S.push_back(sflink);
//		    senode=links[sflink].fromNode;
//			//senode=links[S.front()].fromNode;
//		}
//		while(senode!=mernode);
//
//		int lenode=links[L.front()].fromNode;
//		do
//		{
//			int lflink=L.front();
//			L1.push_back(lflink);
//			L.pop_front();
//			L.push_back(lflink);
//			lenode=links[lflink].fromNode;
//		}
//		while(lenode!=mernode);
//				
//		// cal the combine set of S1 and L1
//		deque<int> SL(S1);
//		for(unsigned li=0;li<L1.size();li++)
//		{
//			SL.push_back(L1[li]);
//		}
//
//		//calculate the g,h and dx : see ( Yu Nie, 2010)
//		//cal g
//				
//		double gl=0.0;
//		double gs=0.0;
//		for(unsigned l1=0;l1<L1.size();l1++)
//		{
//			gl+=links[L1[l1]].cTime;
//		}
//		for(unsigned s1=0;s1<S1.size();s1++)
//		{
//			gs+=links[S1[s1]].cTime;
//		}
//		double g=gl-gs;
//		if(g<0) g=0;
//		//cout<<"g is: "<<g<<endl;
//		//cal h
//		double h=0.0;
//		for(unsigned sli=0;sli<SL.size();sli++)//cal h
//		{
//			h+=links[SL[sli]].dTime;
//		}
//		//cout<<"h is: "<<h<<endl;
//		//find the smallest flow in the L1 of bush
//		double slf=bushs[b].linkFlow[L1[0]];//define the smallest flow in the L
//		for(unsigned li=0;li<L1.size();li++)
//		{
//			if(slf>=bushs[b].linkFlow[L1[li]])
//			{
//				slf=bushs[b].linkFlow[L1[li]];
//			}
//		}
//		//cal dx
//
//		double lamda=1;
//		if(iter<60)
//		{
//			lamda=1;
//		}
//		else if((iter>=60)&&(iter<160))
//		{
//			lamda=1;
//		}
//		else
//		{
//			lamda=0.9;
//		}
//
//		double dx=lamda*(g/h);
//		if(dx>slf) dx=slf;
//		//cout<<"dx is: "<<dx<<endl;
//		k=bushs[b].topology[s];
//		if(dx==0)
//		{
//			continue;
//		}
//		else
//		{
//			double gap=bushs[b].nodes[k].U-bushs[b].nodes[k].u;
//			if(ingap<gap)
//			{
//				ingap=gap;
//				//cout<<"the ingap is : "<<ingap<<endl;
//				//system("PAUSE");
//			}
//		}
//		// change the flow on the S1 and L1
//		for(unsigned si=0;si<S1.size();si++)
//		{
//			bushs[b].linkFlow[S1[si]]=bushs[b].linkFlow[S1[si]] + dx;
//		}
//		for(unsigned li=0;li<L1.size();li++)
//		{
//			bushs[b].linkFlow[L1[li]]=bushs[b].linkFlow[L1[li]] - dx;
//		}
//		//update the flow and time of the links in SL
//		///*
//		for(unsigned sli=0;sli<SL.size();sli++)
//		{
//			int sllink=SL[sli];
//			double ttf=0;
//			for(int oi=1;oi<=odNum;oi++)
//			{
//				ttf+=bushs[oi].linkFlow[sllink];
//			}
//			links[sllink].flow=ttf;
//			links[sllink].setCTime();
//			links[sllink].setDTime();
//		}
//		//*/
//		//update the u and U
//		//renewBushTime(o,b);		
//		//system("PAUSE");	
//	}
//	//system("PAUSE");
//	return ingap;
//}

//-----cal tree
void Network::calTree(int orign,int b)
{
	vector<double> sd(nodesNum+1,inf);
	vector<double> ld(nodesNum+1,0.0);
	sd[orign]=0.0;
	ld[orign]=0.0;

	for(int ti=0;ti<nodesNum;ti++)
	{
		int searchNode=bushs[b].topology[ti];

		for(unsigned m=0;m<bushs[b].nodes[searchNode].clinks.size() ;m++) // search every link which begin node is search node
		{
			int linkID=bushs[b].nodes[searchNode].clinks[m];
			int i=links[linkID].fromNode;// define the begin node of the link is i.
			int j=links[linkID].toNode;
			double tl=links[linkID].cTime;
			//------build the shortest tree
			if (sd[j] >= sd[i] +tl)//optimality condition: d(j)>d(i)+cij
			{
				sd[j] = sd[i]+tl;
				bushs[b].nodes[j].u=sd[j];
				bushs[b].nodes[j].p=linkID;  // record the previous link in the tree
				
			}//end of checking optimality condition

			//build the longest tree
			if (ld[j] <= ld[i] +tl)//optimality condition: d(j)>d(i)+cij
			{
				ld[j] = ld[i]+tl;
				bushs[b].nodes[j].U=ld[j];
				bushs[b].nodes[j].P=linkID;  // record the previous link in the tree
			}//end of checking optimality condition
			//}
		}// end of search link
		
	}// end search node

	
}//end cal tree

//--------renewTime function
void Network::renewBushTime(int o,int b)
{
	//set the u and U to 0
	for(int i=1;i<=nodesNum;i++)
	{
		bushs[b].nodes[i].U=0.0;
		bushs[b].nodes[i].u=0.0;
	}
	for(int i=1;i<=nodesNum;i++)
	{
		//cal the u
		int pred_p=bushs[b].nodes[i].p;//get the link before the i of shortest tree
		while(pred_p)
		{
			bushs[b].nodes[i].u+=links[pred_p].cTime;
			int i_node=links[pred_p].fromNode;
			pred_p=bushs[b].nodes[i_node].p;
		}
		//cal the U
		int pred_P=bushs[b].nodes[i].P;//get the link before the i of the longest tree
		while(pred_P)
		{
			bushs[b].nodes[i].U+=links[pred_P].cTime;
			int i_node=links[pred_P].fromNode;
			pred_P=bushs[b].nodes[i_node].P;
		}
	}
}
//renew the total network flow after searching all the os
void Network::renewFlow()
{
	//-------adding the flow to the network and update the link time
	for(int li=1;li<=linksNum;li++)
	{
		double sumFlow=0.0;// temp variable to store the sum of flow
		for(int lj=1; lj<=odNum;lj++)
		{
			sumFlow+=bushs[lj].linkFlow[li];
		}
		links[li].flow=sumFlow;//update the flow 
		links[li].setCTime(); // update  the travel time
		links[li].setDTime();//update the derivative 
	}//end the assign
}
//expand the bush 

int Network::expandBush(int o,int b)
{
	//get the links not in the bush o
	vector<int> nonLinks;
	vector<int> p1;
	vector<int> p2;
	vector<int> p;
	for(int i=1;i<=linksNum;i++)
	{
		if(bushs[b].linkStatus[i]==0)
		{
			nonLinks.push_back(i);
		}
	}
	//get the p1
	for(int i=0;i<nonLinks.size();i++)
	{
		int searchLink=nonLinks[i];
		int i_node=links[searchLink].fromNode;
		int j_node=links[searchLink].toNode;
		if(bushs[b].nodes[i_node].u+links[searchLink].cTime<bushs[b].nodes[j_node].u)
		{
			p1.push_back(searchLink);
		}
	}
	//get the p2
	for(int i=0;i<nonLinks.size();i++)
	{
		int searchLink=nonLinks[i];
		int i_node=links[searchLink].fromNode;
		int j_node=links[searchLink].toNode;
		if(bushs[b].nodes[i_node].U+links[searchLink].cTime<bushs[b].nodes[j_node].U)
		{
			p2.push_back(searchLink);
		}
	}
	//get the intersection of p1 and p2
	for(int i=0;i<p1.size();i++)
	{
		for(int j=0;j<p2.size();j++)
		{
			if(p2[j]==p1[i])
			{
				p.push_back(p1[i]);
				break;
			}
		}
	}
	if(p.empty())
	{
		p=p2;
	}
	//set the return value : bush changed re=1;else re=0
	int re=1;
	if(p.empty())
	{
		re=0;
	}

	//-----add the p into the bush
	for(int i=0;i<p.size();i++)
	{
		int addLink=p[i];
		int i_node=links[addLink].fromNode;
		int j_node=links[addLink].toNode;
		bushs[b].linkStatus[addLink]=1;
		bushs[b].nodes[i_node].clinks.push_back(addLink);
		bushs[b].nodes[j_node].rlinks.push_back(addLink);

	}

	//----return re
	return re;
}
//-----------inidba
void Network::inidba()
{
	//----creat a bush for each origin and put the flow on the shortest path; the shortest and longest 
	// path tree were generated, links were added into the bush
	for(int i=1;i<=odNum;i++) //----search all the origns
	{
		shortPath(ods[i].orignID);//calculate the shortest path tree of the orign ods[i].orignID

		//-------assign the demands of each od from o to bushs
		for(int j=ods[i].pairID;j<=ods[i].endPID;j++)//search all the commodity stemming from o
		{
			int destiNode=demands[j].destiID;
			while(pred[destiNode]) //find all the links in the shortest route of commodity pair
			{
				int preNode=pred[destiNode];
				int preLink=predLink[destiNode];
				bushs[i].linkFlow[preLink]+=demands[j].demand;//assign the flow on the bush ods[i].orignID
				destiNode=preNode; // update the search Node ID
			} // end searching the link on the shortest path 
		} // end searching the shortest path stemming from o
		
		//---------creat the bush for bush i
		for(int n=1;n<=nodesNum;n++)
		{
				int temp=predLink[n];
				bushs[i].nodes[n].nodeID=n;
				bushs[i].nodes[n].u=travelTime[n];//assing the shortest time to the bush
				bushs[i].nodes[n].U=travelTime[n];
				bushs[i].nodes[n].p=predLink[n];
				bushs[i].nodes[n].P=predLink[n];
				if(temp)
				{
				   int id=links[temp].fromNode;
				   bushs[i].nodes[id].clinks.push_back(temp);//creat the clinks for node id
				   bushs[i].nodes[n].rlinks.push_back(temp);//creat the rlinks for node n
				   bushs[i].linkStatus[temp]=1;
				}
		}//---end the initialization of the bush
		//cout<<"Bush "<<i<<"is generated"<<endl;
	} // end searching all the os


	//update the network flow and time according to the bushs flow 
	renewFlow();


	//update the bushs' u for each o

	for(int i=1;i<=odNum;i++)
	{
		int o=ods[i].orignID;
		int b=i;
		renewBushTime(o,b);
		
	}
	//
	
	//expand the bush
	for(int i=1;i<=odNum;i++)
	{
		int o=ods[i].orignID;
		int b=i;
		expandBush(o,b);
	}

}// end the inidba

//-------Partan line search function

double Network::plSearch(double min,double* fwv,double* pf)
{
	double a=min;
	double b=1.0;
	int b_iter=0;
	double alpha=(a+b)/2;
	double berr=0.01;
	double derive=calPDerive(alpha,fwv,pf);
	while( ((abs(derive)>berr) && ( b_iter>100)))
	{
		derive=calPDerive(alpha,fwv,pf);
		if(derive>0)
		{
			b=alpha;
		}
		else if(derive<0)
		{
			a=alpha;
		}
		alpha=(a+b)/2;
		b_iter+=1;
	}
	return alpha;
}

//------the deriviation function of Partan line search function
double Network::calPDerive(double a,double* fwvv,double* pff)
{
	double derivative=0.0;
	for(int i=1;i<=linksNum;i++)
	{
		double gap=pff[i]-fwvv[i];
		double xal=fwvv[i]+a*gap;
		derivative+=links[i].calTime(xal)*gap;
	}
	return derivative;
}

//-----------LUCE algorithm

void Network::LUCE(double error)
{
	/*cout<<"Algorithm LUCE begin"<<endl;
	system("PAUSE");*/
	 //---
	time_t start,end,time;
	int outIter=0;
	int inIter=0;
	double outgap=1.0;
	double regap=1.0;
	double maxgap=1.0;
	double stopNum=1;
	cout<<"The algorithm is begin:"<<endl;
	start=clock();
	//ini the algorithm
	iniLuce();
	//
	//cout<<"The link flow after the ini"<<endl;
	//for(int li=1;li<=linksNum;li++)
	//{
	//	cout<<"link "<<li<<" flow is : "<<links[li].flow<<endl;
	//}
	//cout<<"Iniluce end"<<endl;
	//system("PAUSE");

	//begin the  outer iteration
	while((stopNum>0)&&(outgap>error)&&(outIter<1000))//begin the outer iteration
	{
		if(outIter>1)
		{
		regap=0.0;
		maxgap=0.0;
		}
		stopNum=0;
		outIter+=1;
		//cout<<"The outer iteration number is : "<<outIter<<endl;
		//system("PAUSE");
		//search every destination
		for(int di=1;di<=odNum;di++)
		{
			
			int stop;
			inIter=0;
			/*do
			{*/
				stop=0;
				inIter+=1;
				double rd=0.0;
				double th=0.0;
				double la=0.0;
				double sg=0.0;
				
				//compute the shortest and longest path value for the bush
				luceBushShort(di);
				


				stopCriterion(di,outIter,&rd,&th,&la,&sg);
				if(outIter>1)
				{
				regap+=rd;
				if(maxgap<rd)
				{
					maxgap=rd;
				}
				}
				
				
				if((outIter==1)||(rd>10e-8)||(th>10e-6)||(la>10e-6))
				{
					
					stopNum+=1;
					stop=1;
				    
					if((la>10e-6)||(outIter==1))
					{
						if(rd<10e-5)
						{
							updateBush1(di);
						}
						else
						{
							updateBush2(di);
						}
						//updateBush3(di);
					}


					//determine the search direction
					directionLuce(di);

					//determain the step size alpha
					double alpha=0.0;
					double c1=0.0;//the derivative at alpha equal to zero
					double c2=0.0;//the objective functon at alpha equarl to zero
					double c3=0.0;//the objective function at alpha value
					int h=0;//variable h
					//compute the c1
					for(int li=1;li<=linksNum;li++)
					{
						
						c1+=links[li].cTime*(bushs[di].eluce[li]-bushs[di].linkFlow[li]);
					
					}
					
					
				
					for(int li=1;li<=linksNum;li++)
					{
						double x=links[li].flow;
						double t0=links[li].calTime(0.0);
						double ta=t0*links[li].flow+((0.03*t0)/(pow(links[li].capacity,4)))*pow(links[li].flow,5);
						c2+=ta;
					}
					
				
					//reset the variable h
					h=-1;
					do
					{
						h=h+1;
						alpha=pow(0.5,h);

						

						vector<double> lis(linksNum+1,0.0);
						for(int li=1;li<=linksNum;li++)
						{
							lis[li]=links[li].flow+alpha*(bushs[di].eluce[li]-bushs[di].linkFlow[li]);
						}
						c3=calObjective(&lis[0]);
						
					}
					while((c3>=c2+(0.0001) *alpha*c1)&&(h<20));
					if(h>19)
					{
						alpha=0.0;
					}

					

					//make the move along the LUCE search direction
					for(int li=1;li<=linksNum;li++)
					{
						if(bushs[di].linkStatus[li]==1)
						{
							bushs[di].linkFlow[li]=bushs[di].linkFlow[li]+alpha*(bushs[di].eluce[li]-bushs[di].linkFlow[li]);
						}
					}
					//update the link after the move
					updatelinks();


				}//end if
			/*}
			while((inIter<5)&&(stop==1));*/
			//end do
			//cout<<"the inIter= "<<inIter<<endl;
				
		}//search every destination
		double T3=0.0;
		double T4=0.0;
		double T5=0.0;
		for(int oi=1;oi<=odNum;oi++)
		{
			//calTree(ods[oi].orignID,oi);
			int begin_pair=ods[oi].pairID;
			int end_pair=ods[oi].endPID;
			for(int pj=begin_pair;pj<=end_pair;pj++)
			{
				int pdid=demands[pj].destiID;
				double drs=demands[pj].demand;
				double urs=bushs[oi].nodes[pdid].u;
				T3=T3+drs*urs;
			}
		}
		
		//cout<<endl<<"T3 is: "<<T3;
		//system("PAUSE");
		for(int li=1;li<=linksNum;li++)
		{
			double xij=links[li].flow;
			double tij=links[li].cTime;
			T4+=xij*tij;
		}
		outgap=abs(1-T3/T4);
		cout<<"outgap is : "<<outgap<<endl;
		regap=regap/(double)(odNum);
		//maxgap=maxgap/odNum;
		cout<<maxgap<<endl;
		
		double T1=calObjective(&links[0]);
		//cout<<"The stopNum is : "<<stopNum<<endl;
		cout<<"The objective function is : "<<setprecision(15)<<T1<<endl;
		//system("PAUSE");
	}//outer iteration
	end =clock();
	time=(end-start)/CLOCKS_PER_SEC;
	double s=calObjective(&links[0]);
	cout<<"The outIter is : "<<outIter<<endl;
	cout<<"The precision is : "<<error<<endl;
	cout<<"The objective function is : "<<setprecision(15)<<s<<endl;
	cout<<"The calculation time is : "<<time<<endl;
}

//--------------------make the bush
void Network::makeBush(int di)
{
	int d=ods[di].destiID;
	shortPathLuce(di);
	for(int ni=1;ni<=nodesNum;ni++)
	{
		bushs[di].nodes[ni].LW=travelTime[di];
	}
	//test
	cout<<"Test the Lw for the first time : "<<endl;
	for(int ni=1;ni<=nodesNum;ni++)
	{
		cout<<"node " <<ni<<" 's lw is : "<<bushs[di].nodes[ni].LW<<endl;
	}
	system("PAUSE");

	//put the link into the bush
	for(int li=1;li<=linksNum;li++)
	{
		int inode=links[li].fromNode;
		int jnode=links[li].toNode;
		if(bushs[di].nodes[inode].LW>bushs[di].nodes[jnode].LW)
		{
			//put the link into the bush
			bushs[di].linkStatus[li]=1;
			//add the link
			bushs[di].nodes[inode].clinks.push_back(li);
			bushs[di].nodes[jnode].rlinks.push_back(li);
		}
	}

}
//--------------------calObjective
double Network::calObjective(double * flow)
{
	double ob=0.0;
	for(int li=1;li<=linksNum;li++)
	{
		double x=flow[li];
		double t0=links[li].calTime(0.0);
		double ta=t0*x+((0.03*t0)/(pow(links[li].capacity,4)))*pow(x,5);
		ob+=ta;
	}
	return ob;
}
//---------------------search direction for LUCE
void Network::directionLuce(int di)
{
	int d=ods[di].destiID;
	//updatelinks();
	//compute the current flow proportions
	for(int ni=1;ni<=nodesNum;ni++)
	{
		double sum=0.0;
		for(unsigned j=0;j<bushs[di].nodes[ni].clinks.size();j++)
		{
			int slink=bushs[di].nodes[ni].clinks[j];
			sum+=bushs[di].linkFlow[slink];
		}
		bushs[di].nodes[ni].fi=sum;
	}
	//compute the link proportion
	int test=0;
	for(int li=1;li<=linksNum;li++)
	{
		int inode=links[li].fromNode;
		int jnode=links[li].toNode;
		if(bushs[di].nodes[inode].fi>0)
		{
			bushs[di].yluce[li]=bushs[di].linkFlow[li]/bushs[di].nodes[inode].fi;
		}
		else
		{
			bushs[di].yluce[li]=0.0;
		}
	}
	//compute the node average costs and their derivatives
	bushs[di].nodes[d].Cd=0.0;
	bushs[di].nodes[d].Gd=0.0;
	////
	/*cout<<"Before the reverse order : "<<endl;
	for(int ni=1;ni<=nodesNum;ni++)
	{
		cout<<ni<<setw(10)<<setw(10);
		cout<<"rlinks : ";
		for(int j=0;j<bushs[di].nodes[ni].rlinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].rlinks[j]<<setw(10);
		}
		cout<<"clinks : ";
		for(int j=0;j<bushs[di].nodes[ni].clinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].clinks[j]<<setw(10);
		}
		cout<<endl;
	}
	system("PAUSE");*/

	reverseOrder(di);//compute the reverse topology order of the bush

	//cout the bush di
	/*for(int ni=1;ni<=nodesNum;ni++)
	{
		cout<<ni<<setw(10)<<setw(10);
		cout<<"rlinks : ";
		for(int j=0;j<bushs[di].nodes[ni].rlinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].rlinks[j]<<setw(10);
		}
		cout<<"clinks : ";
		for(int j=0;j<bushs[di].nodes[ni].clinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].clinks[j]<<setw(10);
		}
		cout<<endl;
	}
	system("PAUSE");*/
	
	//cout<<"The reverse topology is : "<<endl;
	//for(int ni=0;ni<nodesNum;ni++)
	//{
	//	cout<<"The topology is : "<<bushs[di].topology[ni]<<endl;
	//}
	//system("PAUSE");


	for(unsigned ti=1;ti<nodesNum;ti++)
	{
		int inode=bushs[di].topology[ti];
		/*cout<<"The fi is : "<<bushs[di].nodes[inode].fi<<endl;
		system("PAUSE");*/
		if(bushs[di].nodes[inode].fi>0.0)
		{
			//compute the Ci and Gi
			double ci=0.0;
			double gi=0.0;
			for(unsigned cj=0;cj<bushs[di].nodes[inode].clinks.size();cj++)
			{
				int slink=bushs[di].nodes[inode].clinks[cj];
				int jnode=links[slink].toNode;
				ci+=bushs[di].yluce[slink]*(links[slink].cTime+bushs[di].nodes[jnode].Cd);
				gi+=bushs[di].yluce[slink]*bushs[di].yluce[slink]*(links[slink].dTime+bushs[di].nodes[jnode].Gd);
			}
			bushs[di].nodes[inode].Cd=ci;
			bushs[di].nodes[inode].Gd=gi;
			
		}
		else
		{
			
			double ci=inf;
			double gi=0.0;
			double gnum=0.0;
			for(unsigned cj=0;cj<bushs[di].nodes[inode].clinks.size();cj++)
			{
				int slink=bushs[di].nodes[inode].clinks[cj];
				int jnode=links[slink].toNode;
				if(ci>links[slink].cTime+bushs[di].nodes[jnode].Cd)
				{
					ci=links[slink].cTime+bushs[di].nodes[jnode].Cd;
				}
				
			}
			bushs[di].nodes[inode].Cd=ci;
			/*cout<<"ci is : "<<ci<<endl;*/
			for(unsigned cj=0;cj<bushs[di].nodes[inode].clinks.size();cj++)
			{
				int slink=bushs[di].nodes[inode].clinks[cj];
				int jnode=links[slink].toNode ;
				//
				if(bushs[di].nodes[inode].Cd==links[slink].cTime+bushs[di].nodes[jnode].Cd)
				{
					gi+=links[slink].dTime+bushs[di].nodes[jnode].Gd;
					gnum+=1.0;
				}
			}
			
			/*cout<<"gi is : "<<gi<<endl;
			cout<<"gnum is : "<<gnum<<endl;
			system("PAUSE");*/
			bushs[di].nodes[inode].Gd=gi/gnum;
		}
	}
	int test1=1;
	//-------solve the sequence of LUCE basic problems
	for(unsigned odi=1;odi<nodesNum;odi++)
	{
		int snode=bushs[di].topology[nodesNum-odi];
		//compute the sequence of LUCE basic problems
		double ei=0.0;
		for(unsigned bi=0;bi<bushs[di].nodes[snode].rlinks.size();bi++)
		{
			int slink=bushs[di].nodes[snode].rlinks[bi];
			//ei+=bushs[di].linkFlow[slink];#########this is the eij not the fij
			ei+=bushs[di].eluce[slink];
		}
		//find the demand id
		double did=0.0;
		for(int i=ods[di].pairID;i<=ods[di].endPID;i++)
		{
			if(demands[i].orignID==snode)
			{
				did=demands[i].demand;
			}
		}
		bushs[di].nodes[snode].ei=ei+did;
		//if this node is used then:
		if(bushs[di].nodes[snode].ei>0)
		{
			//ini the J
		     deque<int> J;
			 for(unsigned j=0;j<bushs[di].nodes[snode].clinks.size();j++)
			 {
				 int jlink=bushs[di].nodes[snode].clinks[j];
				 J.push_back(jlink);
			 }
			 //do the loop
			 int lam=0;
			 do
			 {
				 lam=0;
				 //compute the local equilibrium cost of node i to d through
				 double a=0.0;
				 double b=0.0;
				 double c=0.0;
				 for(unsigned j=0;j<J.size();j++)
				 {
					 int jlink=J[j];
					 int jnode=links[jlink].toNode;
					 a+=bushs[di].yluce[jlink];
					 b+=(links[jlink].cTime+bushs[di].nodes[jnode].Cd)/(links[jlink].dTime+bushs[di].nodes[jnode].Gd);
					 c+=1/(links[jlink].dTime+bushs[di].nodes[jnode].Gd);
				 }
				 bushs[di].nodes[snode].Vi=(bushs[di].nodes[snode].ei*(1.0-a)+b)/c;
				 //compute the auxiliary flow on the link ij 
				 for(unsigned j=0;j<J.size();j++)
				 {
					 int jlink=J[j];
					 int jnode=links[jlink].toNode;
					 bushs[di].eluce[jlink]=bushs[di].nodes[snode].ei*bushs[di].yluce[jlink]+bushs[di].nodes[snode].Vi/(links[jlink].dTime+bushs[di].nodes[jnode].Gd)-(links[jlink].cTime+bushs[di].nodes[jnode].Cd)/(links[jlink].dTime+bushs[di].nodes[jnode].Gd);
					//if the resulting link is negative 
					 if(bushs[di].eluce[jlink]<0)
					 {
						 bushs[di].eluce[jlink]=0.0;
						 int size=J.size();
						 //remove the jlink from the J
						 for(int s=1;s<=size;s++)
						 {
							 int dlink=J.front();
							 J.pop_front();
							 if(dlink==jlink)
							 {
								 break;
							 }
							 else
							 {
								 J.push_back(dlink);
							 }
						 }
						 //
						 lam=1;
					 }//end if
				 }//next j
			 }
			 while(lam==1);
		}
		else
		{
			for(unsigned j=0;j<bushs[di].nodes[snode].clinks.size();j++)
			{
				int jlink=bushs[di].nodes[snode].clinks[j];
				bushs[di].eluce[jlink]=0.0;

			}//next j
		}//end if
	}//next snode

}//end function

//----------update the bush 1: in one condition that rd is equal to zero where the master problem is in equalibrium
//----------need to add link and delete link 
void Network::updateBush1(int di)
{
	int d=ods[di].destiID;


	//cout the bush
	/*for(int ni=1;ni<=nodesNum;ni++)
	{
		cout<<ni<<setw(10)<<setw(10);
		cout<<"rlinks : ";
		for(int j=0;j<bushs[di].nodes[ni].rlinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].rlinks[j]<<setw(10);
		}
		cout<<"clinks : ";
		for(int j=0;j<bushs[di].nodes[ni].clinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].clinks[j]<<setw(10);
		}
		cout<<endl;
	}
	system("PAUSE");
	cout<<"cout the link flow for the bush "<<endl;
	for(int li=1;li<=linksNum;li++)
	{
		cout<<"link : "<<li<<" status is : "<<bushs[di].linkStatus[li]<<" linkflow is : "<<bushs[di].linkFlow[li]<<endl;
	}
	system("PAUSE");*/



	for(int li=1;li<=linksNum;li++)
	{
		//delete the links 
		if(bushs[di].linkStatus[li]==1)
		{
			int inode=links[li].fromNode;
			int jnode=links[li].toNode;
			//delete the link from the bush
			if((bushs[di].linkFlow[li]==0.0)&&(bushs[di].nodes[inode].clinks.size()>1))
			{
				bushs[di].linkStatus[li]=0;
				//delete from the clinks
				int inum=bushs[di].nodes[inode].clinks.size();
				for(unsigned ni=0;ni<inum;ni++)
				{
					int slink=bushs[di].nodes[inode].clinks.front();
					bushs[di].nodes[inode].clinks.pop_front();
					if(slink!=li)
					{
						bushs[di].nodes[inode].clinks.push_back(slink);
					}
				}
				//delete from the rlinks
				int jnum=bushs[di].nodes[jnode].rlinks.size();
				for(unsigned ni=0;ni<jnum;ni++)
				{
					int slink=bushs[di].nodes[jnode].rlinks.front();
					bushs[di].nodes[jnode].rlinks.pop_front();
					if(slink!=li)
					{
						bushs[di].nodes[jnode].rlinks.push_back(slink);
					}
				}
				//
			}
		}
	}
    for(int li=1;li<=linksNum;li++)
	{
		if(bushs[di].linkStatus[li]==0)
		{
			//add the link into the bush
			int inode=links[li].fromNode;
			int jnode=links[li].toNode;
			if(bushs[di].nodes[inode].LW>bushs[di].nodes[jnode].LW)
			{
				//put this link into the bush
				bushs[di].linkStatus[li]=1;
				bushs[di].nodes[inode].clinks.push_back(li);
				bushs[di].nodes[jnode].rlinks.push_back(li);
			}
		}
		
	}
	/*cout<<"After the bush management"<<endl;
	for(int ni=1;ni<=nodesNum;ni++)
	{
		cout<<ni<<setw(10)<<setw(10);
		cout<<"rlinks : ";
		for(int j=0;j<bushs[di].nodes[ni].rlinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].rlinks[j]<<setw(10);
		}
		cout<<"clinks : ";
		for(int j=0;j<bushs[di].nodes[ni].clinks.size();j++)
		{
			cout<<bushs[di].nodes[ni].clinks[j]<<setw(10);
		}
		cout<<endl;
	}
	system("PAUSE");*/
}
//------------------update2 
void Network::updateBush2(int di)
{
	int d=ods[di].destiID;
	for(int li=1;li<=linksNum;li++)
	{
		//delete the links
		if(bushs[di].linkStatus[li]==1)
		{
			int inode=links[li].fromNode;
			int jnode=links[li].toNode;
			//delete the link from the bush
			if((bushs[di].linkFlow[li]==0.0)&&(bushs[di].nodes[inode].LU<=bushs[di].nodes[jnode].LU)&&(bushs[di].nodes[inode].clinks.size()>1))
			{
				bushs[di].linkStatus[li]=0;
				//delete from the clinks
				int inum=bushs[di].nodes[inode].clinks.size();
				for(unsigned ni=0;ni<inum;ni++)
				{
					int slink=bushs[di].nodes[inode].clinks.front();
					bushs[di].nodes[inode].clinks.pop_front();
					if(slink!=li)
					{
						bushs[di].nodes[inode].clinks.push_back(slink);
					}
				}
				//delete from the rlinks
				int jnum=bushs[di].nodes[jnode].rlinks.size();
				for(unsigned ni=0;ni<jnum;ni++)
				{
					int slink=bushs[di].nodes[jnode].rlinks.front();
					bushs[di].nodes[jnode].rlinks.pop_front();
					if(slink!=li)
					{
						bushs[di].nodes[jnode].rlinks.push_back(slink);
					}
				}
				//
			}
		}
	}
	for(int li=1;li<=linksNum;li++)
	{
		if(bushs[di].linkStatus[li]==0)
		{
			//add the link into the bush
			int inode=links[li].fromNode;
			int jnode=links[li].toNode;
			if(bushs[di].nodes[inode].LU>bushs[di].nodes[jnode].LU)
			{
				//put this link into the bush
				bushs[di].linkStatus[li]=1;
				bushs[di].nodes[inode].clinks.push_back(li);
				bushs[di].nodes[jnode].rlinks.push_back(li);
			}
		}
	}
}
//update 3
void Network::updateBush3(int di)
{
	int d=ods[di].destiID;
	for(int li=1;li<=linksNum;li++)
	{
		//delete the links
		if(bushs[di].linkStatus[li]==1)
		{
			int inode=links[li].fromNode;
			int jnode=links[li].toNode;
			//delete the link from the bush
			if((bushs[di].linkFlow[li]==0.0)&&(bushs[di].nodes[inode].LU<=bushs[di].nodes[jnode].LU)&&(bushs[di].nodes[inode].clinks.size()>1))
			{
				bushs[di].linkStatus[li]=0;
				//delete from the clinks
				int inum=bushs[di].nodes[inode].clinks.size();
				for(unsigned ni=0;ni<inum;ni++)
				{
					int slink=bushs[di].nodes[inode].clinks.front();
					bushs[di].nodes[inode].clinks.pop_front();
					if(slink!=li)
					{
						bushs[di].nodes[inode].clinks.push_back(slink);
					}
				}
				//delete from the rlinks
				int jnum=bushs[di].nodes[jnode].rlinks.size();
				for(unsigned ni=0;ni<jnum;ni++)
				{
					int slink=bushs[di].nodes[jnode].rlinks.front();
					bushs[di].nodes[jnode].rlinks.pop_front();
					if(slink!=li)
					{
						bushs[di].nodes[jnode].rlinks.push_back(slink);
					}
				}
				//
			}
		}
	}
	for(int li=1;li<=linksNum;li++)
	{
		if(bushs[di].linkStatus[li]==0)
		{
			//add the link into the bush
			int inode=links[li].fromNode;
			int jnode=links[li].toNode;
			if((bushs[di].nodes[inode].LU>bushs[di].nodes[jnode].LU)&&(bushs[di].nodes[inode].LW>bushs[di].nodes[jnode].LW))
			{
				//put this link into the bush
				bushs[di].linkStatus[li]=1;
				bushs[di].nodes[inode].clinks.push_back(li);
				bushs[di].nodes[jnode].rlinks.push_back(li);
			}
		}
	}
}


//cal the shortest path and longest for each node in the bush
void Network::luceBushShort(int di)
{
	int d=ods[di].destiID;
	//cal the reverse topology order and record them in the topology
		reverseOrder(di);
		//set the variables
		for(int ni=1;ni<=nodesNum;ni++)
		{
			bushs[di].nodes[ni].LW=inf;
			bushs[di].nodes[ni].LU=0.0;
		}
		//very inportant to set the ini value for the LW and LU
		bushs[di].nodes[d].LW=0.0;
		bushs[di].nodes[d].LU=0.0;
		//search every node in the reverse order : -O(i,d) except the d
		for(int ni=1;ni<nodesNum;ni++)
		{
			
			int cnode=bushs[di].topology[ni];

			/*cout<<"The search node is : " <<cnode<<endl;
			system("PAUSE");*/

			//compute the node mimnum
			//int jnode=links[bushs[di].nodes[cnode].clinks.front()].toNode;
			//bushs[di].nodes[cnode].LW=links[bushs[di].nodes[cnode].clinks.front()].cTime+bushs[di].nodes[jnode].LW;
			for(int wi=0;wi<bushs[di].nodes[cnode].clinks.size();wi++)
			{
				int wlink=bushs[di].nodes[cnode].clinks[wi];
				int jj=links[wlink].toNode;
				if(bushs[di].nodes[cnode].LW>links[wlink].cTime+bushs[di].nodes[jj].LW)
				{
					bushs[di].nodes[cnode].LW=links[wlink].cTime+bushs[di].nodes[jj].LW;
				}
			}

			/*cout<<"The cnode 's minimum cost is : "<<bushs[di].nodes[cnode].LW<<endl;
			system("PAUSE");*/

			//compute the maximum cost 
			if(bushs[di].nodes[cnode].fi>0)
			{
				for(int ui=0;ui<bushs[di].nodes[cnode].clinks.size();ui++)
				{
					int ulink=bushs[di].nodes[cnode].clinks[ui];
					int jj=links[ulink].toNode;
					if((bushs[di].nodes[cnode].LU<links[ulink].cTime+bushs[di].nodes[jj].LU)&&(bushs[di].nodes[cnode].fi>0))
					{
						bushs[di].nodes[cnode].LU=links[ulink].cTime+bushs[di].nodes[jj].LU;
					}
				}
			}
			else
			{
				for(int ui=0;ui<bushs[di].nodes[cnode].clinks.size();ui++)
				{
					int ulink=bushs[di].nodes[cnode].clinks[ui];
					int jj=links[ulink].toNode;
					if((bushs[di].nodes[cnode].LU<links[ulink].cTime+bushs[di].nodes[jj].LU)&&(bushs[di].nodes[cnode].LW==links[ulink].cTime+bushs[di].nodes[jj].LW))
					{
						bushs[di].nodes[cnode].LU=links[ulink].cTime+bushs[di].nodes[jj].LU;
					}
				}
			}

			/*cout<<"The max of cnode is : "<<bushs[di].nodes[cnode].LU<<endl;
			system("PAUSE");*/

		}//end search the order
}
//----------cal the stop criterion for the destination di
void Network::stopCriterion(int di,int outI,double* rd,double* th,double* la,double* sg)
{
	int d=ods[di].destiID;
	if(outI>1)
	{
		
		
		//cout the lw
		/*for(int ni=1;ni<=nodesNum;ni++)
		{
			cout<<"node " <<ni<<" 's LW is : "<<bushs[di].nodes[ni].LW<<endl;
		}
		system("PAUSE");*/



		double up=0.0;
		double down=0.0;
		//cal the up
		for(int pi=ods[di].pairID;pi<=ods[di].endPID;pi++)
		{
			int or=demands[pi].orignID;
			double demand=demands[pi].demand;
			up+=bushs[di].nodes[or].LW*demand;
		}
		//cout<<"The up is : "<<up<<endl;
		//cal the down
		for(int li=1;li<=linksNum;li++)
		{
			/*if(bushs[di].linkStatus[li]==1)
			{*/
				down+=links[li].cTime*bushs[di].linkFlow[li];
			/*}*/
		}
		//cout<<"The down is : "<<down<<endl;
		*rd=0.0;
		if(down>0)
		{
			*rd=1.0-up/down;
		}
		/*cout<<"The rd is : "<<*rd<<endl;
		system("PAUSE");*/
		//cal the theit
		*th=0.0;
		for(int pi=ods[di].pairID;pi<=ods[di].endPID;pi++)
		{
			int origin=demands[pi].orignID;
			if(*th<bushs[di].nodes[origin].LU/bushs[di].nodes[origin].LW-1)
			{
				*th=bushs[di].nodes[origin].LU/bushs[di].nodes[origin].LW-1;
			}
		}
		//cal the la
		double lnum=0;
		for(int li=1;li<=linksNum;li++)
		{
			if(bushs[di].linkStatus[li]==0)
			{
				int in=links[li].fromNode;
				int jn=links[li].toNode;
				if(bushs[di].nodes[in].LW>=links[li].cTime+bushs[di].nodes[jn].LW)
				{
					lnum+=1;
				}
			}
		}
		*la=lnum/linksNum;
		/*cout<<"The la is : "<<*la<<endl;*/
		//cal the sg
		double snum=0;
		for(int li=1;li<=linksNum;li++)
		{
			if((bushs[di].linkStatus[li]==1)&&(bushs[di].linkFlow[li]>0))
			{
				int in=links[li].fromNode;
				int jn=links[li].toNode;
				if(bushs[di].nodes[in].LW<=bushs[di].nodes[jn].LW)
				{
					snum+=1;
				}
			}
		}
		*sg=snum/linksNum;
		/*cout<<"The sg is : "<<*sg<<endl;
		system("PAUSE");*/
	}

}
//-----cal the reverse order for the LUCE bush
void Network::reverseOrder(int di)
{
	int d=ods[di].destiID;
	//reset the topology
	bushs[di].topology.clear();
	for(int ni=1;ni<=nodesNum+1;ni++)
	{
		bushs[di].topology.push_back(0);
	}
	int mm=0;
	//creat a new bush ini as bushs[di]
	bush tbush=bushs[di];
	deque<int> H;
	H.push_back(d);
	//search H
	while(!H.empty())
	{
		int searchNode=H.front();
		H.pop_front();
		//put the searchNode into the topology
		bushs[di].topology[mm]=searchNode;
		mm+=1;
		//search all the rlinks of searchNode
		while(!tbush.nodes[searchNode].rlinks.empty())
		{
			int dlink=tbush.nodes[searchNode].rlinks.front();
			tbush.nodes[searchNode].rlinks.pop_front();
			int inode=links[dlink].fromNode;
			//delete the dlink
			while(!tbush.nodes[inode].clinks.empty())
			{
				//pop each link in clinks and if it is not dlink then put them back into clinks again
				int clink=tbush.nodes[inode].clinks.front();
				tbush.nodes[inode].clinks.pop_front();
				if(clink==dlink)
				{
					break;
				}
				else
				{
					tbush.nodes[inode].clinks.push_back(clink);
				}
			}
			//put inode into the H 
			if(tbush.nodes[inode].clinks.empty())
			{
				H.push_back(inode);
			}
		}
	}
}
//----------iniLuce
void Network::iniLuce()
{
	//creat the bush for all the destinations 
	for(int di=1;di<=odNum;di++)
	{
		//do the shortest path algorithm
		shortPathLuce(di);
		//assign the demand onto the bushs
		for(int pi=ods[di].pairID;pi<=ods[di].endPID;pi++)
		{
			int origin=demands[pi].orignID;
			while(pred[origin])
			{
				int jnode=pred[origin];
				int jlink=predLink[origin];
				bushs[di].linkFlow[jlink]+=demands[pi].demand;
				origin=jnode;
			}
		}//end assign the demands to this bush
		//---creat the bush for bush di
		for(int ni=1;ni<=nodesNum;ni++)
		{
			int temp=predLink[ni];
			bushs[di].nodes[ni].nodeID=ni;
			if(temp)
			{
			   //assign the FSB ij of i in to the fi,there is only one link stem from node i in the shortest tree
				bushs[di].nodes[ni].fi=bushs[di].linkFlow[temp];
				//construct the bush
				int id=links[temp].fromNode;
				int jd=links[temp].toNode;
				bushs[di].nodes[id].clinks.push_back(temp);
				bushs[di].nodes[jd].rlinks.push_back(temp);
				bushs[di].linkStatus[temp]=1;

			}
		}

		/*cout<<"The shortest tree of "<<di<<endl;
		for(int ni=1;ni<=nodesNum;ni++)
		{
			cout<<"Node : "<<ni <<"' s prenode is : "<<pred[ni]<<endl;
		}
		system("PAUSE");*/
	/*cout<<"cout the link flow for the bush "<<endl;
	for(int li=1;li<=linksNum;li++)
	{
		cout<<"link : "<<li<<" status is : "<<bushs[di].linkStatus[li]<<" linkflow is : "<<bushs[di].linkFlow[li]<<endl;
	}
	system("PAUSE");*/

	}//end searching all the ods
	//------update the flow time and dtime for each link
	updatelinks();
	//

}
//----------update the links 
void Network::updatelinks()
{
	//
	for(int li=1;li<=linksNum;li++)
	{
		double lf=0.0;
		for(int di=1;di<=odNum;di++)
		{
			lf+=bushs[di].linkFlow[li];
		}
		links[li].flow=lf;
		links[li].setCTime();
		links[li].setDTime();
	}
}
//----------cal short path for LUCE 
void Network::shortPathLuce(int b)
{
	int desti=ods[b].destiID;
	vector<double> d(nodesNum+1,inf);
	deque<int> Q;
	for (int i=0;i<nodesNum+1;i++)
	{
		travelTime[i]=inf;//shortest travel time equal to d
		d[i]=inf;
        pred[i]=inf;
		predLink[i]=inf;
	}
	travelTime[desti]=0.0;
	d[desti]=0.0;
	pred[desti]=0;
	predLink[desti]=0;
	Q.push_back(desti);
	while(!Q.empty())
	{
		int sNode=Q.front();
		Q.pop_front();
		for(int li=nodes[sNode].beginLinkID;li<=nodes[sNode].endLinkID;li++)
		{
			int iNode=links[li].fromNode;
			double t=links[li].cTime;
			//check the length
			if(d[iNode]>d[sNode]+t)
			{
				int iexist=0;
				d[iNode]=d[sNode]+t;
				travelTime[iNode]=d[iNode];
				pred[iNode]=sNode;
				predLink[iNode]=li;
				if(!Q.empty())
				{
				for(unsigned int n=0;n < Q.size();n++)
				{
					if (Q[n] == iNode) iexist=1;
				}
				}

				if(!iexist) 
				Q.push_back(iNode);
			}
		}
		
	}
}
//---------convert the network data
void Network::convertNet(string file,long seekarg)
{
	//creat the struct to record the data
	struct ss
	{
		int jNode;
		vector<int> linkID;
		vector<int> iNode;
		vector<double> capa;
		vector<double> len;
		vector<double> spe;

	};
	ss so;
	so.jNode=0;
	vector<ss> jnodes(nodesNum+1,so);

	cout<<"the nodes size is : "<<jnodes.size()<<endl;
	system("PAUSE");
	//to record the link num
	int mm=0;
	for(int ni=1;ni<=nodesNum;ni++)
	{
		jnodes[ni].jNode=nodes[ni].nodeID;
		for(int li=1;li<=linksNum;li++)
		{
			int end=links[li].toNode;
			if(end==jnodes[ni].jNode)
			{
				mm+=1;
				jnodes[ni].linkID.push_back(mm);
				jnodes[ni].iNode.push_back(links[li].fromNode);
				jnodes[ni].capa.push_back(links[li].capacity);
				jnodes[ni].len.push_back(links[li].length);
				jnodes[ni].spe.push_back(links[li].speed);
			}
		}
	}
	//
	cout<<"The links num is : "<<mm<<endl;
	system("PAUSE");
	//cout the file
	ofstream outFile(file,seekarg);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	outFile<<left<<nodesNum<<"\n";
	//cout the nodes 
	for(int ni=1;ni<=nodesNum;ni++)
	{
		outFile<<left<<setw(10)<<jnodes[ni].jNode<<setw(10)<<jnodes[ni].linkID.front()<<setw(10)<<jnodes[ni].linkID.back()<<"\n";
	}
	outFile<<left<<mm<<"\n";
	for(int ni=1;ni<=nodesNum;ni++)
	{
		for(int li=0;li<jnodes[ni].iNode.size();li++)
		{
			outFile<<left<<setw(10)<<jnodes[ni].iNode[li]<<setw(10)<<jnodes[ni].jNode<<setw(10)<<jnodes[ni].capa[li]<<setw(10)<<jnodes[ni].len[li]<<setw(10)<<jnodes[ni].spe[li]<<"\n";
		}
	}
	outFile.close();

}
//---------convert the ods data
void Network::convertOD(string file,long seekarg)
{
	//creat a struct to record the data
	struct ss
	{
		int dID;
		vector<int> tripID;
		vector<int> originID;
		vector<double> demand;
	};
	ss s0;
	s0.dID=0;
	vector<ss> destination(odNum+1,s0);
	cout<<destination.size()<<endl;
	system("PAUSE");
	//to record the pair number
	int mm=0;
	//search every destinations
	for(int di=1;di<=odNum;di++)
	{
		destination[di].dID=ods[di].orignID;
		//search every pair
		for(int dj=1;dj<=pairNum;dj++)
		{
			//check if the destination is the dID
			if(demands[dj].destiID==destination[di].dID)
			{
				mm+=1;
				destination[di].tripID.push_back(mm);
				destination[di].originID.push_back(demands[dj].orignID);
				destination[di].demand.push_back(demands[dj].demand);
			}
		}
		//
	}
	cout<<"The m is : "<<mm<<endl;
	system("PAUSE");
	//cout the file 
	ofstream outFile(file,seekarg);
	if(!outFile)
	{
		cerr<<"error: unable to write the file: "<<outFile<<endl;
	}
	outFile<<left<<odNum<<"\n";
	for(int di=1;di<=odNum;di++)
	{
		
		outFile<<left<<setw(10)<<destination[di].dID<<setw(15)<<destination[di].tripID.front()<<setw(10)<<destination[di].tripID.back()<<"\n";

	}
	outFile<<left<<mm<<"\n";
	for(int di=1;di<=odNum;di++)
	{
		for(int dj=0;dj<destination[di].originID.size();dj++)
		{
			outFile<<left<<setw(10)<<destination[di].originID[dj]<<setw(10)<<destination[di].dID<<setw(10)<<destination[di].demand[dj]<<"\n";
		}
	}
	outFile.close();
}
//---------------read the LUCE network data into nodes and links ,the file is sf.nl
void Network::readLuceNet(string file, long seekarg)
{
	//ini the nodes and links
	nodes.clear();
	links.clear();
	Node n00;
	Link l00;
	nodes.push_back(n00);
	links.push_back(l00);

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
		int nodeID=0;
		int bID=0;
		int eID=0;
		Node no;
		inFile>>nodeID>>bID>>eID;
		no.nodeID=nodeID;
		no.beginLinkID=bID;
		no.endLinkID=eID;
		nodes.push_back(no);

	}
	//----read the links number
	inFile>>linksNum;
	cout<<"The number of links in the network is: "<<linksNum<<endl;

	//----read the links attributes
	for ( int i=1;i<linksNum+1;i++)
	{
		int fNode=0;
		int eNode=0;
		double cap=0.0;
		double len=0.0;
		double spe=0.0;
		Link li;
		inFile>>fNode>>eNode>>cap>>len>>spe;
		li.linkID=i;
		li.fromNode=fNode;
		li.toNode=eNode;
		li.capacity=cap;
		li.length=len;
		li.speed=spe;
		li.cTime=len/spe;
		links.push_back(li);
	}
	//after read the network, then give the dimention of pred and travel time 
	pred.clear();
	predLink.clear();
	travelTime.clear();
		for (int i=0;i<nodesNum+2;i++)
	{
        pred.push_back(inf);
		predLink.push_back(inf);
		travelTime.push_back(0.0);
	}
	// give the dimention of y and dir
		y.clear();
		dir.clear();
		v.clear();
		pre_x.clear();
	for(int i=0;i<linksNum+1;i++)
	{
		y.push_back(0.0);
		dir.push_back(0.0);
		v.push_back(0.0);
		pre_x.push_back(0.0);

	}
}
//---------------read the LUCE demand data into ods and demands, the file is like "sf.od"
void Network::readLuceDemand(string file, long seekarg)
{
	//cout<<"The ods 's size before the clear is : "<<ods.size()<<endl;
	//system("PAUSE");
	ods.clear();
	demands.clear();
	bushs.clear();
	OD od00;
	Demand demand00;
	bush bush00;
	ods.push_back(od00);
	demands.push_back(demand00);
	bushs.push_back(bush00);

	//cout<<"The od 's size after the clear is : "<<ods.size()<<endl;
	//system("PAUSE");

	ifstream inFile(file,seekarg); //put demand data into "inFile"

	if (!inFile)
	{
		cerr<<"File could not be opened"<<endl;
		exit(1);
	} //end if 

	//read the number of O
	inFile >> odNum;
	vector<double> ll(linksNum+1,0.0);
	cout<<" The Number of D is: "<<odNum<<endl;
	//read the D into ods;
	for(int i=1; i<odNum+1;i++)
	{
		int did;
		int bp;
		int ep;
		OD od;
		inFile>>did>>bp>>ep;
		od.destiID=did;
		od.pairID=bp;
		od.endPID=ep;
		od.OBLflow=ll;
		ods.push_back(od);
	}
	//---ini bush and path
	path p0;
	bush b0;
	bushNode bn0;
	bushlink bl0;
	for(int i=1;i<=nodesNum;i++)
	{
		b0.nodes.push_back(bn0);// ini the bush nodes
		
		p0.traceNode.push_back(0);//set the path traceNode size to nodesNum+1
		p0.traceLink.push_back(0);//set the path traceLink size to linksNum+1
	}
	for(int i=1;i<=linksNum;i++)
	{
		b0.linkStatus.push_back(0);
		b0.linkFlow.push_back(0.0);
		b0.links.push_back(bl0);//ini the bush links
		b0.yluce.push_back(0.0);//ini the luce proportion variables
		b0.eluce.push_back(0.0);
	}
	for(int i=1;i<=odNum;i++)
	{
		bushs.push_back(b0);
	}
	

	//read the number of the pairs
	inFile>>pairNum;
	cout<<"The number of OD pairs in the network is: "<<pairNum<<endl;
	//read the demands into the vector demands
	for(int i=1;i<pairNum+1;i++)
	{
		int origin;
		int destin;
		double dem;
		Demand d;
		inFile>>origin>>destin>>dem;
		d.pairID=i;
		d.destiID=destin;
		d.orignID=origin;
		d.demand=dem;
		d.paths.push_back(p0);//set the 0 of paths as p0
		demands.push_back(d);
	}
	/*
	cout<<"The od's size after reassignment is : "<<ods.size()<<endl;
	system("PAUSE");
	*/
}

Network::~Network()
{

}