/* Model.cpp */
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




#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/initialize_random.h"
#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Point.h"

#include "Model.h"

/*
 *    Class: RepastHPCAgentPackageProvider  
 * Function: RepastHPCAgentPackageProvider
 * --------------------
 * RepastHPCAgentPackageProvider constructor
 * 
 * agentPtr: agent
 *
 * returns: -
 */
RepastHPCAgentPackageProvider::RepastHPCAgentPackageProvider(repast::SharedContext<RepastHPCAgent>* agentPtr): agents(agentPtr){ }

/*
 *    Class: RepastHPCAgentPackageProvider  
 * Function: providePackage
 * --------------------
 * Builds ans agent package and puts in a vector
 * 
 * agent: agent
 * out: vector of agents packaages
 *
 * returns: -
 */
void RepastHPCAgentPackageProvider::providePackage(RepastHPCAgent * agent, std::vector<RepastHPCAgentPackage>& out){
    repast::AgentId id = agent->getId();
    
    char newm[COM_BUFFER_SIZE];
    agent->getm(newm);
    
    RepastHPCAgentPackage package(id.id(), id.startingRank(), id.agentType(), id.currentRank(), agent->getC(), agent->getTotal(), newm);
    out.push_back(package);
}

/*
 *    Class: RepastHPCAgentPackageProvider  
 * Function: provideContent
 * --------------------
 * Puts a vector of agents into a vector of ageng packages
 * 
 * req: vector of agents
 * out: vector of agents packaages
 *
 * returns: -
 */
void RepastHPCAgentPackageProvider::provideContent(repast::AgentRequest req, std::vector<RepastHPCAgentPackage>& out){
    std::vector<repast::AgentId> ids = req.requestedAgents();
    for(size_t i = 0; i < ids.size(); i++){
        providePackage(agents->getAgent(ids[i]), out);
    }
}

/*
 *    Class: RepastHPCAgentPackageReceiver
 * Function: RepastHPCAgentPackageReceiver 
 * --------------------
 * RepastHPCAgentPackageReceiver constructor
 * 
 * agentPtr: agent
 *
 * returns: -
 */
RepastHPCAgentPackageReceiver::RepastHPCAgentPackageReceiver(repast::SharedContext<RepastHPCAgent>* agentPtr): agents(agentPtr){
}

/*
 *    Class: RepastHPCAgentPackageReceiver
 * Function: RepastHPCAgentPackageReceiver 
 * --------------------
 * RepastHPCAgentPackageReceiver constructor
 * 
 * package: agent package
 *
 * returns: new agent
 */
RepastHPCAgent * RepastHPCAgentPackageReceiver::createAgent(RepastHPCAgentPackage package){
    repast::AgentId id(package.id, package.rank, package.type, package.currentRank);
    return new RepastHPCAgent(id, package.c, package.total, package.m);
}

/*
 *    Class: RepastHPCAgentPackageReceiver
 * Function: updateAgent 
 * --------------------
 * Update agent with data received from a package
 * 
 * package: agent package
 *
 * returns: new agent
 */
void RepastHPCAgentPackageReceiver::updateAgent(RepastHPCAgentPackage package){
    repast::AgentId id(package.id, package.rank, package.type);
    RepastHPCAgent * agent = agents->getAgent(id);
    agent->set(package.currentRank, package.c, package.total);
}

/*
 *    Class: DataSource_AgentTotals
 * Function: DataSource_AgentTotals 
 * --------------------
 * DataSource_AgentTotals constructor
 * 
 * c: context
 *
 * returns: -
 */
DataSource_AgentTotals::DataSource_AgentTotals(repast::SharedContext<RepastHPCAgent>* c) : context(c){ 
}

/*
 *    Class: DataSource_AgentTotals
 * Function: getData 
 * --------------------
 * Get total sum of local agents Total counter
 * 
 * -: -
 *
 * returns: sum
 */
int DataSource_AgentTotals::getData(){
	int sum = 0, s;
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {
		s = (*iter)->getTotal();
		sum+= s;
		iter++;
	}
	return sum;
}

/*
 *    Class: DataSource_AgentCTotals
 * Function: DataSource_AgentCTotals 
 * --------------------
 * DataSource_AgentCTotals constructor
 * 
 * c: context
 *
 * returns: -
 */
