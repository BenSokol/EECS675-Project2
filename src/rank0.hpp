/**
* @Filename: rank0.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [3:49pm]
* @Modified: March 29th, 2019 [9:56pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef RANK0_HPP
#define RANK0_HPP

#include <memory>
#include <mpi.h>
#include <string>

#include "DataInit.hpp"

class rank0 {
public:
  rank0(std::string aFileNameStr, size_t aReportYear, char aCustomerType);
  ~rank0();
  void run();

private:
  const std::string mFileNameStr;
  const size_t mReportYear;
  const char mCustomerType;

  std::unique_ptr<DataInit> mData;
  MPI_Datatype MPI_SalesRecord;
};

#endif
