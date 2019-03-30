/**
* @Filename: ErrorReporter.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [1:42pm]
* @Modified: March 29th, 2019 [10:52pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iostream>

#include "SalesRecord.hpp"

#include "ErrorReporter.hpp"

// Must be in the same order as ErrorReporter::CODES
std::vector<std::string> ErrorReporter::ErrorMessages{ "model number is less than 0.",
                                                       "model number is greater than or equal to numModels.",
                                                       "date does not consist of exactly 6 digits.",
                                                       "date includes an invalid month. Valid months are 01 - 12.",
                                                       "date includes an invalid year. Valid years are 1997-2018",
                                                       "invalid customer type.",
                                                       "amount of sale is less than or equal to zero.",
                                                       "invalid type. Expected int.",
                                                       "invalid type. Expected char.",
                                                       "unknown error." };

ErrorReporter::ErrorReporter() {
  defineStructSalesRecordToMPI(&MPI_SalesRecord);
}

ErrorReporter::~ErrorReporter() {}

void ErrorReporter::run() {
  bool isDone = false;
  while (!isDone) {
    SalesRecord record;
    MPI_Status status;
    MPI_Recv(&record, 1, MPI_SalesRecord, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    switch (status.MPI_TAG) {
      case CODES::END_REPORTER:
        isDone = true;
        break;
      case CODES::COUNT:
        [[fallthrough]];
      case CODES::VALID:
        break;
      default:
        std::cout << "Rank " << status.MPI_SOURCE << " "
                  << "reported \"" << ErrorMessages[status.MPI_TAG] << "\": {"
                  << "modelNumber: " << record.mWidgetModelNumber << ", "
                  << "date: " << record.mDateOfPurchase << ", "
                  << "customerType: " << record.mCustomerType << ", "
                  << "amount: " << record.mPurchaseAmountInDollars << "}\n"
                  << std::flush;
        break;
    }
  }

  // Wait for all tasks to synchronize
  MPI_Barrier(MPI_COMM_WORLD);
}