DataSource_AgentCTotals::DataSource_AgentCTotals(repast::SharedContext<RepastHPCAgent>* c) : context(c){ 
}

/*
 *    Class: DataSource_AgentCTotals
 * Function: getData 
 * --------------------
 * Get total sum of local agents Total counter
 * 
 * -: -
 *
 * returns: sum
 */
int DataSource_AgentCTotals::getData(){
	int sum = 0, s;
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {
		s = (*iter)->getC();
		sum+= s;
		iter++;
	}
	return sum;
}

/*
 *    Class: RepastHPCModel
 * Function: RepastHPCModel 
 * --------------------
 * RepastHPCModel constructor
 * 
 * propsFile: props file
 * argc:
 * argv:
 * comm: mpi communicator
 *
 * returns: -
 */
RepastHPCModel::RepastHPCModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm){
	props = new repast::Properties(propsFile, argc, argv, comm);
	stopAt = repast::strToInt(props->getProperty("stop.at"));
	countOfAgents = repast::strToInt(props->getProperty("count.of.agents"));

	procPerx = repast::strToInt(props->getProperty("proc.per.x"));
	procPery = repast::strToInt(props->getProperty("proc.per.y"));

	initializeRandom(*props, comm);
	if(repast::RepastProcess::instance()->rank() == 0) props->writeToSVFile("./output/record.csv");
	provider = new RepastHPCAgentPackageProvider(&context);
	receiver = new RepastHPCAgentPackageReceiver(&context);

	repast::Point<double> origin(0,0);
	repast::Point<double> extent(WIDTH, HEIGHT);    
	repast::GridDimensions gd(origin, extent);
    
	std::vector<int> processDims;
	processDims.push_back(procPerx);
	processDims.push_back(procPery); //Nº process = procPerx*procPery
    
	discreteSpace = new repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >("AgentDiscreteSpace", gd, processDims, 10, comm);
	
	std::cout << "RANK " << repast::RepastProcess::instance()->rank() << " BOUNDS: " << discreteSpace->bounds().origin() << " " << discreteSpace->bounds().extents() << std::endl;
    
   	context.addProjection(discreteSpace);
    
	// Data collection
	// Create the data set builder
	std::string fileOutputName("./output/agent_total_data.csv");
	repast::SVDataSetBuilder builder(fileOutputName.c_str(), ",", repast::RepastProcess::instance()->getScheduleRunner().schedule());
	
	// Create the individual data sets to be added to the builder
	DataSource_AgentTotals* agentTotals_DataSource = new DataSource_AgentTotals(&context);
	builder.addDataSource(createSVDataSource("Total", agentTotals_DataSource, std::plus<int>()));

	DataSource_AgentCTotals* agentCTotals_DataSource = new DataSource_AgentCTotals(&context);
	builder.addDataSource(createSVDataSource("C", agentCTotals_DataSource, std::plus<int>()));

	// Use the builder to create the data set
	agentValues = builder.createDataSet();
}

/*
 *    Class: RepastHPCModel
 * Function: ~RepastHPCModel 
 * --------------------
 * RepastHPCModel destructor
 * 
 * -: -
 *
 * returns: -
 */
RepastHPCModel::~RepastHPCModel(){
	delete props;
	delete provider;
	delete receiver;
	
	delete agentValues;
}

