#!/bin/bash
#SBATCH --partition=nodo.q
#SBATCH --nodes=1
#SBATCH --ntasks=12                  # Number of MPI ranks
#SBATCH --exclusive
#SBATCH --output=output/outputslurm.%N.%j.out
#SBATCH --error=output/outputslurm.%N.%j.err
#SBATCH --mail-type=END
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=xxxxx@bbbb.ffff
 
 
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
export LD_LIBRARY_PATH=/home/caos/amoreno/sfw/lib/:/home/caos/amoreno/sfw/Boost/Boost_1.61/lib/:$LD_LIBRARY_PATH
#export TAU_TRACE=1
mpiexec -n 12 bin/Model.exe props/config.props props/model.propsi >output/sortida1.txt ;
pprof -p > output/pprof_out1;
date

