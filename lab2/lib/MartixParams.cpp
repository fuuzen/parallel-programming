#include <mpi.h>
#include "lib.h"

MPI_Datatype create_matrix_params_type() {
  MPI_Datatype mpi_matrix_params_type;
  int block_lengths[4] = {1, 1, 1, 1};
  MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
  MPI_Aint displacements[4];
  
  MatrixParams dummy;
  MPI_Aint base_address;
  MPI_Get_address(&dummy, &base_address);
  MPI_Get_address(&dummy.m, &displacements[0]);
  MPI_Get_address(&dummy.n, &displacements[1]);
  MPI_Get_address(&dummy.k, &displacements[2]);
  MPI_Get_address(&dummy.local_m, &displacements[3]);
  
  for (int i = 0; i < 4; i++) {
    displacements[i] = MPI_Aint_diff(displacements[i], base_address);
  }
  
  MPI_Type_create_struct(4, block_lengths, displacements, types, &mpi_matrix_params_type);
  MPI_Type_commit(&mpi_matrix_params_type);
  
  return mpi_matrix_params_type;
}