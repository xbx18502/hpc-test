#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16
hostname
date
export OMP_NUM_THREADS=16
export I_MPI_DEBUG=5
export I_MPI_HYDRA_BOOTSTRAP=ssh


./matmul_omp_2.elf 5000

