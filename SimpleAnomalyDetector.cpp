/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Shira Goren 207814989 with Roi Avraham 318778081
 */

#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    minCorrelation = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

/**
 * creates and returns array of all points.
 * @param x array of x values
 * @param y array of y values
 * @param size size of arrays x and y
 * @return array af points
 * */
Point** arrayPoints(float* x, float* y, int size) {
    Point** points = new Point*[size];
    for (int i=0; i < size; i++) {
        Point* point = new Point(x[i], y[i]);
        points[i] = point;
    }
    return points;
}

/**
 * find max deviation.
 * @param size size of point array
 * @param arrayOfPoints point array
 * @param correlated a reference to a correlatedFeature object
 * @return max deviation
 * */
float findMaxDeviation(Point** arrayOfPoints, int size,
                       const correlatedFeatures& correlated) {
    /*create deviations array*/
    float maxDeviation = 0.0;
    /*run through points and check deviation to normal*/
    for (int k=0;k< size;k++) {
        Point point(arrayOfPoints[k]->x, arrayOfPoints[k]->y);//create point
        float d =  (dev(point, correlated.lin_reg));//find deviation
        /*save max deviation*/
        if (maxDeviation < d) {
            maxDeviation = d;
        }
    }
    return (maxDeviation*1.2);
}

/**
 * creates cf object.
 * */
correlatedFeatures createCFObject(string &featureI, string &featureC,
                                  float m, float* i, float* c, int size) {
    /*create correlated object*/
    correlatedFeatures correlated;
    correlated.feature1 = featureI;//namesOfFeatures[i];
    correlated.feature2 = featureC;//namesOfFeatures[c];
    correlated.corrlation = m;
    /*save all data of i, c categories*/
    float* x = i;//ts.dataByColumn(i);
    float* y = c;//ts.dataByColumn(c);
    /*create array of points*/
    Point** arrayOfPoints = arrayPoints(x, y, size);
    /*create normal from points*/
    correlated.lin_reg = linear_reg(arrayOfPoints, size);
    /*save max deviations in object*/
    correlated.threshold = (findMaxDeviation(arrayOfPoints, size, correlated));
    return correlated;
}

/**
 * find correlated couple in vector and return it.
 * */
correlatedFeatures SimpleAnomalyDetector::findCorrelated(string &featureI, string &featureII) {
    for (int i = 0; i < getNormalModel().size(); i++) {
        if (getNormalModel()[i].feature1 == (featureI) &&
             getNormalModel()[i].feature2 == (featureII)) {
            return getNormalModel()[i];
        }
    }
    correlatedFeatures empty;
    empty.feature1 = "Empty";
    return empty;
}


/**
 * create a correlated features objects.
 * @param ts reference to a timeSeries object
 * */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    /*vector of all categories*/
    vector<string> namesOfFeatures = ts.countColumns();
    int size = ts.numOfRows() - 1;
    /*run through all categories and find all correlatives*/
    for (int i = 0; i < namesOfFeatures.size(); i++) {
        float m = minCorrelation; //our correlation threshold
        int c = -1; //index of most correlative category

        /*run through all categories to come, and finds correlatives*/
        for (int j = i + 1;j < namesOfFeatures.size(); j++) {
            float* x = ts.dataByColumn(i);//all data of category i
            float* y = ts.dataByColumn(j);//all data of category j
            float p = pearson(x, y, size);//find pearson
            /*check if correlative*/
            if(p > m || (-1*p > m)) {
                /*save pearson as threshold and j as correlative index*/
                m = p;
                c = j;
            }
        }
        /*make sure a correlation was found*/
        if (c != -1) {
            /*create cf object*/
            correlatedFeatures correlated
                    = createCFObject(namesOfFeatures[i], namesOfFeatures[c],
                                     m, ts.dataByColumn(i), ts.dataByColumn(c),
                                     size);
            /*add correlated object to timeseries object if correlation is large enough*/
            cf.push_back(correlated);
        }
    }
}

/**
 * detects anomaly based on previous correlated features.
 * @param ts object of timeSeries
 * @return vector of all anomalies found
 * */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<vector<float>> data = ts.dataOfFeatures();//save all lines
    vector<AnomalyReport> allReports; //create report vector
    /*vector of all categories*/
    vector<string> namesOfFeatures = ts.countColumns();
    int size = ts.numOfRows();
    /*run through all lines and check points of correlatives*/
    for (int i = 1; i < size; i++) {
        /*run through all features*/
        for (int j = 0; j < namesOfFeatures.size(); j++) {
            /*run through rest of the features in the line*/
            for (int k = j + 1; k <namesOfFeatures.size(); k++) {
                correlatedFeatures found = findCorrelated(namesOfFeatures[j], namesOfFeatures[k]);
                /*make sure correlation was found*/
                if (found.feature1 != "Empty") {
                    Point point(data[i][j], data[i][k]);//create point
                    float d = dev(point, found.lin_reg);//find deviation
                    /*add anomaly to vector if deviation is too large*/
                    if (d > found.threshold) {
                        AnomalyReport newReport(found.feature1 + "-" + found.feature2, i);
                        allReports.push_back(newReport);
                    }
                }
            }

        }
    }
    return allReports;
}

