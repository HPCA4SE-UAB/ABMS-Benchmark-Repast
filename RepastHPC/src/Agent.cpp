/* Agent.cpp */
/* 
* Benchmark model for RepastHPC ABMS
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



#include "Agent.h"
#include "repast_hpc/Moore2DGridQuery.h"
#include "repast_hpc/Point.h"
#include <fftw3.h>
#include <math.h>
#include <string.h>

/*
 *    Class: RepastHPCAgent  
 * Function: RepastHPCAgent
 * --------------------
 * RepastHPCAgent class constructor
 * 
 * id: agents identificator
 *
 * returns: -
 */
RepastHPCAgent::RepastHPCAgent(repast::AgentId id): id_(id), c(100), total(200){ 
	for (int i=0; i<COM_BUFFER_SIZE; i++)
		m[i]=0;
}

/*
 *    Class: RepastHPCAgent  
 * Function: RepastHPCAgent
 * --------------------
 * RepastHPCAgent class constructor
 * 
 * id: agents identificator
 * newC: initial value of payoff counter when agent cooperates 
 * newTotal: initial value of total payoff counter
 * newm: initial data to data buffer in agents communications
 *
 * returns: -
 */
RepastHPCAgent::RepastHPCAgent(repast::AgentId id, double newC, double newTotal, char newm[]): id_(id), c(newC), total(newTotal){
	for (int i=0; i<COM_BUFFER_SIZE; i++)
		m[i]=newm[i];
}

/*
 *    Class: RepastHPCAgent  
 * Function: ~RepastHPCAgent
 * --------------------
 * RepastHPCAgent class destructor
 * 
 * -: -
 *
 * returns: -
 */
RepastHPCAgent::~RepastHPCAgent(){ 
}

/*
 *    Class: RepastHPCAgent  
 * Function: getm
 * --------------------
 * Get de communication message
 * 
 * newm: array to copy communication message m
 *
 * returns: -
 */
void RepastHPCAgent::getm(char newm[]){
 	for (int i=0; i<COM_BUFFER_SIZE; i++)
		newm[i]=m[i];
}

/*
 *    Class: RepastHPCAgent  
 * Function: set
 * --------------------
 * Set rank, newC and newTotal
 * 
 * currentRank: process rank
 * newC: value of payoff counter when agent cooperates 
 * newTotal: value of total payoff counter
 *
 *
 * returns: -
 */
void RepastHPCAgent::set(int currentRank, double newC, double newTotal){
	id_.currentRank(currentRank);
	c     = newC;
	total = newTotal;
}

/*
 *    Class: RepastHPCAgent  
 * Function: setm
 * --------------------
 * Set de communication message
 * 
 * newm: array to copy communication message m
 *
 * returns: -
 */
void RepastHPCAgent::setm(char newm[]){
 	for (int i=0; i<COM_BUFFER_SIZE; i++)
		m[i]=newm[i];
}

/*
 *    Class: RepastHPCAgent  
 * Function: cooperate
 * --------------------
 * Get if agent wants to cooperate
 * 
 * -: -
 *
 * returns: true: agent wants to cooperate
 */
bool RepastHPCAgent::cooperate(){
	return repast::Random::instance()->nextDouble() < c/total;
}

/*
 *    Class: RepastHPCAgent  
 * Function: frand
 * --------------------
 * Get a random number between 0 and RAND_MAX
 * 
 * -: -
 *
 * returns: random number
 */
double RepastHPCAgent::frand(void) {
	double value;
	value = ((double)rand()/(RAND_MAX));
	return value;
}


/*
 *    Class: RepastHPCAgent  
 * Function: compute 
 * --------------------
 * compute a FFT of a FFT_VECTOR_SIZE vector size
 * 
 * -: No parameters
 *
 * returns: 
 */
void RepastHPCAgent::compute() {
	int i;
	int N = FFT_VECTOR_SIZE; 
	fftw_complex *in, *out;
	fftw_plan p;
	reinterpret_cast<fftw_complex*>(in);
	srand(123456);
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        	        
	for (i = 0; i < N; i++ ) {
		in[i][0] = frand();
		in[i][1] = frand();
	}
        	        							
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); 
	fftw_destroy_plan(p);	
	fftw_free(in); fftw_free(out);
}


/*
 *    Class: RepastHPCAgent  
 * Function: play 
 * --------------------
 * play prisoner’s dilemma with all agents located until RADIUS distance
 *
 * context-: Repast context
 * space: Repast space
 *
 * returns: 
 */
void RepastHPCAgent::play(repast::SharedContext<RepastHPCAgent>* context,
                              repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space){
	std::vector<RepastHPCAgent*> agentsToPlay;
	char compm[COM_BUFFER_SIZE] = "123456789"; 
	char compm1[COM_BUFFER_SIZE]; 
	int i=0;
    
	std::vector<int> agentLoc;
	space->getLocation(id_, agentLoc);
	repast::Point<int> center(agentLoc);
	repast::Moore2DGridQuery<RepastHPCAgent> moore2DQuery(space);
	moore2DQuery.query(center,  RADIOUS, false, agentsToPlay);
    
    
	double cPayoff     = 0;
	double totalPayoff = 0;
	std::vector<RepastHPCAgent*>::iterator agentToPlay = agentsToPlay.begin();
	while(agentToPlay != agentsToPlay.end()){
		std::vector<int> otherLoc;
		space->getLocation((*agentToPlay)->getId(), otherLoc);
		repast::Point<int> otherPoint(otherLoc);

		bool iCooperated = cooperate();                          // Do I cooperate?
		double payoff = (iCooperated ?
			((*agentToPlay)->cooperate() ?  7 : 1) :     // If I cooperated, did my opponent?
			((*agentToPlay)->cooperate() ? 10 : 3));     // If I didn't cooperate, did my opponent?
		if(iCooperated) cPayoff += payoff;
		totalPayoff             += payoff;
		
		agentToPlay++;
		i++;
    	}
	c      += cPayoff;
	total  += totalPayoff;
}

