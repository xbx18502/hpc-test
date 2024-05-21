#!/bin/bash -x
#SBATCH -p photon
#SBATCH -o %xout_float.txt
#SBATCH -t 1:00:00

hostname
#. /etc/profile.d/modules.sh
module load nvhpc/23.3
#nvidia-smi
./matmul_acc.elf
date
