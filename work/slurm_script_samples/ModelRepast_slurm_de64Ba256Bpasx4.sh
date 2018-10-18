#!/bin/bash
#SBATCH --partition=nodo.q
#SBATCH --nodes=3
#SBATCH --ntasks=32                  # Number of MPI ranks
#SBATCH --exclusive
#SBATCH --time=10:00:00
#SBATCH --output=output/outputslurm.%N.%j.out
#SBATCH --error=output/outputslurm.%N.%j.err
#SBATCH --mail-type=END
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=amoreno@euss.cat
 
 
pwd
hostname
date

echo "Running Demo_03_Step_03 of Repast on  $SLURM_JOB_NUM_NODES nodes with $SLURM_NTASKS tasks, each with $SLURM_CPUS_PER_TASK cores."


# Enable modules
#source /opt/Modules/3.2.9/init/Modules4bash.sh
 
# Load modules
# module load gcc/4.9.1
# module load mpich/3.2
module load gcc/7.2.0
module load openmpi/1.10.2 


# Program to execute
export TAU_MAKEFILE=/home/caos/amoreno/sfw/tau-2.26.3/x86_64/lib/Makefile.tau-mpi-pdt
export TAU_OPTIONS=-optCompInst
export PATH=/home/caos/amoreno/sfw/tau-2.26.3/x86_64/bin/:$PATH
export TAU_COMM_MATRIX=1
export LD_LIBRARY_PATH=/home/caos/amoreno/sfw/repast_hpc-2.2.0/lib/:/home/caos/amoreno/sfw/Boost/Boost_1.61/lib/:$LD_LIBRARY_PATH
#export TAU_TRACE=1
for a in 1 2;
do mpiexec -n 32 /home/caos/amoreno/AgentsModels/Model_Repast_tutorial_AlbanModel/SRC/work/bin/Demo_03_0$a.exe /home/caos/amoreno/AgentsModels/Model_Repast_tutorial_AlbanModel/SRC/work/props/config.props /home/caos/amoreno/AgentsModels/Model_Repast_tutorial_AlbanModel/SRC/work/props/model.props6 >output/sortida$a.txt;
pprof -p > /home/caos/amoreno/AgentsModels/Model_Repast_tutorial_AlbanModel/SRC/work/output/pprof_out$a;
done

date

