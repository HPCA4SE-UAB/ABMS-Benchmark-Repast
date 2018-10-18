/* Demo_03_Agent.h */

#ifndef DEMO_03_AGENT
#define DEMO_03_AGENT

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"


#define COM_BUFFER_SIZE 256
//#define COM_BUFFER_SIZE 64
//#define COM_BUFFER_SIZE 32
//#define COM_BUFFER_SIZE 16

#define FFT_VECTOR_SIZE 16384
//#define FFT_VECTOR_SIZE 512
//#define FFT_VECTOR_SIZE 16


/* Agents */
class RepastHPCDemoAgent{
	
private:
    repast::AgentId   id_;
    double              c;
    double          total;
    char 				m[COM_BUFFER_SIZE]; //amv
	
public:
    RepastHPCDemoAgent(repast::AgentId id);
	RepastHPCDemoAgent(){}
    RepastHPCDemoAgent(repast::AgentId id, double newC, double newTotal, char newm[]);
	
    ~RepastHPCDemoAgent();
	
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
    void play(repast::SharedContext<RepastHPCDemoAgent>* context,
              repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* space);    // Choose three other agents from the given context and see if they cooperate or not
    void move(repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* space);
    bool die(repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* space);
    bool reproduction(repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* space);
    
};

/* Serializable Agent Package */
struct RepastHPCDemoAgentPackage {
	
public:
    int    id;
    int    rank;
    int    type;
    int    currentRank;
    double c;
    double total;
    char	m[COM_BUFFER_SIZE]; //amv
	
    /* Constructors */
    RepastHPCDemoAgentPackage(); // For serialization
    RepastHPCDemoAgentPackage(int _id, int _rank, int _type, int _currentRank, double _c, double _total, char _m[]);
	
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
