#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16
hostname
date
module load compiler/2023.0.0
module load mkl/2023.0.0
#icpx -qmkl -O -o matmul_BLAS.elf matmul_BLAS.cpp


./matmul_BLAS.elf 1000
