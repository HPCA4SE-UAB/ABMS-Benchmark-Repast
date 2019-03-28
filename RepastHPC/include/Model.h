/* Model.h */
/* 
* Benchmark model for Repast HPC ABMS
* This file is part of the ABMS-Benchmark-FLAME distribution (https://github.com/xxxx).
* Copyright (c) 2018 Universitat Autònoma de Barcelona, Escola Universitària Salesiana de Sarrià
* 
*Based on: Alban Rousset, Bénédicte Herrmann, Christophe Lang, Laurent Philippe
*A survey on parallel and distributed multi-agent systems for high performance comput-
*ing simulations Computer Science Review 22 (2016) 27–46
*
* This program is free software: you can redistribute it and/or modify  
* it under the terms of the GNU General Public License as published by  
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
* 
*  You should have received a copy of the GNU General Public License 
*  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef MODEL
#define MODEL

#include <boost/mpi.hpp>
#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/AgentRequest.h"
#include "repast_hpc/TDataSource.h"
#include "repast_hpc/SVDataSet.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"

#include "Agent.h"

#include <string>


//1. Model parameter selection

//-Size of space where agents move arround
//Select the desired HEIGHT and WIDTH at RepastHPC/include/Model.h
#define HEIGHT 300
#define WIDTH 300


/* Agent Package Provider */
class RepastHPCAgentPackageProvider {
	
private:
    repast::SharedContext<RepastHPCAgent>* agents;
	
public:
	
    RepastHPCAgentPackageProvider(repast::SharedContext<RepastHPCAgent>* agentPtr);
	
    void providePackage(RepastHPCAgent * agent, std::vector<RepastHPCAgentPackage>& out);
	
    void provideContent(repast::AgentRequest req, std::vector<RepastHPCAgentPackage>& out);
	
};

/* Agent Package Receiver */
class RepastHPCAgentPackageReceiver {
	
private:
    repast::SharedContext<RepastHPCAgent>* agents;
	
public:
	
    RepastHPCAgentPackageReceiver(repast::SharedContext<RepastHPCAgent>* agentPtr);
	
    RepastHPCAgent * createAgent(RepastHPCAgentPackage package);
	
    void updateAgent(RepastHPCAgentPackage package);
	
};


/* Data Collection */
class DataSource_AgentTotals : public repast::TDataSource<int>{
private:
	repast::SharedContext<RepastHPCAgent>* context;

public:
	DataSource_AgentTotals(repast::SharedContext<RepastHPCAgent>* c);
	int getData();
};
	

class DataSource_AgentCTotals : public repast::TDataSource<int>{
private:
	repast::SharedContext<RepastHPCAgent>* context;
	
public:
	DataSource_AgentCTotals(repast::SharedContext<RepastHPCAgent>* c);
	int getData();
};


class RepastHPCModel{
	int stopAt;
	int countOfAgents;
	int procPerx;
	int procPery;
	std::string initialAgentsFile;
	std::string initialFFTVectorFile;

	repast::Properties* props;
	repast::SharedContext<RepastHPCAgent> context;
	
	RepastHPCAgentPackageProvider* provider;
	RepastHPCAgentPackageReceiver* receiver;

	repast::SVDataSet* agentValues;
    repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* discreteSpace;
	
public:
	RepastHPCModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
	~RepastHPCModel();
	void init();
	void requestAgents();
	void cancelAgentRequests();
	void removeLocalAgents();
	void printAgentsPosition();
	void doSomething();
	void initSchedule(repast::ScheduleRunner& runner);
	void recordResults();
};


class DataSource_AgentNumber : public repast::TDataSource<int>{
private:
	repast::SharedContext<RepastHPCAgent>* context;
	RepastHPCModel* model;	
public:
	DataSource_AgentNumber(repast::SharedContext<RepastHPCAgent>* c, RepastHPCModel* m);
	int getData();
};


#endif
