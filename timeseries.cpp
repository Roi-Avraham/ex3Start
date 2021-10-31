#include "timeseries.h"

/**
 * Constructor.
 * */
TimeSeries::TimeSeries(const char *CSVFileName) {
    this->CSVFileName = CSVFileName;
}

/**
 * collect all features.
 * @return vector of all features.
 * */
vector<string> TimeSeries::countColumns() const {
    std::ifstream file(CSVFileName);
    std::string line;

    /*create vector of features*/
    vector<string> namesOfFeatures (0);

    /*runs through first line in file*/
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string result;

        /*adds all features to vector*/
        while (std::getline(iss, result, ',')) {
            namesOfFeatures.push_back(result);
        }
        return namesOfFeatures;
    }
}

/**
* count lines in file.
* @return num of rows
* */
int TimeSeries::numOfRows() const{
    std::ifstream file(CSVFileName);
    std::string line;
    int row = 0;

    /*run through all lines in file*/
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        row++;
    }
    return row;
}

/**
  * return data by index of column.
  * @return all data by rows
  * */
vector<vector<float>> TimeSeries::dataOfFeatures() const{
    /*open file*/
    std::ifstream file(CSVFileName);
    std::string line;
    /*create flag for first row in file*/
    int firstRow = 0;
    /*initialize table of all data*/
    vector<vector<float>> allData (0);
    /*run through lines and save data from wanted column*/
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string result;
        /**/
        vector<float> rows (0);
        /*run through and save all rows from file*/
        while (std::getline(iss, result, ',')) {
            /*check row is not first line*/
            if(firstRow != 0) {
                float num_float = std::stof(result);
                rows.push_back(num_float);//save row
            }

        }
        firstRow++;
        allData.push_back(rows);//add row to all data
    }
    return allData;
}

/**
* return data by index of column.
* @param index column we want from each row in file
* @return array of column collected from each row
* */
float* TimeSeries::dataByColumn(int index) const {
    int size = numOfRows();
    auto* data = new float[size];
    vector<vector<float>> allData = dataOfFeatures();
    for (int k = 1; k < allData.size();k++) {
        data[k-1] = allData[k][index];
    }
    return data;
}