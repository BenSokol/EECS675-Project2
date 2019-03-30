/**
* @Filename: SalesRecord.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [1:32pm]
* @Modified: March 29th, 2019 [10:56pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/


#include <mpi.h>
#include <string>

#include "SalesRecord.hpp"

SalesRecord::SalesRecord() {
  mWidgetModelNumber = 0;
  mDateOfPurchase = 0;
  mCustomerType = '\0';
  mPurchaseAmountInDollars = 0.0;
}

SalesRecord::SalesRecord(long int aWidgetModelNumber, long int aDateOfPurchase, char aCustomerType,
                         double aPurchaseAmountInDollars) {
  mWidgetModelNumber = aWidgetModelNumber;
  mDateOfPurchase = aDateOfPurchase;
  mCustomerType = aCustomerType;
  mPurchaseAmountInDollars = aPurchaseAmountInDollars;
}

SalesRecord::~SalesRecord() {}


void defineStructSalesRecordToMPI(MPI_Datatype* typeToBeCreated) {
  int blklen[4] = { 1, 1, 1, 1 };
  MPI_Aint displ[4] = { 0, 0, 0, 0 };
  MPI_Datatype types[4] = { MPI_LONG_LONG, MPI_LONG_LONG, MPI_CHAR, MPI_DOUBLE };

  SalesRecord sample;
  MPI_Aint base;
  MPI_Get_address(&sample.mWidgetModelNumber, &base);
  MPI_Aint oneField;
  MPI_Get_address(&sample.mDateOfPurchase, &oneField);
  displ[1] = oneField - base;
  MPI_Get_address(&sample.mCustomerType, &oneField);
  displ[2] = oneField - base;
  MPI_Get_address(&sample.mPurchaseAmountInDollars, &oneField);
  displ[3] = oneField - base;

  MPI_Type_create_struct(4, blklen, displ, types, typeToBeCreated);
  MPI_Type_commit(typeToBeCreated);
}
