/**
* @Filename: ErrorReporter.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [1:42pm]
* @Modified: March 29th, 2019 [9:19pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef ERRORREPORTER_HPP
#define ERRORREPORTER_HPP

#include <mpi.h>
#include <string>
#include <vector>

class ErrorReporter {
public:
  enum CODES : int {
    MODEL_NUMBER_MIN = 0,
    MODEL_NUMBER_MAX = 1,
    DATE_NUM_DIGITS = 2,
    DATE_INVALID_MONTH = 3,
    DATE_INVALID_YEAR = 4,
    INVALID_CUSTOMER_TYPE = 5,
    INVALID_SALE_VALUE = 6,
    TYPE_INT = 7,
    TYPE_CHAR = 8,
    UNKNOWN = 9,
    VALID = 10,
    END_REPORTER = 100,
    COUNT
  } CODE;

  ErrorReporter();
  ~ErrorReporter();

  void run();

private:
  MPI_Datatype MPI_SalesRecord;
  static std::vector<std::string> ErrorMessages;
};


#endif
