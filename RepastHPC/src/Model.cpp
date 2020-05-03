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

fftw_complex	*in = nullptr;


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
    
    RepastHPCAgentPackage package(id.id(), id.startingRank(), id.agentType(), id.currentRank(), agent->getC(), agent->getTotal(), newm, agent->getN());
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
    return new RepastHPCAgent(id, package.c, package.total, package.m, package.N, in);
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
 *    Class: DataSource_AgentNumber
 * Function: DataSource_AgentNumber 
 * --------------------
 * DataSource_AgentNumber constructor
 * 
 * c: context
 *
 * returns: -
 */
DataSource_AgentNumber::DataSource_AgentNumber(repast::SharedContext<RepastHPCAgent>* c, RepastHPCModel* m) : context(c), model(m){ 
}


/*
 *    Class: DataSource_AgentNumber
 * Function: getData 
 * --------------------
 * Get total sum of local number of agents counter
 * 
 * -: -
 *
 * returns: sum
 */
int DataSource_AgentNumber::getData(){
	int sum = 0;
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {
		sum++;
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

	procPerx = repast::strToInt(props->getProperty("proc.per.x"));
	procPery = repast::strToInt(props->getProperty("proc.per.y"));

	initialAgentsFile = props->getProperty("initial.agents.file");
	initialFFTVectorFile = props->getProperty("initial.fft.vector.file");
	
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

	DataSource_AgentNumber* agentNumber_DataSource = new DataSource_AgentNumber(&context, this);
	builder.addDataSource(createSVDataSource("NumAgents", agentNumber_DataSource, std::plus<int>()));

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
	delete in;
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
	int x,y,z;
	char newm[COM_BUFFER_SIZE] = "123456789";
	FILE *fp;
	u_int32_t idg; 

        //Load fft vector file
	int i;

        fp = fopen(initialFFTVectorFile.c_str(),"r");
        if( feof(fp) ) return ;

        fscanf(fp, "%d", &N);

        reinterpret_cast<fftw_complex*>(in);
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

        for (i = 0; i < N; i++ ) {
                fscanf(fp, "%lf %lf", &in[i][0], &in[i][1]);
                if( feof(fp) ) {
                        break ;
                }
        }

        fclose(fp);

	float xmin = discreteSpace->dimensions().origin().getX();
	float ymin = discreteSpace->dimensions().origin().getY();
	float xmax = discreteSpace->dimensions().origin().getX() + discreteSpace->dimensions().extents().getX();
	float ymax = discreteSpace->dimensions().origin().getY() + discreteSpace->dimensions().extents().getY();
	countOfAgents = 0;

	fp = fopen(initialAgentsFile.c_str(),"r");

	while(1) {
		fscanf(fp, "%u %d %d %d", &idg, &x, &y, &z);
		if( feof(fp) ) { 
			break ;
		}
      
		if ( ( x >= xmin) && (x < xmax) && (y >= ymin) && (y < ymax) ){
	                repast::Point<int> initialLocation(x,y);
        	        repast::AgentId id(countOfAgents, rank, 0);
                	id.currentRank(rank);
                	RepastHPCAgent* agent = new RepastHPCAgent(id, N, in);
                	agent->setm(newm); 
                	context.addAgent(agent);
                	discreteSpace->moveTo(id, initialLocation);
			countOfAgents++;
			//printf("rank %d(%d) 8: %d %d \n", rank, idg, x, y);
		}

	}	
	fclose(fp);
}


/*
 *    Class: RepastHPCModel
 * Function: printAgentsPosition 
 * --------------------
 * Print agents position
 * 
 * -: -
 *
 * returns: -
 */
void RepastHPCModel::printAgentsPosition(){
	int rank = repast::RepastProcess::instance()->rank();

        std::vector<RepastHPCAgent*> agents;
        //context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, countOfAgents, agents);
        context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, agents);
        std::vector<RepastHPCAgent*>::iterator it = agents.begin();
        while(it != agents.end()){
		std::vector<int> agentLoc;
		discreteSpace->getLocation((*it)->getId(), agentLoc);
		repast::Point<int> agentLocation(agentLoc);
		std::cout << "T(" << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << ")" << "P(" << rank << ") " << (*it)->getId() << " " << agentLocation << std::endl;
                it++;
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
	//context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, countOfAgents, agents);
	context.selectAgents(repast::SharedContext<RepastHPCAgent>::LOCAL, agents);
	std::vector<RepastHPCAgent*>::iterator it = agents.begin();
	while(it != agents.end()){
        	//std::cout << "Play agent: " << (*it)->getId() << std::endl;
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
			//std::cout << "Agent to reproduct: " << id << std::endl;

			int rank = repast::RepastProcess::instance()->rank();
 			std::vector<int> initialLocation;
			discreteSpace->getLocation((*it)->getId(), initialLocation);
			repast::AgentId newid(countOfAgents, rank, 0);
			countOfAgents++;
			id.currentRank(rank);
			RepastHPCAgent* agent = new RepastHPCAgent(newid, N, in);
			agent->setm(newm); 
			context.addAgent(agent);
			discreteSpace->moveTo(newid, initialLocation);

			//std::cout << "Agent created: " << newid << std::endl;
		}

		it++;
    	}
   	
	it = agents.begin();
    	while(it != agents.end()){
		dierequest = (*it)->die(discreteSpace);
		//std::cout << "dierequest:" << dierequest << std::endl;

		if (dierequest){
			repast::AgentId id = (*it)->getId();
			//std::cout << "Agent to die: " << id << std::endl;
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
	runner.scheduleEvent(2, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::doSomething)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::recordResults)));
	runner.scheduleStop(stopAt);
	
	// Data collection
	runner.scheduleEvent(1.5, 5, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::record)));
	runner.scheduleEvent(10.6, 10, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::record)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
	//runner.scheduleEvent(3, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::printAgentsPosition)));
	//runner.scheduleEvent(50, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::printAgentsPosition)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCModel> (this, &RepastHPCModel::printAgentsPosition)));
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


