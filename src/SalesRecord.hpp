/**
* @Filename: SalesRecord.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [1:32pm]
* @Modified: March 29th, 2019 [10:45pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef SALESRECORD_HPP
#define SALESRECORD_HPP

#include <cstdint>
#include <mpi.h>

typedef struct SalesRecord {
public:
  SalesRecord();
  SalesRecord(intmax_t aWidgetModelNumber, intmax_t aDateOfPurchase, char aCustomerType,
              double aPurchaseAmountInDollars);
  ~SalesRecord();

  intmax_t mWidgetModelNumber;
  intmax_t mDateOfPurchase;
  char mCustomerType;
  double mPurchaseAmountInDollars;
} SalesRecord;

void defineStructSalesRecordToMPI(MPI_Datatype* typeToBeCreated);

#endif
