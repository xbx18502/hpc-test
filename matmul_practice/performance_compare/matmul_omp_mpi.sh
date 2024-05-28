#!/bin/bash
#SBATCH -p photon
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
hostname
date
. /etc/profile.d/modules.sh
module load compiler/2024.0.2 mpi/2021.11

export I_MPI_DEBUG=5
export OMP_NUM_THREADS=16
export I_MPI_HYDRA_BOOTSTRAP=ssh
n=7000
mpiexec.hydra -n 4 ./matmul_omp_mpi.elf $n


