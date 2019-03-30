/**
* @Filename: DataInit.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 8th, 2019 [2:29pm]
* @Modified: March 29th, 2019 [11:13pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SalesRecord.hpp"
#include "UTL_colors.hpp"
#include "UTL_inputValidation.hpp"

#include "DataInit.hpp"

DataInit::DataInit(std::string aFileName) : mIsValid(true), mFileName(aFileName) {
  std::string usage = "Usage: mpirun mpirun … reportGenerator <dataFileName> <reportYear> <customerType>";

  std::ifstream fs;
  fs.open(mFileName);
  if (!fs.is_open()) {
    std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset << ": Unable to open file '" << mFileName
              << "'.\n"
              << usage << "\n";
    mIsValid = false;
    return;
  }
  std::string tempNumRecords = "";
  std::string tempNumModels = "";

  fs >> tempNumRecords;
  fs >> tempNumModels;

  // validate number of records is a number and is >= 0;
  if (!UTL::isNumber(tempNumRecords, 0)) {
    std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset
              << ": Invalid number of records. Received: '" << tempNumRecords << "'\n"
              << usage << "\n";
    mIsValid = false;
    return;
  }

  // validate number of models is a number and is >= 0;
  if (!UTL::isNumber(tempNumModels)) {
    std::cout << UTL::COLOR::FG::BRIGHT::red << "ERROR" << UTL::COLOR::reset
              << ": Invalid number of models. Received: '" << tempNumModels << "'\n"
              << usage << "\n";
    mIsValid = false;
    return;
  }

  mNumRecords = std::stoull(tempNumRecords);
  mNumModels = std::stoull(tempNumModels);
  mSalesRecordArr = std::vector<SalesRecord>(mNumRecords);

  // Read SalesRecords in from file. This loop will validate every step along the way to ideally prevent any program crashes from a bad file.
  // That is achomplished by reading in all values into strings, then converting those strings,
  // if they are of the correct type, to the correct type and saving in a SalesRecord.
  // This will >>NOT<< check any of the error cases for the sake of this project, only general stuff.
  for (size_t i = 0; i < mNumRecords; ++i) {
    std::string widgetModelNumberStr = "";
    std::string dateOfPurchaseStr = "";
    std::string customerTypeStr = "";
    std::string purchaseAmountInDollarsStr = "";

    if (fs.eof()) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset << ": EOF reached before "
                << mNumRecords << " have been read in from file.";
      break;
    }
    fs >> widgetModelNumberStr;
    if (fs.eof()) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset << ": EOF reached before "
                << mNumRecords << " have been read in from file.";
      break;
    }
    fs >> dateOfPurchaseStr;
    if (fs.eof()) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset << ": EOF reached before "
                << mNumRecords << " have been read in from file.";
      break;
    }
    fs >> customerTypeStr;
    if (fs.eof()) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset << ": EOF reached before "
                << mNumRecords << " have been read in from file.";
      break;
    }
    fs >> purchaseAmountInDollarsStr;

    if (!UTL::isNumber(widgetModelNumberStr)) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset
                << ": Invalid widget model encountered while reading in from file. Received: '" << widgetModelNumberStr
                << "'\n         Skipping entry {" << widgetModelNumberStr << " " << dateOfPurchaseStr << " "
                << customerTypeStr << " " << purchaseAmountInDollarsStr
                << "}\n         NOTE: This check only validates that the variable is a number, it does NOT check what "
                   "that value is.\n";
      continue;
    }
    if (!UTL::isNumber(dateOfPurchaseStr)) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset
                << ": Invalid data of purchase encountered while reading in from file. Received: '" << dateOfPurchaseStr
                << "'\n         Skipping entry {" << widgetModelNumberStr << " " << dateOfPurchaseStr << " "
                << customerTypeStr << " " << purchaseAmountInDollarsStr
                << "}\n         NOTE: This check only validates that the variable is a number, it does NOT check what "
                   "that value is.\n";
      continue;
    }
    if (customerTypeStr.length() != 1) {
      std::cout
          << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset
          << ": Invalid length of customer type. Received: '" << customerTypeStr << "'\n         Skipping entry {"
          << widgetModelNumberStr << " " << dateOfPurchaseStr << " " << customerTypeStr << " "
          << purchaseAmountInDollarsStr
          << "}\n         NOTE: This check only validates that the variable is a single char, it does NOT check what "
             "that char is.\n";
      continue;
    }
    if (!UTL::isNumber(purchaseAmountInDollarsStr)) {
      std::cout << UTL::COLOR::FG::BRIGHT::magenta << "WARNING" << UTL::COLOR::reset
                << ": Invalid purchase amount encountered while reading in from file. Received: '"
                << purchaseAmountInDollarsStr << "'\n         Skipping entry {" << widgetModelNumberStr << " "
                << dateOfPurchaseStr << " " << customerTypeStr << " " << purchaseAmountInDollarsStr
                << "}\n         NOTE: This check only validates that the variable is a number, it does NOT check what "
                   "that value is.\n";
      continue;
    }

    intmax_t widgetModelNumber = std::stoll(widgetModelNumberStr);
    intmax_t dateOfPurchase = std::stoll(dateOfPurchaseStr);
    char customerType = customerTypeStr[0];
    double purchaseAmountInDollars = std::stod(purchaseAmountInDollarsStr);

    mSalesRecordArr[i] = SalesRecord(widgetModelNumber, dateOfPurchase, customerType, purchaseAmountInDollars);
  }
}

DataInit::~DataInit() {}

bool DataInit::isValidArgs() const {
  return mIsValid;
}

std::vector<SalesRecord> DataInit::getSalesRecordArray() {
  return mSalesRecordArr;
}

size_t DataInit::getNumModels() const {
  return mNumModels;
}

size_t DataInit::getNumRecords() const {
  return mNumRecords;
}

void DataInit::printSalesArr() const {
  for (size_t i = 0; i < mNumRecords; ++i) {
    std::cout << mSalesRecordArr[i].mWidgetModelNumber << ", " << mSalesRecordArr[i].mDateOfPurchase << ", "
              << mSalesRecordArr[i].mCustomerType << ", " << mSalesRecordArr[i].mPurchaseAmountInDollars << "\n";
  }
}
