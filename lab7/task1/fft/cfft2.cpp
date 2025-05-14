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
        MPI_Send(x+idx[i*np+j]*2, 2, MPI_DOUBLE, i, j, MPI_COMM_WORLD);
      }
    }

    delete [] idx;
  }
  else
  {
    for ( int j = 0; j < np; ++j)
    {
      MPI_Recv(x_local, 2, MPI_DOUBLE, 0, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  m = ( int ) ( log ( ( double ) n ) / log ( 1.99 ) );
  mj = 1;
  mj2 = 2;
//
//  Toggling switch for work array.
//
  tgle = 1;
  step ( n, mj, x, buf, y, w, sgn, p, rank );

  if ( n == 2 )
  {
    return;
  }

  for ( j = 0; j < m - 2; j++ )
  {
    mj = mj2;
    mj2 = mj2 * 2;
    
    if ( mj2 > np ) {
      if ( ( rank * np ) % mj2 <= mj )
      {
        rk = rank + mj / np;
        MPI_Send(tgle ? y_local : x_local, 2*np, MPI_DOUBLE, rk, rank, MPI_COMM_WORLD);
        MPI_Recv(buf, 2*np, MPI_DOUBLE, rk, rk, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
      else
      {
        rk = rank - mj / np;
        MPI_Recv(buf, 2*np, MPI_DOUBLE, rk, rk, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(tgle ? y_local : x_local, 2*np, MPI_DOUBLE, rk, rank, MPI_COMM_WORLD);
      }
    }

    if ( tgle )
    {
      step ( n, mj, y_local, buf, x_local, w, sgn, p, rank );
      tgle = 0;
    }
    else
    {
      step ( n, mj, x_local, buf, y_local, w, sgn, p, rank );
      tgle = 1;
    }
  }
//
//  Last pass thru data: move y to x if needed 
//
  if ( tgle ) 
  {
    ccopy ( n/p, y_local, x_local );
  }

  mj = n / 2;
  step ( n, mj, x_local, buf, y_local, w, sgn, p, rank );
  
  MPI_Gather(x_local, 2 * n / p, MPI_DOUBLE, x, 2 * n / p, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  delete [] x_local;
  delete [] y_local;
  return;
}