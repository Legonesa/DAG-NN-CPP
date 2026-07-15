#include "readCSV.hpp"

// Opens the file.
DataSet::DataSet(const char* fileName) {
    file = fileName;
    std::ifstream inputFile;
    inputFile.open(file);
    std::string fullLine;
    getline(inputFile, fullLine);
    std::string flood = "";
    for (int i = 0; i < (int)(fullLine.length()); i++) {
        if (fullLine[i] != ',') {
            flood = flood + fullLine[i];
        }
        else {
            std::cout << "Category: " << flood << std::endl;
            values[flood];
            flood = "";
        }
    }
    std::cout << "Category: " << flood << std::endl;
    values[flood];
    flood = "";
    inputFile.close();
    std::cout << values.size() << " category detected!" << std::endl;
}
// Reads the file.
void DataSet::readCSV() {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue = "";
    int categoryInd = 0;
    int rows = 1;
    std::vector<std::string> categories;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ',') tmpValue = tmpValue + tmp[i];
        else {
            categories.push_back(tmpValue);
            tmpValue = "";
        }
    }
    categories.push_back(tmpValue);
    tmpValue = "";
    while (getline(inputFile, tmp)) {
        bool isThere = false;
        bool doLastThing = true;
        for (int i = 0; i < tmp.length(); i++) {
            if (isThere == true) {
                if (tmp[i] != '"') tmpValue = tmpValue + tmp[i];
                else {
                    values[categories.at(categoryInd)][rows] = tmpValue;
                    categoryInd++;
                    tmpValue = "";
                    isThere = false;
                    doLastThing = false;
                    i++;
                }
            }
            else {
                if (tmp[i] != ',') {
                    if (tmp[i] == '"') { isThere = true; continue; }
                    tmpValue = tmpValue + tmp[i];
                    doLastThing = true;
                }
                else {
                    values[categories.at(categoryInd)][rows] = tmpValue;
                    categoryInd++;
                    doLastThing = false;
                    tmpValue = "";
                }
            }
        }
        if (doLastThing) values[categories.at(categoryInd)][rows] = tmpValue;
        categoryInd = 0;
        tmpValue = "";
        rows++;
    }
    inputFile.close();
}
// Reads a specific range of rows.
void DataSet::readCSVinRange(int begin, int end) {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue;
    int categoryInd = 0;
    int rows = 1;
    std::vector<std::string> categories;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ',') tmpValue = tmpValue + tmp[i];
        else {
            categories.push_back(tmpValue);
            tmpValue = "";
        }
    }
    categories.push_back(tmpValue);
    tmpValue = "";
    while (getline(inputFile, tmp)) {
        bool isThere = false;
        bool doLastThing = true;
        if (rows <= end && rows >= begin) {
            for (int i = 0; i < tmp.length(); i++) {
                if (isThere == true) {
                    if (tmp[i] != '"') tmpValue = tmpValue + tmp[i];
                    else {
                        values[categories.at(categoryInd)][rows] = tmpValue;
                        categoryInd++;
                        tmpValue = "";
                        isThere = false;
                        doLastThing = false;
                        i++;
                    }
                }
                else {
                    if (tmp[i] != ',') {
                        if (tmp[i] == '"') { isThere = true; continue; }
                        tmpValue = tmpValue + tmp[i];
                        doLastThing = true;
                    }
                    else {
                        values[categories.at(categoryInd)][rows] = tmpValue;
                        categoryInd++;
                        doLastThing = false;
                        tmpValue = "";
                    }
                }
            }
            if (doLastThing) values[categories.at(categoryInd)][rows] = tmpValue;
            categoryInd = 0;
            tmpValue = "";
        }
        rows++;
    }
    inputFile.close();
}
// Reads a single row.
void DataSet::readCSVRow(int row) {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue;
    int categoryInd = 0;
    int rows = 1;
    std::vector<std::string> categories;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ',') tmpValue = tmpValue + tmp[i];
        else {
            categories.push_back(tmpValue);
            tmpValue = "";
        }
    }
    categories.push_back(tmpValue);
    tmpValue = "";
    while (getline(inputFile, tmp)) {
        bool isThere = false;
        bool doLastThing = true;
        if (rows == row) {
            for (int i = 0; i < tmp.length(); i++) {
                if (isThere == true) {
                    if (tmp[i] != '"') tmpValue = tmpValue + tmp[i];
                    else {
                        values[categories.at(categoryInd)][rows] = tmpValue;
                        categoryInd++;
                        tmpValue = "";
                        isThere = false;
                        doLastThing = false;
                        i++;
                    }
                }
                else {
                    if (tmp[i] != ',') {
                        if (tmp[i] == '"') { isThere = true; continue; }
                        tmpValue = tmpValue + tmp[i];
                        doLastThing = true;
                    }
                    else {
                        values[categories.at(categoryInd)][rows] = tmpValue;
                        categoryInd++;
                        doLastThing = false;
                        tmpValue = "";
                    }
                }
            }
            if (doLastThing) values[categories.at(categoryInd)][rows] = tmpValue;
            categoryInd = 0;
            tmpValue = "";
            break;
        }
        rows++;
    }
    inputFile.close();
}
// Reads a single category (column).
void DataSet::readCSVCategory(std::string category) {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue = "";
    int categoryInd = 0;
    int rows = 1;
    std::vector<std::string> categories;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ',') tmpValue = tmpValue + tmp[i];
        else {
            categories.push_back(tmpValue);
            tmpValue = "";
        }
    }
    categories.push_back(tmpValue);
    tmpValue = "";
    while (getline(inputFile, tmp)) {
        bool isThere = false;
        bool doLastThing = true;
        for (int i = 0; i < tmp.length(); i++) {
            if (categories.at(categoryInd) == category) {
                if (isThere == true) {
                    if (tmp[i] != '"') {
                        tmpValue = tmpValue + tmp[i];
                    }
                    else {
                        values[categories.at(categoryInd)][rows] = tmpValue;
                        tmpValue = "";
                        isThere = false;
                        doLastThing = false;
                        break;
                    }
                }
                else {
                    if (tmp[i] != ',') {
                        if (tmp[i] == '"') { isThere = true; continue; }
                        tmpValue = tmpValue + tmp[i];
                        doLastThing = true;
                    }
                    else {
                        values[categories.at(categoryInd)][rows] = tmpValue;
                        doLastThing = false;
                        tmpValue = "";
                        break;
                    }
                }
            }
            if (tmp[i] == ',' && isThere == false) categoryInd++;
        }
        if (categories.at(categoryInd) == category && doLastThing == true) values[categories.at(categoryInd)][rows] = tmpValue;
        categoryInd = 0;
        tmpValue = "";
    }
    inputFile.close();
}
// Reads a specific range of rows within a category.
void DataSet::readCSVCategoryinRange(std::string category, int begin, int end) {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue;
    int categoryInd = 0;
    int rows = 1;
    std::vector<std::string> categories;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ',') tmpValue = tmpValue + tmp[i];
        else {
            categories.push_back(tmpValue);
            tmpValue = "";
        }
    }
    categories.push_back(tmpValue);
    tmpValue = "";
    while (getline(inputFile, tmp)) {
        bool isThere = false;
        bool doLastThing = true;
        if (rows <= end && rows >= begin) {
            for (int i = 0; i < tmp.length(); i++) {
                if (categories.at(categoryInd) == category) {
                    if (isThere == true) {
                        if (tmp[i] != '"') {
                            tmpValue = tmpValue + tmp[i];
                        }
                        else {
                            values[categories.at(categoryInd)][rows] = tmpValue;
                            tmpValue = "";
                            isThere = false;
                            doLastThing = false;
                            break;
                        }
                    }
                    else {
                        if (tmp[i] != ',') {
                            if (tmp[i] == '"') { isThere = true; continue; }
                            tmpValue = tmpValue + tmp[i];
                            doLastThing = true;
                        }
                        else {
                            values[categories.at(categoryInd)][rows] = tmpValue;
                            doLastThing = false;
                            tmpValue = "";
                            break;
                        }
                    }
                }
                if (tmp[i] == ',' && isThere == false) categoryInd++;
            }
            if (categories.at(categoryInd) == category && doLastThing == true) values[categories.at(categoryInd)][rows] = tmpValue;
            categoryInd = 0;
            tmpValue = "";
        }
        rows++;
    }
    inputFile.close();
}
// Reads a single row within a category.
void DataSet::readCSVCategoryinRow(std::string category, int row) {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue;
    int categoryInd = 0;
    int rows = 1;
    std::vector<std::string> categories;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] != ',') tmpValue = tmpValue + tmp[i];
        else {
            categories.push_back(tmpValue);
            tmpValue = "";
        }
    }
    categories.push_back(tmpValue);
    tmpValue = "";
    while (getline(inputFile, tmp)) {
        bool isThere = false;
        bool doLastThing = true;
        if (rows == row) {
            for (int i = 0; i < tmp.length(); i++) {
                if (categories.at(categoryInd) == category) {
                    if (isThere == true) {
                        if (tmp[i] != '"') {
                            tmpValue = tmpValue + tmp[i];
                        }
                        else {
                            values[categories.at(categoryInd)][rows] = tmpValue;
                            tmpValue = "";
                            isThere = false;
                            doLastThing = false;
                            break;
                        }
                    }
                    else {
                        if (tmp[i] != ',') {
                            if (tmp[i] == '"') { isThere = true; continue; }
                            tmpValue = tmpValue + tmp[i];
                            doLastThing = true;
                        }
                        else {
                            values[categories.at(categoryInd)][rows] = tmpValue;
                            doLastThing = false;
                            tmpValue = "";
                            break;
                        }
                    }
                }
                if (tmp[i] == ',' && isThere == false) categoryInd++;
            }
            if (categories.at(categoryInd) == category && doLastThing == true) values[categories.at(categoryInd)][rows] = tmpValue;
            categoryInd = 0;
            tmpValue = "";
            break;
        }
        rows++;
    }
    inputFile.close();
}
// Returns a parsed value. (Numeric value.)
long double DataSet::getValueNum(std::string category, int row) {
    try {
        return stold(values.at(category).at(row));
    }
    catch (const std::invalid_argument& e) {
        std::cout << std::endl << "It's a string, not a num => " << "Category: " << category << " Row: " << row << " Value: " << values[category][row] << std::endl;
        std::cout << e.what() << std::endl;
        return 0;
    }
    catch (const std::out_of_range& e) {
        std::cout << std::endl << "Value is out of the range => " << "Category: " << category << " Rows: " << row << " Value: " << values[category][row] << std::endl;
        std::cout << "Problem: " << e.what() << std::endl;
        return 0;
    }
}
// Returns a parsed value. (String value.)
std::string DataSet::getValueStr(std::string category, int row) {
    try {
        return values.at(category).at(row);
    }
    catch (const std::out_of_range& e) {
        std::cout << std::endl << "Value is out of the range => " << "Category: " << category << " Rows: " << row << " Value: " << values[category][row] << std::endl;
        std::cout << "Problem: " << e.what() << std::endl;
        return "RANGE ERROR!";
    }
}
// Returns the name of the category at the specified index.
std::string DataSet::getCategoryName(int CategoryIndex) {
    std::ifstream inputFile;
    inputFile.open(file);
    std::string tmp;
    std::string tmpValue = "";
    int CategoryInd = 1;
    getline(inputFile, tmp);
    for (int i = 0; i < tmp.length(); i++) {
        if (tmp[i] == ',') { CategoryInd++; continue; }
        if (CategoryInd == CategoryIndex) tmpValue = tmpValue + tmp[i];
    }
    inputFile.close();
    return tmpValue;
}
// Clears the memory.
void DataSet::freeMemory() {
    values.clear();
}
