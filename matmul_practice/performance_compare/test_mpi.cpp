#include <stdio.h>
#include <mpi.h>
#include <omp.h>

int main (int argc, char **argv)
{
  int rank, size;
  int name_len;
  char name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init (&argc, &argv);/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);/* get number of processes */
  MPI_Get_processor_name(name, &name_len);
#pragma omp parallel
  {
        printf( "Hello, parallel world %d / %d : %s : %d / %d\n", rank, size, name, omp_get_thread_num(), omp_get_num_threads()); fflush(stdout);
  }
  MPI_Finalize();
  return 0;
}
/*
module load compiler/2024.0.2 mpi/2021.11
mpiicx -O -qopenmp test_mpi.c -o test_mpi.elf


*/