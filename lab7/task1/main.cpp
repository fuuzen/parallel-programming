# include "fft/fft.h"
# include "time/time.h"

# include <iostream>
# include <iomanip>
# include <mpi.h>
# include <random>

using namespace std;

//****************************************************************************80

int main ( )

//****************************************************************************80
//
//  Purpose:
//
//    MAIN is the main program for FFT_PARALLEL.
//
//  Discussion:
//
//    The complex data in an N vector is stored as pairs of values in a
//    real vector of length 2*N.
//
//  Modified:
//
//    12 May 2025
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
{
  int nits = 10000;
  int ln2lim = 20;
  double ctime;
  double ctime1;
  double ctime2;
  double error;
  int first;
  double flops;
  double fnm1;
  int i;
  int icase;
  int it;
  int ln2;
  double mflops;
  int n;
  static double seed;
  double sgn;
  double *w;
  double *x;
  double *y;
  double *z;
  double z0;
  double z1;
  int rank;
  int size;
  
  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if ( rank == 0 )
  {
    timestamp ( );
    cout << "\n";
    cout << "FFT_PARALLEL\n";
    cout << "  C++ version\n";
    cout << "\n";
    cout << "  Demonstrate an implementation of the Fast Fourier Transform\n";
    cout << "  of a complex data vector.\n";
//
//  Prepare for tests.
//
    cout << "\n";
    cout << "  Accuracy check:\n";
    cout << "\n";
    cout << "    FFT ( FFT ( X(1:N) ) ) == N * X(1:N)\n";
    cout << "\n";
    cout << "             N      NITS    Error         Time          Time/Call     MFLOPS\n";
    cout << "\n";
  }
//
//  LN2 is the log base 2 of N.  Each increase of LN2 doubles N.
//
  n = size / 2;

  for ( ln2 = 1; ln2 <= ln2lim; ln2++ )
  {
    n = 2 * n;
//
//  Allocate storage for the complex arrays W, X, Y, Z.  
//
//  We handle the complex arithmetic,
//  and store a complex number as a pair of doubles, a complex vector as a doubly
//  dimensioned array whose second dimension is 2. 
//
    if ( rank == 0 )
    {
      x = new double[2*n];
      y = new double[2*n];
      z = new double[2*n];
    }
    w = new double[  n];

    first = 1;

    for ( icase = 0; icase < 2; icase++ )
    {
      if ( rank == 0 )
      {
        if ( first )
        {
          seed  = 331.0;
          mt19937_64 engine(seed);
          uniform_real_distribution<double> dist(0.0, 1.0);
          
          for ( i = 0; i < 2 * n; i = i + 2 )
          {
            z0 = dist(engine);;
            z1 = dist(engine);;
            x[i] = z0;
            z[i] = z0;
            x[i+1] = z1;
            z[i+1] = z1;
          }
        } 
        else
        {
          for ( i = 0; i < 2 * n; i = i + 2 )
          {
            z0 = 0.0;
            z1 = 0.0;
            x[i] = z0;
            z[i] = z0;
            x[i+1] = z1;
            z[i+1] = z1;
          }
        }
      }
//
//  Initialize the sine and cosine tables.
//
      cffti ( n, w );
      MPI_Barrier(MPI_COMM_WORLD);
//
//  Transform forward, back 
//
      if ( first )
      {
        sgn = + 1.0;
        cfft2 ( n, x, y, w, sgn );
        sgn = - 1.0;
        cfft2 ( n, y, x, w, sgn );
// 
//  Results should be same as initial multiplied by N.
//
        MPI_Barrier(MPI_COMM_WORLD);
        if ( rank == 0 )
        {
          fnm1 = 1.0 / ( double ) n;
          error = 0.0;
          for ( i = 0; i < 2 * n; i = i + 2 )
          {
            error = error 
            + pow ( z[i]   - fnm1 * x[i], 2 )
            + pow ( z[i+1] - fnm1 * x[i+1], 2 );
          }
          error = sqrt ( fnm1 * error );
          cout << "  " << setw(12) << n
              << "  " << setw(8) << nits
              << "  " << setw(12) << error;
        }
        
        first = 0;
      }
      else
      {
        ctime1 = cpu_time ( );
        for ( it = 0; it < nits; it++ )
        {
          sgn = + 1.0;
          cfft2 ( n, x, y, w, sgn );
          sgn = - 1.0;
          cfft2 ( n, y, x, w, sgn );
        }
        if ( rank == 0 )
        {
          ctime2 = cpu_time ( );
          ctime = ctime2 - ctime1;

          flops = 2.0 * ( double ) nits * ( 5.0 * ( double ) n * ( double ) ln2 );

          mflops = flops / 1.0E+06 / ctime;

          cout << "  " << setw(12) << ctime
              << "  " << setw(12) << ctime / ( double ) ( 2 * nits )
              << "  " << setw(12) << mflops << "\n";
        }
      }
    }
    if ( ( ln2 % 4 ) == 0 ) 
    {
      nits = nits / 10;
    }
    if ( nits < 1 ) 
    {
      nits = 1;
    }

    if ( rank == 0 )
    {
      delete [] x;
      delete [] y;
      delete [] z;
    }
    delete [] w;
  }

  if ( rank == 0 )
  {
    cout << "\n";
    cout << "FFT_PARALLEL:\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    timestamp ( );
  }
  
  MPI_Finalize();
  return 0;
}
