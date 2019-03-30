/**
* @Filename: rankN.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [3:49pm]
* @Modified: March 29th, 2019 [9:57pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef RANKN_HPP
#define RANKN_HPP

#include <mpi.h>

class rankN {
public:
  rankN(size_t aReportYear, char aCustomerType, size_t aRank);
  ~rankN();
  void run();

private:
  const size_t mReportYear;
  const char mCustomerType;
  const size_t mRank;

  MPI_Datatype MPI_SalesRecord;
};

#endif
