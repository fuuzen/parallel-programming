#include "main.h"
#include <cstring>


int main ( int argc, char *argv[] )
{
  if (argc != 4) {
    printf("Usage: %s M N num_threads\n", argv[0]);
    return 1;
  }

  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  int num_threads = atoi(argv[3]);

  double diff;
  double epsilon = 0.001;
  int i;
  int iterations;
  int iterations_print;
  int j;
  double mean = 0.0;
  double * u = (double *)calloc(M * N, sizeof(double));
  double * w = (double *)calloc(M * N, sizeof(double));
  double wtime;

  printf ( "\n" );
  printf ( "HEATED_PLATE_OPENMP\n" );
  printf ( "  C++/Pthreads version\n" );
  printf ( "  A program to solve for the steady state temperature distribution\n" );
  printf ( "  over a rectangular plate.\n" );
  printf ( "\n" );
  printf ( "  Spatial grid of %d by %d points.\n", M, N );
  printf ( "  The iteration will be repeated until the change is <= %e\n", epsilon ); 
  printf ( "  Number of threads =              %d\n", num_threads );

  ArgWithMean arg_with_mean;
  arg_with_mean.M = M;
  arg_with_mean.N = N;
  arg_with_mean.w = w;
  arg_with_mean.u = u;
  arg_with_mean.mean = &mean;
  ArgWithDiff arg_with_diff;
  arg_with_diff.M = M;
  arg_with_diff.N = N;
  arg_with_diff.w = w;
  arg_with_diff.u = u;
  arg_with_diff.diff = &diff;

  /*
    Set the boundary values, which don't change. 
  */
  parallel_for(1, M-1, 1, [](int i, void* arg) -> void {
    ArgWithMean* a = (ArgWithMean*)arg;
    a->asgn_w(i, a->N-1, 100.0);
    a->asgn_w(i, 0, 100.0);
  }, &arg_with_mean, num_threads);

  parallel_for(0, N, 1, [](int j, void* arg) -> void {
    ArgWithMean* a = (ArgWithMean*)arg;
    a->asgn_w(a->M-1, j, 100.0);
    a->asgn_w(0, j, 0.0);
  }, &arg_with_mean, num_threads);

  /*
    Average the boundary values, to come up with a reasonable
    initial value for the interior.
  */

  parallel_for(1, M-1, 1, [](int i, void* arg) -> void {
    ArgWithMean* a = (ArgWithMean*)arg;
    a->sub_mean += a->w_at(i, 0) + a->w_at(i, a->N-1);
  }, &arg_with_mean, num_threads, true);

  printf ( "  MEAN = %f\n", mean );

  parallel_for(0, N, 1, [](int j, void* arg) -> void {
    ArgWithMean* a = (ArgWithMean*)arg;
    a->sub_mean += a->w_at(a->M-1, j) + a->w_at(0, j);
  }, &arg_with_mean, num_threads, true);

  /*
    OpenMP note:
    You cannot normalize MEAN inside the parallel region.  It
    only gets its correct value once you leave the parallel region.
    So we interrupt the parallel region, set MEAN, and go back in.
  */
  mean = mean / ( double ) ( 2 * M + 2 * N - 4 );
  printf ( "\n" );
  printf ( "  MEAN = %f\n", mean );

  /* 
    Initialize the interior solution to the mean value.
  */
  parallel_for(1, M-1, 1, [](int i, void* arg) -> void {
    ArgWithMean* a = (ArgWithMean*)arg;
    for ( int j = 1; j < a->N-1; j++ ) {
      a->asgn_w(i, j, *(a->mean));
    }
  }, &arg_with_mean, num_threads);

  /*
    iterate until the new solution W differs from the old solution U
    by no more than EPSILON.
  */
  iterations = 0;
  iterations_print = 1;
  printf ( "\n" );
  printf ( " Iteration  Change\n" );
  printf ( "\n" );

  Timer *timer = new Timer();  // 开始计时

  diff = epsilon;

  while ( epsilon <= diff ) {
    /*
      Save the old solution in U.
    */
    memcpy(u, w,  M * N * sizeof(double));
    
    /*
      Determine the new estimate of the solution at the interior points.
      The new solution W is the average of north, south, east and west neighbors.
    */
    parallel_for(1, M-1, 1, [](int i, void* arg) -> void {
      ArgWithMean* a = (ArgWithMean*)arg;
      for ( int j = 1; j < a->N-1; j++ ) {
        a->asgn_w(i, j, 
          ( a->u_at(i-1, j) + a->u_at(i+1, j) + a->u_at(i, j-1) + a->u_at(i, j+1) ) / 4.0
        );
      }
    }, &arg_with_mean, num_threads);

    /*
      C and C++ cannot compute a maximum as a reduction operation.

      Therefore, we define a private variable `sub_diff` for each thread.
      Once they have all computed their values, we use a CRITICAL section
      to update DIFF.
    */
    diff = 0.0;

    parallel_for(1, M-1, 1, [](int i, void* arg) -> void {
      ArgWithDiff* a = (ArgWithDiff*)arg;
      for ( int j = 1; j < a->N-1; j++ ) {
        double diff = fabs ( a->w_at(i, j) - a->u_at(i, j) );
        if ( a->sub_diff < diff ) {
          a->sub_diff = diff;
        }
      }
    }, &arg_with_diff, num_threads, true);

    iterations++;
    if ( iterations == iterations_print ) {
      printf ( "  %8d  %f\n", iterations, diff );
      iterations_print = 2 * iterations_print;
    }
  } 

  printf ( "\n" );
  printf ( "  %8d  %f\n", iterations, diff );
  printf ( "\n" );
  printf ( "  Error tolerance achieved.\n" );
  printf ( "  Wallclock time = ");
  delete timer;  // 结束计时
  printf ( " sec\n" );
  /*
    Terminate.
  */
  printf ( "\n" );
  printf ( "HEATED_PLATE_OPENMP:\n" );
  printf ( "  Normal end of execution.\n" );

  return 0;
}
