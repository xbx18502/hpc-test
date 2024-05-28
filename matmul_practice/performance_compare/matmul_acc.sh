#!/bin/bash -x
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 1:00:00

hostname
#. /etc/profile.d/modules.sh
module load nvhpc/23.3
#nvidia-smi
n=1000
./matmul_acc.elf $n
date
