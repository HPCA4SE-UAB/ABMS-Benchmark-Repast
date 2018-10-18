/* Agent.h */
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



#ifndef AGENT
#define AGENT

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"

//1. Model parameter selection

//-Size of communication message:
// Select the desired value of COM_BUFFER_SIZE at RepastHPC/include/Agent.h
#define COM_BUFFER_SIZE 256
//#define COM_BUFFER_SIZE 64
//#define COM_BUFFER_SIZE 32
//#define COM_BUFFER_SIZE 16

//-Size of FFT vector
//Select the desired value of FFT_VECTOR_SIZE at RepastHPC/include/Agent.h
#define FFT_VECTOR_SIZE 16384
//#define FFT_VECTOR_SIZE 512
//#define FFT_VECTOR_SIZE 16

//-Interaction radius between agents
//Select the desired radius at RepastHPC/include/Agent.h
#define RADIOUS 10

//-birth_rate: birth probability, inteval [0,1], 0: no birth, 1: 100% probability of birth
//Select the desired value at RepastHPC/include/Agent.h
#define DEATH_RATE 0.2

//-death_rate: death probability, inteval [0,1], 0: no death, 1: 100% probability of death
//Select the desired value at RepastHPC/include/Agent.h
#define BIRTH_RATE 0.2

//-Birth rate center, position where birth_rate aplies totaly, from this point it goes down lineally until borders of space
//Select the desired center_birth_x and center_birth_y at RepastHPC/include/Agent.h
#define CENTER_BIRTH_X 150
#define CENTER_BIRTH_Y 150

//-Death rate center, position where Death_rate aplies totaly, from this point it goes down lineally until borders of space
//Select the desired center_death_x and center_death_y at RepastHPC/include/Agent.h
#define CENTER_DEATH_X 50
#define CENTER_DEATH_Y 50


/* Agents */
class RepastHPCAgent{
	
private:
    repast::AgentId   id_;
    double              c;
    double          total;
    char 				m[COM_BUFFER_SIZE]; //amv
	
public:
    RepastHPCAgent(repast::AgentId id);
	RepastHPCAgent(){}
    RepastHPCAgent(repast::AgentId id, double newC, double newTotal, char newm[]);
	
    ~RepastHPCAgent();
	
    /* Required Getters */
    virtual repast::AgentId& getId(){                   return id_;    }
    virtual const repast::AgentId& getId() const {      return id_;    }
    void getm(char newm[]);
	
    /* Getters specific to this kind of Agent */
    double getC(){                                      return c;      }
    double getTotal(){                                  return total;  }
	
    /* Setter */
    void set(int currentRank, double newC, double newTotal);
    void setm(char newm[]);
	
    /* Actions */
    double frand();    
    void compute();
    bool cooperate();                                                 // Will indicate whether the agent cooperates or not; probability determined by = c / total
    void play(repast::SharedContext<RepastHPCAgent>* context,
              repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);    // Choose three other agents from the given context and see if they cooperate or not
    void move(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
    bool die(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
    bool reproduction(repast::SharedDiscreteSpace<RepastHPCAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCAgent> >* space);
    
};

/* Serializable Agent Package */
struct RepastHPCAgentPackage {
	
public:
    int    id;
    int    rank;
    int    type;
    int    currentRank;
    double c;
    double total;
    char	m[COM_BUFFER_SIZE]; //amv
	
    /* Constructors */
    RepastHPCAgentPackage(); // For serialization
    RepastHPCAgentPackage(int _id, int _rank, int _type, int _currentRank, double _c, double _total, char _m[]);
	
    /* For archive packaging */
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & id;
        ar & rank;
        ar & type;
        ar & currentRank;
        ar & c;
        ar & total;
        ar & m;
    }
};


#endif
