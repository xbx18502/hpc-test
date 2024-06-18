#!/bin/bash -x
#SBATCH -p fire
#SBATCH -o %x.out
#SBATCH -t 1:00:00

hostname
#. /etc/profile.d/modules.sh
module load nvhpc/23.3
#nvidia-smi

./matmul_acc.elf $1
date
