#!/bin/bash
#SBATCH -p blast
. /etc/profile.d/modules.sh
. /home/share/intel/oneapi/vtune/2024.0/env/vars.sh
module load compiler/2024.0.2 mkl/2024.0 mpi/2021.11
# 環境変数確認用
# env
# SLURMのジョブ番号とIntelMPIのランク番号で一意な出力先ディレクトリ名を生成している
# $@ はこのスクリプトを実行する際の引数全体（この場合は ./a.out）
#vtune --collect hotspots -r result_${SLURM_JOBID}_${PMI_RANK} ./matmul_omp_mpi.elf 1000
vtune --collect hpc-performance -r result_${SLURM_JOBID}_${PMI_RANK} ./matmul_omp_mpi.elf 1000
#memory-access

