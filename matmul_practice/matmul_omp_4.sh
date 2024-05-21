#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x_4.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16
export OMP_NUM_THREADS=4
export I_MPI_DEBUG=5
export I_MPI_HYDRA_BOOTSTRAP=ssh

N=1000
./matmul_omp_4.elf $N

date