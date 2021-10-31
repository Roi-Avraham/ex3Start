cmake_minimum_required(VERSION 3.17)
project(ex3Start)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex3Start MainTrain.cpp anomaly_detection_util.h
        AnomalyDetector.h SimpleAnomalyDetector.h timeseries.h
        SimpleAnomalyDetector.cpp anomaly_detection_util.cpp timeseries.cpp minCircle.cpp minCircle.h)