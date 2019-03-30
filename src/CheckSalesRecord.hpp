/**
* @Filename: CheckSalesRecord.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [9:01pm]
* @Modified: March 29th, 2019 [10:46pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef CHECKSALESRECORD_HPP
#define CHECKSALESRECORD_HPP

#include <cstdlib>
#include <mpi.h>
#include <vector>

#include "SalesRecord.hpp"

void checkSalesRecord(const SalesRecord& record, MPI_Datatype& MPI_SalesRecord, const size_t& numModels,
                      const size_t& mReportYear, const char& mCustomerType, std::vector<double>& totalSales,
                      std::vector<double>& yearSales, std::vector<double>& custSales);

#endif
