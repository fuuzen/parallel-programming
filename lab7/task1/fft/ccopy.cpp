//****************************************************************************80

void ccopy ( int n, double x[], double y[] )

//****************************************************************************80
//
//  Purpose:
//
//    CCOPY copies a complex vector.
//
//  Discussion:
//
//    The "complex" vector A[N] is actually stored as a double vector B[2*N].
//
//    The "complex" vector entry A[I] is stored as:
//
//      B[I*2+0], the real part,
//      B[I*2+1], the imaginary part.
//
//  Modified:
//
//    23 March 2009
//
//  Author:
//
//    Original C version by Wesley Petersen.
//    C++ version by John Burkardt.
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
//    Input, int N, the length of the "complex" array.
//
//    Input, double X[2*N], the array to be copied.
//
//    Output, double Y[2*N], a copy of X.
//
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    y[i*2+0] = x[i*2+0];
    y[i*2+1] = x[i*2+1];
   }
  return;
}
