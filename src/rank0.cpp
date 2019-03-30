/**
* @Filename: rank0.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [3:50pm]
* @Modified: March 29th, 2019 [10:53pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>
#include <iostream>
#include <memory>
#include <mpi.h>

#include "CheckSalesRecord.hpp"
#include "DataInit.hpp"
#include "ErrorReporter.hpp"
#include "ReportGenerator.hpp"
#include "SalesRecord.hpp"
#include "UTL_assert.h"
#include "UTL_colors.hpp"

#include "rank0.hpp"

rank0::rank0(std::string aFileNameStr, size_t aReportYear, char aCustomerType)
    : mFileNameStr(aFileNameStr), mReportYear(aReportYear), mCustomerType(aCustomerType) {
  mData = std::unique_ptr<DataInit>(new DataInit(mFileNameStr));
  defineStructSalesRecordToMPI(&MPI_SalesRecord);
}

rank0::~rank0() {}

void rank0::run() {
  // Check if command line args are valid, exit if they are not.
  if (!mData->isValidArgs()) {
    MPI_Abort(MPI_COMM_WORLD, MPI_ERR_ARG);
    return;
  }

  std::vector<SalesRecord> salesRecords = mData->getSalesRecordArray();

#ifndef NDEBUG
  std::cout << "File Info:\n";
  std::cout << "  isValidArgs=" << (mData->isValidArgs() ? "true" : "false") << "\n";
  std::cout << "  getNumModels=" << mData->getNumModels() << "\n";
  std::cout << "  getNumRecords=" << mData->getNumRecords() << "\n";
  std::cout << "  getReportYear=" << mReportYear << "\n";
  std::cout << "  getCustomerType=" << mCustomerType << "\n\n";
#endif

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

  size_t fileInfo[2] = { mData->getNumRecords(), mData->getNumModels() };
  MPI_Bcast(fileInfo, 2, MPI_UNSIGNED_LONG, 0, subComm);

  size_t recordsPerRankN = mData->getNumRecords() / numRanksN;
  size_t extraRecords = mData->getNumRecords() % numRanksN;
  std::vector<int> sendCounts(numRanksN, 0);
  std::vector<int> displs(numRanksN, 0);

  for (size_t i = 0; i < numRanksN; i++) {
    sendCounts[i] = recordsPerRankN;
    if (i < extraRecords) {
      sendCounts[i]++;
    }
    if (i < (numRanksN - 1)) {
      displs[i + 1] = displs[i] + sendCounts[i];
    }
  }

  if (extraRecords > 0) {
    recordsPerRankN++;
  }

  std::vector<SalesRecord> salesRecordSlice(recordsPerRankN);

  // Send data to other ranks
  MPI_Scatterv(salesRecords.data(), sendCounts.data(), displs.data(), MPI_SalesRecord, salesRecordSlice.data(),
               recordsPerRankN, MPI_SalesRecord, 0, subComm);

  std::vector<double> totalSales(mData->getNumModels(), 0);
  std::vector<double> yearSales(mData->getNumModels(), 0);
  std::vector<double> custSales(mData->getNumModels(), 0);

  // Process 1/(#ranks - 1) of data.
  for (size_t i = 0; i < recordsPerRankN; i++) {
    checkSalesRecord(salesRecordSlice[i], MPI_SalesRecord, mData->getNumModels(), mReportYear, mCustomerType,
                     totalSales, yearSales, custSales);
  }

  std::vector<double> totalSalesResults(mData->getNumModels(), 0);
  std::vector<double> yearSalesResults(mData->getNumModels(), 0);
  std::vector<double> custSalesResults(mData->getNumModels(), 0);

  // Wait for all subComm to synchronize...
  MPI_Barrier(subComm);

  // Combine results from other ranks...
  MPI_Reduce(totalSales.data(), totalSalesResults.data(), mData->getNumModels(), MPI_DOUBLE, MPI_SUM, 0, subComm);
  MPI_Reduce(yearSales.data(), yearSalesResults.data(), mData->getNumModels(), MPI_DOUBLE, MPI_SUM, 0, subComm);
  MPI_Reduce(custSales.data(), custSalesResults.data(), mData->getNumModels(), MPI_DOUBLE, MPI_SUM, 0, subComm);

  // Wait for all subComm to synchronize...
  MPI_Barrier(subComm);

  // Send message to ErrorReporter to end
  MPI_Send(nullptr, 0, MPI_SalesRecord, 1, ErrorReporter::CODES::END_REPORTER, MPI_COMM_WORLD);

  // Wait for all tasks to synchronize
  MPI_Barrier(MPI_COMM_WORLD);

  // Generate report
  ReportGenerator gen(mReportYear, mCustomerType);
  gen.generate(mData->getNumModels(), totalSalesResults, yearSalesResults, custSalesResults);
}
