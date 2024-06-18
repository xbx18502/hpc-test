#!/bin/bash
#SBATCH -p photon
#sbatch -o %x.out
hostname
date
. /etc/profile.d/modules.sh
module load compiler/2024.0.2 mkl/2024.0 mpi/2021.11
. /home/share/intel/oneapi/vtune/2024.0/env/vars.sh
# 普通に実行する場合
./sgemm 1000
# vtuneでプロファイルを取りながら実行する場合（出力先ディレクトリ名はジョブIDを使って一意なものを生成）
# vtune -collect hotspots -r ./result_sgemm_${SLURM_JOBID} ./matmul_omp.elf 1000
vtune -collect memory-access -r ./result_sgemm_${SLURM_JOBID} ./matmul_omp.elf 1000