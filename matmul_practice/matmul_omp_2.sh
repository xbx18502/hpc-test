#!/bin/bash
#SBATCH -p photon
#SBATCH -o %x.out
#SBATCH -t 00:00:05
#SBATCH -N 1
#SBATCH -c 16
export OMP_NUM_THREADS=4
export I_MPI_DEBUG=5
export I_MPI_HYDRA_BOOTSTRAP=ssh

./matmul_omp.elf