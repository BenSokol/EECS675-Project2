/**
* @Filename: ReportGenerator.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [11:24am]
* @Modified: March 29th, 2019 [11:28pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "UTL_colors.hpp"
#include "UTL_numberFormat.hpp"

#include "ReportGenerator.hpp"

ReportGenerator::ReportGenerator(size_t aYear, char aCustomerType) : mYear(aYear), mCustomerType(aCustomerType) {}

ReportGenerator::~ReportGenerator() {}

void ReportGenerator::generate(size_t size, std::vector<double> totalSales, std::vector<double> yearSales,
                               std::vector<double> custSales) {
  std::string col1 = "Model Number";
  std::string col2 = "Total Amounts";
  std::string col3 = "Amounts for " + std::to_string(mYear);
  std::string col4 = "Amounts for customer Type ";
  col4 += mCustomerType;

  std::cout << "\n" << std::string((col1.size() + col2.size() + col3.size() + col4.size() + 13), '-');
  std::cout << "\n";
  std::cout << "| " << UTL::COLOR::CODES::bold << col1 << UTL::COLOR::CODES::reset << " | " << UTL::COLOR::CODES::bold
            << col2 << UTL::COLOR::CODES::reset << " | " << UTL::COLOR::CODES::bold << col3 << UTL::COLOR::CODES::reset
            << " | " << UTL::COLOR::CODES::bold << col4 << UTL::COLOR::CODES::reset << " |\n";
  std::cout << std::string((col1.size() + col2.size() + col3.size() + col4.size() + 13), '-');
  std::cout << "\n";
  std::cout.precision(2);
  for (size_t i = 0; i < size; ++i) {
    std::cout << "| ";
    std::cout << std::setw(col1.size()) << std::setprecision(2) << std::fixed << i;
    std::cout << " | ";
    std::string totalSalesStr = UTL::formatAsCurrency(totalSales[i]);
    std::cout << std::setw(col2.size()) << std::fixed << totalSalesStr;
    std::cout << " | ";
    std::string yearSalesStr = UTL::formatAsCurrency(yearSales[i]);
    std::cout << std::setw(col3.size()) << std::fixed << yearSalesStr;
    std::cout << " | ";
    std::string custSalesStr = UTL::formatAsCurrency(custSales[i]);
    std::cout << std::setw(col4.size()) << std::fixed << custSalesStr;
    std::cout << " |\n";
  }
  std::cout << std::string((col1.size() + col2.size() + col3.size() + col4.size() + 13), '-');
  std::cout << "\n\n";
}
