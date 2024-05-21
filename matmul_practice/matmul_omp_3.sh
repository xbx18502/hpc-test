#!/bin/bash
#SBATCH -p photon
#SBATCH -o %x.out
#SBATCH -t 00:10:00
#SBATCH -N 1
#SBATCH -c 16
export OMP_NUM_THREADS=16
export I_MPI_DEBUG=5
export I_MPI_HYDRA_BOOTSTRAP=ssh

N=3000
./matmul_omp_3.elf $N

date