/*
 *    Class: RepastHPCAgent  
 * Function: move 
 * --------------------
 * move agent
 *
 * space: Repast space
 *
 * returns: 
 */
void RepastHPCAgent::move(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space){

	std::vector<int> agentLoc;
	space->getLocation(id_, agentLoc);
	std::vector<int> agentNewLoc;
	int maxx = (int)space->dimensions().origin().getX() + (int)space->dimensions().extents().getX();
	int maxy = (int)space->dimensions().origin().getY() + (int)space->dimensions().extents().getY();
	int minx = (int)space->dimensions().origin().getX();
	int miny = (int)space->dimensions().origin().getY();

	int nextx = agentLoc[0] + (repast::Random::instance()->nextDouble() < 0.5 ? -1 : 1);
	int nexty = agentLoc[1] + (repast::Random::instance()->nextDouble() < 0.5 ? -1 : 1);

	if (nextx > maxx)
		agentNewLoc.push_back(maxx);
	else if (nextx < minx)
		agentNewLoc.push_back(minx);
	else
		agentNewLoc.push_back(nextx);	

	if (nexty > maxy)
		agentNewLoc.push_back(maxy);       
	else if (nexty < miny)
		agentNewLoc.push_back(miny);       
	else
	agentNewLoc.push_back(nexty);      
	space->moveTo(id_,agentNewLoc);
}

/*
 *    Class: RepastHPCAgent  
 * Function: die 
 * --------------------
 * compute death algorithm
 *
 * space: Repast space
 *
 * returns: true: death
 */
bool RepastHPCAgent::die(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space){
	std::vector<int> agentLoc;
	space->getLocation(id_, agentLoc);
	int x = agentLoc[0];
	int y = agentLoc[1];
	int maxx = (int)space->dimensions().origin().getX() + (int)space->dimensions().extents().getX();
	int maxy = (int)space->dimensions().origin().getY() + (int)space->dimensions().extents().getY();
	int minx = (int)space->dimensions().origin().getX();
	int miny = (int)space->dimensions().origin().getY();
	int HEIGHT = maxx - minx;
	int WIDTH = maxy - miny;
	float death_rate_factor = DEATH_RATE * (1 - fmin(1 , sqrt( pow(abs(x-CENTER_DEATH_X),2) + pow(abs(x-CENTER_DEATH_Y),2) )/((HEIGHT+WIDTH)/2)));
	return (repast::Random::instance()->nextDouble() < death_rate_factor ? true : false);
}

/*
 *    Class: RepastHPCAgent  
 * Function: reproduction 
 * --------------------
 * compute birth algorithm
 *
 * space: Repast space
 *
 * returns: true: birth
 */
bool RepastHPCAgent::reproduction(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space){
	std::vector<int> agentLoc;
        space->getLocation(id_, agentLoc);
        int x = agentLoc[0];
        int y = agentLoc[1];
        int maxx = (int)space->dimensions().origin().getX() + (int)space->dimensions().extents().getX();
        int maxy = (int)space->dimensions().origin().getY() + (int)space->dimensions().extents().getY();
        int minx = (int)space->dimensions().origin().getX();
        int miny = (int)space->dimensions().origin().getY();
        int HEIGHT = maxx - minx;
        int WIDTH = maxy - miny;
	float birth_rate_factor = BIRTH_RATE * (1 - fmin(1 , sqrt( pow(abs(x-CENTER_BIRTH_X),2) + pow(abs(y-CENTER_BIRTH_Y),2) )/((HEIGHT+WIDTH)/2)));
	return (repast::Random::instance()->nextDouble() < birth_rate_factor ? true : false);
}

/* Serializable Agent Package Data */

/*
 *
 *    Class: RepastHPCAgentPackage 
 * Function: RepastHPCAgentPackage
 * --------------------
 * RepastHPCAgentPackage constructor
 *
 * -: -
 *
 * returns: -
 */
RepastHPCAgentPackage::RepastHPCAgentPackage(){ 
}

/*
 *    Class: RepastHPCAgentPackage 
 * Function: RepastHPCAgentPackage
 * --------------------
 * RepastHPCAgentPackage constructor
 *
 * _id : agent id
 * _rank : process rank 
 * _type : type of agent
 * _currentRank : current process rank 
 * _c: value of payoff counter when agent cooperates 
 * _total: value of total payoff counter
 * _m: communication message
 *
 * returns: -
 */
RepastHPCAgentPackage::RepastHPCAgentPackage(int _id, int _rank, int _type, int _currentRank, double _c, double _total, char _m[]):
id(_id), rank(_rank), type(_type), currentRank(_currentRank), c(_c), total(_total){ 
	for (int i=0; i<COM_BUFFER_SIZE; i++)
		m[i]=_m[i];
}