/* Main.cpp */
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



#include <boost/mpi.hpp>
#include "repast_hpc/RepastProcess.h"
 
#include "Model.h"

/*
* Function:  main 
* --------------------
* main function of RepastHPC benchmark model
* 
* argc,argv: argv[1]: The name of the configuration file, argv[2]: The name of the properties file
*
* returns: 0 
*/

int main(int argc, char** argv){
	std::string configFile = argv[1]; // The name of the configuration file is Arg 1
	std::string propsFile  = argv[2]; // The name of the properties file is Arg 2
	
	boost::mpi::environment env(argc, argv);
	boost::mpi::communicator world;

	boost::mpi::timer time;

	repast::RepastProcess::init(configFile);
	
	RepastHPCModel* model = new RepastHPCModel(propsFile, argc, argv, &world);
	repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
	
	model->init();
	model->initSchedule(runner);
	
	runner.run();
	
	delete model;
	
	repast::RepastProcess::instance()->done();
	
	/* Stop timing and print total time */
        if(world.rank() == 0) std::cout << "Execution time (msecs): " << ((int)(time.elapsed() * 1000.0)) << std::endl;
	
	return 0;
}
