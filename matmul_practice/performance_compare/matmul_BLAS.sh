#!/bin/bash
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 00:30:00
#SBATCH -N 1
#SBATCH -c 16
hostname
date
n=7000
./matmul_BLAS.elf $n
