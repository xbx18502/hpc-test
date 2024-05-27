#!/bin/bash
#SBATCH -p photon
#SBATCH -o %x.out
#SBATCH -t 00:30:00
hostname
date
. /etc/profile.d/modules.sh
module load compiler/2024.0.2 mpi/2021.11

export I_MPI_DEBUG=5
export OMP_NUM_THREADS=2
export I_MPI_HYDRA_BOOTSTRAP=ssh

mpiexec.hydra -n 2 ./matmul_omp_mpi.elf


