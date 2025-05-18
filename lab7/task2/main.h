# include <iostream>
# include <math.h>
# include "lib/timer.h"
# include "lib/parallel_for.h"

int main ( int argc, char *argv[] );

class Arg: public ArgInterface {
public:
  int M;
  int N;
  double * w;
  double * u;

  void asgn_w(const int i, const int j, double value) {
    w[i * N + j] = value;
  }

  void asgn_u(const int i, const int j, double value) {
    u[i * N + j] = value;
  }

  double w_at(const int i, const int j) {
    return w[i * N + j];
  }

  double u_at(const int i, const int j) {
    return u[i * N + j];
  }
};


class ArgWithMean: public Arg {
public:
  double * mean;
  double sub_mean = 0.0;

  void converge() override {
    *mean += sub_mean;
  }

  ArgWithMean* clone() override {
    ArgWithMean * cloned = new ArgWithMean(*this);
    return cloned;
  }
};


class ArgWithDiff: public Arg {
public:
  double * diff;
  double sub_diff = 0.0;

  void converge() override {
    if ( *diff < sub_diff ) {
      *diff = sub_diff;
    }
  }

  ArgWithDiff* clone() override {
    ArgWithDiff * cloned = new ArgWithDiff(*this);
    return cloned;
  }
};