/*
 *    Class: RepastHPCModel
 * Function: init 
 * --------------------
 * Creation of agents
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::init(){
	int rank = repast::RepastProcess::instance()->rank();
	int x,y;
	char newm[COM_BUFFER_SIZE] = "123456789"; 
	
	for(int i = 0; i < countOfAgents; i++){

                x = (int)discreteSpace->dimensions().origin().getX() + (repast::Random::instance()->nextDouble())*(int)discreteSpace->dimensions().extents().getX();
                y = (int)discreteSpace->dimensions().origin().getY() + (repast::Random::instance()->nextDouble())*(int)discreteSpace->dimensions().extents().getY();

        	repast::Point<int> initialLocation(x,y);

		repast::AgentId id(i, rank, 0);
		id.currentRank(rank);
		RepastHPCAgent* agent = new RepastHPCAgent(id);
		agent->setm(newm); 
		context.addAgent(agent);
        	discreteSpace->moveTo(id, initialLocation);
	}
}

/*
 *    Class: RepastHPCModel
 * Function: doSomething
 * --------------------
 * Run agents in every simulation step
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::doSomething(){
	int whichRank = 0; 
	bool dierequest = false, reproductionrequest = false;
	char newm[COM_BUFFER_SIZE] = "123456789"; //amv

	if(repast::RepastProcess::instance()->rank() == whichRank) std::cout << " TICK " << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << std::endl;
	
	std::vector<RepastHPCAgent*> agents;
	context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, countOfAgents, agents);
	std::vector<RepastHPCAgent*>::iterator it = agents.begin();
	while(it != agents.end()){
        	std::cout << "Play agent: " << (*it)->getId() << std::endl;
		(*it)->play(&context, discreteSpace);
        	(*it)->compute();
		it++;
	}

    	it = agents.begin();
    	while(it != agents.end()){
		(*it)->move(discreteSpace);
		it++;
    	}
 
   	it = agents.begin();
    	while(it != agents.end()){
		reproductionrequest = (*it)->reproduction(discreteSpace);

		if (reproductionrequest){
			repast::AgentId id = (*it)->getId();
			std::cout << "Agent to reproduct: " << id << std::endl;

			int rank = repast::RepastProcess::instance()->rank();
 			std::vector<int> initialLocation;
			discreteSpace->getLocation((*it)->getId(), initialLocation);
			repast::AgentId newid(countOfAgents, rank, 0);
			countOfAgents++;
			id.currentRank(rank);
			RepastHPCAgent* agent = new RepastHPCAgent(newid);
			agent->setm(newm); //amv - inicialitzo el array
			context.addAgent(agent);
			discreteSpace->moveTo(newid, initialLocation);

			std::cout << "Agent created: " << newid << std::endl;
		}

		it++;
    	}
   	
	it = agents.begin();
    	while(it != agents.end()){
		dierequest = (*it)->die(discreteSpace);
		//std::cout << "dierequest:" << dierequest << std::endl;

		if (dierequest){
			repast::AgentId id = (*it)->getId();
			std::cout << "Agent to die: " << id << std::endl;
			repast::RepastProcess::instance()->agentRemoved(id);
			context.removeAgent(id);
		}

		it++;
    	}

	discreteSpace->balance();
    	repast::RepastProcess::instance()->synchronizeAgentStatus<RepastHPCAgent, RepastHPCAgentPackage, RepastHPCAgentPackageProvider, RepastHPCAgentPackageReceiver>(context, *provider, *receiver, *receiver);
    
    	repast::RepastProcess::instance()->synchronizeProjectionInfo<RepastHPCAgent, RepastHPCAgentPackage, RepastHPCAgentPackageProvider, RepastHPCAgentPackageReceiver>(context, *provider, *receiver, *receiver);

	repast::RepastProcess::instance()->synchronizeAgentStates<RepastHPCAgentPackage, RepastHPCAgentPackageProvider, RepastHPCAgentPackageReceiver>(*provider, *receiver);
    
}

/*
 *    Class: RepastHPCModel
 * Function: initSchedule
 * --------------------
 * Schedulle what to do in every simulation time
 * 
 * runner: scheduler runner identificacion
 *
 * returns: -
 */
void RepastHPCModel::initSchedule(repast::ScheduleRunner& runner){
	runner.scheduleEvent(1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::requestAgents)));
	runner.scheduleEvent(2, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::doSomething)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::recordResults)));
	runner.scheduleStop(stopAt);
	
	// Data collection
	runner.scheduleEvent(1.5, 5, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::record)));
	runner.scheduleEvent(10.6, 10, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
}

/*
 *    Class: RepastHPCModel
 * Function: recordResults
 * --------------------
 * Record results
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::recordResults(){
	if(repast::RepastProcess::instance()->rank() == 0){
		props->putProperty("Result","Passed");
		std::vector<std::string> keyOrder;
		keyOrder.push_back("RunNumber");
		keyOrder.push_back("stop.at");
		keyOrder.push_back("Result");
		props->writeToSVFile("./output/results.csv", keyOrder);
    }
}


	