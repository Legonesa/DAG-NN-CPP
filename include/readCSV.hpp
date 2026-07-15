#ifndef READCSV_HPP
#define READCSV_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>

class DataSet{
public:
    DataSet(const char *fileName);
    void readCSV();
    void readCSVinRange(int begin, int end);
    void readCSVRow(int row);
    void readCSVCategory(std::string category);
    void readCSVCategoryinRange(std::string category, int begin, int end);
    void readCSVCategoryinRow(std::string category, int row);
    long double getValueNum(std::string category, int row);
    std::string getValueStr(std::string category, int row);
    std::string getCategoryName(int CategoryIndex);
    void freeMemory();
private:
    const char* file;
    std::map<std::string, std::map<int, std::string>> values;
};

#endif