/**
* @Filename: ReportGenerator.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [11:24am]
* @Modified: March 29th, 2019 [10:45pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef REPORTGENERATOR_HPP
#define REPORTGENERATOR_HPP

#include <string>
#include <vector>

class ReportGenerator {
public:
  ReportGenerator(size_t aYear, char aCustomerType);
  ~ReportGenerator();

  void generate(size_t size, std::vector<double> totalSales, std::vector<double> yearSales,
                std::vector<double> custSales);

private:
  const size_t mYear;
  const char mCustomerType;
};

#endif
