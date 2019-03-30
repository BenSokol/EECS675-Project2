/**
* @Filename: DataInit.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [2:29pm]
* @Modified: March 29th, 2019 [10:54pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef DATAINIT_HPP
#define DATAINIT_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include "SalesRecord.hpp"

class DataInit {
public:
  DataInit(std::string aFileName);
  ~DataInit();

  bool isValidArgs() const;
  std::vector<SalesRecord> getSalesRecordArray();
  size_t getNumModels() const;
  size_t getNumRecords() const;

  void printSalesArr() const;

private:
  bool mIsValid;
  std::string mFileName;
  std::vector<SalesRecord> mSalesRecordArr;
  size_t mNumModels;
  size_t mNumRecords;
};

#endif
