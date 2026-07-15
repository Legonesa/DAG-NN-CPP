#ifndef READCSV_HPP
#define READCSV_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>

class DataSet {
public:
    DataSet(const char* fileName); // Opens the file.
    void readCSV(); // Reads the file.
    void readCSVinRange(int begin, int end); // Reads a specific range of rows.
    void readCSVRow(int row); // Reads a single row.
    void readCSVCategory(std::string category); // Reads a single category (column).
    void readCSVCategoryinRange(std::string category, int begin, int end); // Reads a specific range of rows within a category.
    void readCSVCategoryinRow(std::string category, int row); // Reads a single row within a category.
    long double getValueNum(std::string category, int row); // Returns a parsed value. (Numeric value.)
    std::string getValueStr(std::string category, int row); // Returns a parsed value. (String value.)
    std::string getCategoryName(int CategoryIndex); // Returns the name of the category at the specified index.
    void freeMemory(); // Clears the memory.
private:
    const char* file;
    std::map<std::string, std::map<int, std::string>> values;
};

#endif
