/**
* @Filename: main.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 27th, 2019 [11:12am]
* @Modified: March 29th, 2019 [10:53pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iostream>
#include <mpi.h>

#include "ErrorReporter.hpp"
#include "rank0.hpp"
#include "rankN.hpp"

#include "UTL_colors.hpp"
#include "UTL_inputValidation.hpp"

int main(int argc, char *argv[]) {
  int rank;
  int communicatorSize;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize);

  if (communicatorSize < 2) {
    if (rank == 0) {
      std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset
                << ": Requires a communicatorSize >= 2\nAborting...\n";
      MPI_Abort(MPI_COMM_WORLD, MPI_ERR_ARG);
    }
    return 1;
  }

  std::string fileName = std::string(argv[1]);
  size_t reportYear = 0;
  char customerType = '\0';

  std::string usage = "Usage: mpirun mpirun … reportGenerator <dataFileName> <reportYear> <customerType>";
  if (argc != 4) {
    if (rank == 0) {
      std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset
                << ": Invalid number of command line arguments.\n"
                << usage << "\n";
      MPI_Abort(MPI_COMM_WORLD, MPI_ERR_ARG);
    }
    return 1;
  }

  std::string reportYearStr = std::string(argv[2]);
  std::string customerTypeStr = std::string(argv[3]);

  if (reportYearStr.length() != 4 || !UTL::isNumber(reportYearStr)) {
    if (rank == 0) {
      std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset << ": Invalid year. Received: '"
                << reportYearStr << "'\n"
                << usage << "\n";
      MPI_Abort(MPI_COMM_WORLD, MPI_ERR_ARG);
    }
    return 1;
  }

  if ((customerTypeStr.length() != 1)
      || !(customerTypeStr[0] == 'G' || customerTypeStr[0] == 'I' || customerTypeStr[0] == 'R')) {
    if (rank == 0) {
      std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset << ": Invalid Customer Type. Received: '"
                << customerTypeStr << "'. Expected 'G', 'I', or 'R'.\n"
                << usage << "\n";
      MPI_Abort(MPI_COMM_WORLD, MPI_ERR_ARG);
    }
    return 1;
  }

  reportYear = std::stoull(reportYearStr);
  customerType = customerTypeStr[0];

  if (rank == 0) {
    rank0 r0(fileName, reportYear, customerType);
    r0.run();
  }
  else if (rank == 1) {
    ErrorReporter err;
    err.run();
  }
  else {
    rankN rn(reportYear, customerType, static_cast<size_t>(rank));
    rn.run();
  }

  MPI_Finalize();
  return 0;
}
