# compile
- 逐次プログラムのコンパイル
  - gcc -O3 test.c -o test

- OpenMPによるスレッド並列プログラムのコンパイル
  - gcc -fopenmp -O3 test.c -o test

- MPIによるプロセス並列プログラムのコンパイル
  - mpicc -O3 test.c -o test

- OpenMPとMPIによるハイブリッド並列プログラムのコンパイル
  - mpicc -fopenmp -O3 test.c -o test


# batch job

## specify a partition:
example:

#SBATCH -p photon
## serial

#!/bin/bash\
#SBATCH -o %x.o%j\
#SBATCH -t 00:10:00\
#SBATCH -N 1\
./test

## openmp

#!/bin/bash \
#SBATCH -o %x.o%j\
#SBATCH -t 00:10:00\
#SBATCH -N 1\
#SBATCH -c 16\
export OMP_NUM_THREADS=4\
./test

## mpi

#!/bin/bash\
#SBATCH -o %x.o%j\
#SBATCH -t 00:10:00\
#SBATCH -N 3\
#SBATCH -c 16\
mpirun -np 48 -npernode 16 ./test

## openmp+mpi

#!/bin/bash\
#SBATCH -o %x.o%j\
#SBATCH -t 00:10:00\
#SBATCH -N 3\
#SBATCH -c 16\
export OMP_NUM_THREADS=8\
mpirun -np 6 -npernode 2 ./test

# ジョブの操作

sbatch test.sh

squeue
