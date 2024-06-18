#!/bin/bash
#SBATCH -o %x.out
#SBATCH -t 00:10:00
#SBATCH -N 1
#SBATCH -c 16
#SBATCH -p photon

# Check if an argument is passed, if not, default to 4
if [ $# -eq 0 ]; then
    OMP_NUM_THREADS=4
else
    OMP_NUM_THREADS=$1
fi

export OMP_NUM_THREADS=$OMP_NUM_THREADS
./prog1.elf
