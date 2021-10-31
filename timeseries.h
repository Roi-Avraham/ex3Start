/*
 * timeseries.h
 *
 * Author: Shira Goren 207814989 with Roi Avraham 318778081
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
using namespace std;

/**
 * creates a data table
 * */
class TimeSeries {

public:
    const char *CSVFileName;

    /**
     * constructor.
     * */
    TimeSeries(const char *CSVFileName);

    /**
     * collect all features.
     * @return vector of all features.
     * */
    vector<string> countColumns() const;

    /**
     * count lines in file.
     * @return num of rows
     * */
    int numOfRows() const;

    /**
     * return data by index of column.
     * @return array of column collected from each row
     * */
    vector<vector<float>> dataOfFeatures() const;

    /**
    * return data by index of column.
    * @param index column we want from each row in file
    * @return array of column collected from each row
    * */
    float* dataByColumn(int index) const;
};


#endif /* TIMESERIES_H_ */
