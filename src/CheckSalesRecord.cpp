/**
* @Filename: CheckSalesRecord.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [9:07pm]
* @Modified: March 29th, 2019 [10:49pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <mpi.h>
#include <string>
#include <vector>

#include "ErrorReporter.hpp"
#include "SalesRecord.hpp"

#include "CheckSalesRecord.hpp"

void checkSalesRecord(const SalesRecord& record, MPI_Datatype& MPI_SalesRecord, const size_t& numModels,
                      const size_t& reportYear, const char& customerType, std::vector<double>& totalSales,
                      std::vector<double>& yearSales, std::vector<double>& custSales) {
  static const uint8_t ErrorReporterRank = 1;
  std::string dateStr = std::to_string(record.mDateOfPurchase);

  if (record.mWidgetModelNumber < 0) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::MODEL_NUMBER_MIN, MPI_COMM_WORLD);
    return;
  }
  else if (record.mWidgetModelNumber > static_cast<intmax_t>(numModels)) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::MODEL_NUMBER_MAX, MPI_COMM_WORLD);
    return;
  }
  else if (dateStr.length() != 6) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::DATE_NUM_DIGITS, MPI_COMM_WORLD);
    return;
  }

  size_t month = std::stoul(dateStr.substr(4, 2));
  size_t year = std::stoul(dateStr.substr(0, 4));
  if (month == 0 || month > 12) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::DATE_INVALID_MONTH, MPI_COMM_WORLD);
    return;
  }
  else if (year < 1997 || year > 2018) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::DATE_INVALID_YEAR, MPI_COMM_WORLD);
    return;
  }
  else if (!(record.mCustomerType == 'I' || record.mCustomerType == 'R' || record.mCustomerType == 'G')) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::INVALID_CUSTOMER_TYPE,
             MPI_COMM_WORLD);
    return;
  }
  else if (record.mPurchaseAmountInDollars <= 0) {
    MPI_Send(&record, 1, MPI_SalesRecord, ErrorReporterRank, ErrorReporter::CODES::INVALID_SALE_VALUE, MPI_COMM_WORLD);
    return;
  }

  // Sales Record must be valid.
  totalSales[record.mWidgetModelNumber] += record.mPurchaseAmountInDollars;
  if (year == reportYear) {
    yearSales[record.mWidgetModelNumber] += record.mPurchaseAmountInDollars;
  }
  if (record.mCustomerType == customerType) {
    custSales[record.mWidgetModelNumber] += record.mPurchaseAmountInDollars;
  }
}
