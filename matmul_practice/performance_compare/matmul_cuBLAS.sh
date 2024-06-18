#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16

hostname
module load cuda/12.0.0

./matmul_cuBLAS.elf $1
date