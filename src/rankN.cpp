/**
* @Filename: rankN.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [4:59pm]
* @Modified: March 29th, 2019 [10:45pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>
#include <iostream>
#include <memory>
#include <mpi.h>
#include <string>
#include <vector>

#include "CheckSalesRecord.hpp"
#include "ErrorReporter.hpp"
#include "SalesRecord.hpp"

#include "rankN.hpp"

rankN::rankN(size_t aReportYear, char aCustomerType, size_t aRank)
    : mReportYear(aReportYear), mCustomerType(aCustomerType), mRank(aRank) {
  defineStructSalesRecordToMPI(&MPI_SalesRecord);
}

rankN::~rankN() {}

void rankN::run() {
  MPI_Comm subComm;
  MPI_Group worldGroup;
  MPI_Group subGroup;
  int numRanksN_int;
  size_t numRanksN;

  const int excl[1] = { 1 };
  MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
  MPI_Group_excl(worldGroup, 1, excl, &subGroup);
  MPI_Comm_create_group(MPI_COMM_WORLD, subGroup, 0, &subComm);
  MPI_Comm_size(subComm, &numRanksN_int);
  numRanksN = static_cast<size_t>(numRanksN_int);

  size_t fileInfo[2];
  MPI_Bcast(fileInfo, 2, MPI_UNSIGNED_LONG, 0, subComm);

  size_t numRecords = fileInfo[0];
  size_t numModels = fileInfo[1];

  size_t recordsPerRankN = numRecords / numRanksN;
  size_t extraRecords = numRecords % numRanksN;

  if ((mRank - 1) < extraRecords) {
    recordsPerRankN++;
  }

  std::vector<SalesRecord> salesRecordSlice(recordsPerRankN);
  MPI_Scatterv(nullptr, nullptr, nullptr, MPI_SalesRecord, salesRecordSlice.data(), recordsPerRankN, MPI_SalesRecord, 0,
               subComm);

  std::vector<double> totalSales(numModels, 0);
  std::vector<double> yearSales(numModels, 0);
  std::vector<double> custSales(numModels, 0);

  // Process 1/(#ranks - 1) of data.
  for (size_t i = 0; i < recordsPerRankN; i++) {
    checkSalesRecord(salesRecordSlice[i], MPI_SalesRecord, numModels, mReportYear, mCustomerType, totalSales, yearSales,
                     custSales);
  }

  // Wait for all subComm to synchronize...
  MPI_Barrier(subComm);

  // Combine results from other ranks...
  MPI_Reduce(totalSales.data(), nullptr, numModels, MPI_DOUBLE, MPI_SUM, 0, subComm);
  MPI_Reduce(yearSales.data(), nullptr, numModels, MPI_DOUBLE, MPI_SUM, 0, subComm);
  MPI_Reduce(custSales.data(), nullptr, numModels, MPI_DOUBLE, MPI_SUM, 0, subComm);

  // Wait for all subComm to synchronize...
  MPI_Barrier(subComm);

  // Wait for all tasks to synchronize
  MPI_Barrier(MPI_COMM_WORLD);
}
