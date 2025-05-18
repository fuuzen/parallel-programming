//****************************************************************************80

void step ( int n, int mj, double a[], double b[], double c[], double w[], double sgn, int np, int rank )

//****************************************************************************80
//
//  Purpose:
//
//    STEP carries out one step of the workspace version of CFFT2.
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
{
  int j;
  int jmj2;
  int k;
  int lj;
  int mj2;
  double ambr;
  double ambu;
  double wjw[2];

  mj2 = 2 * mj;

  if ( mj2 <= np )  // scene 1
  {
    lj = np / mj2;
    
    for ( j = 0; j < lj; j++ )
    {
      jmj2 = j * mj2;

      for ( k = 0; k < mj; k++ )
      {
        wjw[0] = w[(n/mj2)*k*2+0];
        wjw[1] = w[(n/mj2)*k*2+1];

        if ( sgn < 0.0 ) 
        {
          wjw[1] = - wjw[1];
        }

        ambr = wjw[0] * a[(jmj2+mj+k)*2+0] - wjw[1] * a[(jmj2+mj+k)*2+1];
        ambu = wjw[1] * a[(jmj2+mj+k)*2+0] + wjw[0] * a[(jmj2+mj+k)*2+1];

        c[(jmj2   +k)*2+0] = a[(jmj2+k)*2+0] + ambr;
        c[(jmj2   +k)*2+1] = a[(jmj2+k)*2+1] + ambu;
        c[(jmj2+mj+k)*2+0] = a[(jmj2+k)*2+0] - ambr;
        c[(jmj2+mj+k)*2+1] = a[(jmj2+k)*2+1] - ambu;
      }
    }
  }
  else if ( (rank * np) % mj2 < mj )  // scene 2
  {
    for ( k = 0; k < np; k++ )
    {
      wjw[0] = w[(n / mj2) * ((rank * np) % mj + k)*2+0];
      wjw[1] = w[(n / mj2) * ((rank * np) % mj + k)*2+1];

      if ( sgn < 0.0 ) 
      {
        wjw[1] = - wjw[1];
      }
      
      ambr = wjw[0] * b[k*2+0] - wjw[1] * b[k*2+1];
      ambu = wjw[1] * b[k*2+0] + wjw[0] * b[k*2+1];
      
      c[k*2+0] = a[k*2+0] + ambr;
      c[k*2+1] = a[k*2+1] + ambu;
    }
  }
  else  // scene 3
  {
    for ( k = 0; k < np; k++ )
    {
      wjw[0] = w[(n / mj2) * ((rank * np) % mj + k)*2+0];
      wjw[1] = w[(n / mj2) * ((rank * np) % mj + k)*2+1];

      if ( sgn < 0.0 ) 
      {
        wjw[1] = - wjw[1];
      }

      ambr = wjw[0] * b[k*2+0] - wjw[1] * b[k*2+1];
      ambu = wjw[1] * b[k*2+0] + wjw[0] * b[k*2+1];

      c[k*2+0] = a[k*2+0] - ambr;
      c[k*2+1] = a[k*2+1] - ambu;
    }
  }
  
  return;
}
