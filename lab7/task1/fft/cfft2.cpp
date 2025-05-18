# include "fft.h"
# include <mpi.h>
# include <functional>

//****************************************************************************80

void cfft2 ( int n, double x[], double y[], double w[], double sgn )

//****************************************************************************80
//
//  Purpose:
//
//    CFFT2 performs a complex Fast Fourier Transform.
//
//  Modified:
//
//    15 May 2025
//
//  Author:
//
//    Original C version by Wesley Petersen.
//    Original C++ version by John Burkardt.
//    Parallel version by fuuzen.
//
//  Reference:
//
//    Wesley Petersen, Peter Arbenz, 
//    Introduction to Parallel Computing - A practical guide with examples in C,
//    Oxford University Press,
//    ISBN: 0-19-851576-6,
//    LC: QA76.58.P47.
//
//  Parameters:
//
//    Input, int N, the size of the array to be transformed.
//
//    Input/output, double X[2*N], the data to be transformed.  
//    On output, the contents of X have been overwritten by work information.
//
//    Output, double Y[2*N], the forward or backward FFT of X.
//
//    Input, double W[N], a table of sines and cosines.
//
//    Input, double SGN, is +1 for a "forward" FFT and -1 for a "backward" FFT.
//
{
  int j;
  int m;
  int mj;
  int mj2;
  int tgle;
  int p;
  int np;
  int rank;
  int rk;
  double *x_local;
  double *y_local;
  double *buf;
  double *a;
  double *b;
  double *c;
  int *idx;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  np = n / p;
  x_local = new double[2*np];
  y_local = new double[2*np];
  buf = new double[2*np];
  
  if ( rank == 0 )
  {
    idx = new int[n];
    idx[0] = 0;
    
    std::function<void(int[], int, int)> f = [&f](int a[], int size, int k) {
      a[size / 2] = a[0] + k;
      if (size > 2) {
        f(a, size/2, k*2);
        f(a+size/2, size/2, k*2);
      }
    };

    f(idx, n, 1);
    
    for ( int j = 0; j < np; ++j)
    {
      x_local[j + 0] = x[idx[j]*2 + 0];
      x_local[j + 1] = x[idx[j]*2 + 1];
    }

    for ( int i = 1; i < p; ++i)
    {
      for ( int j = 0; j < np; ++j)
      {
        MPI_Send(x+idx[i*np+j]*2, 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      }
    }

    delete [] idx;
  }
  else
  {
    for ( int j = 0; j < np; ++j)
    {
      MPI_Recv(x_local+j*2, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  m = ( int ) ( log ( ( double ) n ) / log ( 1.99 ) );
  mj2 = 1;
//
//  Toggling switch for work array.
//
  tgle = 0;

  for ( j = 0; j < m; j++ )
  {
    MPI_Barrier(MPI_COMM_WORLD);
    mj = mj2;
    mj2 = mj2 * 2;
    
    if ( mj2 <= np )  // scene 1
    {
      a = tgle ? y_local : x_local;
      b = nullptr;
      c = tgle ? x_local : y_local;
    }
    else if ( ( rank * np ) % mj2 < mj )  // scene 2
    {
      rk = rank + mj / np;
      a = tgle ? y_local : x_local;
      b = buf;
      c = tgle ? x_local : y_local;

      MPI_Send(a, 2*np, MPI_DOUBLE, rk, rank, MPI_COMM_WORLD);
      MPI_Recv(b, 2*np, MPI_DOUBLE, rk, rk, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else  // scene 3
    {
      rk = rank - mj / np;
      a = buf;
      b = tgle ? y_local : x_local;
      c = tgle ? x_local : y_local;

      MPI_Recv(a, 2*np, MPI_DOUBLE, rk, rk, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(b, 2*np, MPI_DOUBLE, rk, rank, MPI_COMM_WORLD);
    }
    
    step ( n, mj, a, b, c, w, sgn, np, rank );

    tgle = tgle ? 0 : 1;
  }

  MPI_Gather(tgle ? y_local : x_local, 2 * np, MPI_DOUBLE, y, 2 * np, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  // MPI_Barrier(MPI_COMM_WORLD);
  // printf("%d finish\n", rank);
  // MPI_Barrier(MPI_COMM_WORLD);

  delete [] x_local;
  delete [] y_local;
  delete [] buf;
  return;
}