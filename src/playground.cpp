/*
  The MIT License (MIT)

  Copyright (c) 2011-2016 Broad Institute, Aiden Lab

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <set>
#include <vector>
#include <streambuf>
#include <curl/curl.h>
#include "zlib.h"
#include "straw.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;



class MyHFile {
public:
    string prefix = "http"; // HTTP code
    bool isHttp = false;
    ifstream fin;
    CURL *curl;
    int64_t master = 0LL;
    map<string, chromosome> chromosomeMap;
    string genomeID;
    int32_t numChromosomes = 0;
    int32_t version = 0;
    int64_t nviPosition = 0LL;
    int64_t nviLength = 0LL;
    vector<int32_t> resolutions;
    static int64_t totalFileSize;

    vector<double> readNormalizationVectorFromFooter(indexEntry cNormEntry) {
        char *buffer;
        if (isHttp) {
            buffer = getData(curl, cNormEntry.position, cNormEntry.size);
        } else {
            buffer = new char[cNormEntry.size];
            fin.seekg(cNormEntry.position, ios::beg);
            fin.read(buffer, cNormEntry.size);
        }
        membuf sbuf3(buffer, buffer + cNormEntry.size);
        istream bufferin(&sbuf3);
        vector<double> cNorm = readNormalizationVector(bufferin, version);
        delete buffer;
        return cNorm;
    }
};

class MZData {
public:
    indexEntry c1NormEntry, c2NormEntry;
    int64_t myFilePos = 0LL;
    vector<double> expectedValues;
    bool foundFooter = false;
    vector<double> c1Norm;
    vector<double> c2Norm;
    int32_t c1 = 0;
    int32_t c2 = 0;
    string matrixType;
    string norm;
    string unit;
    int32_t resolution = 0;
    int32_t numBins1 = 0;
    int32_t numBins2 = 0;

    MatrixZoomData(HiCFile *hiCFile, const chromosome &chrom1, const chromosome &chrom2, const string &matrixType,
                   const string &norm, const string &unit, int32_t resolution) {

        int32_t c01 = chrom1.index;
        int32_t c02 = chrom2.index;
        if (c01 <= c02) { // default is ok
            this->c1 = c01;
            this->c2 = c02;
            this->numBins1 = static_cast<int32_t>(chrom1.length / resolution);
            this->numBins2 = static_cast<int32_t>(chrom2.length / resolution);
        } else { // flip
            this->c1 = c02;
            this->c2 = c01;
            this->numBins1 = static_cast<int32_t>(chrom2.length / resolution);
            this->numBins2 = static_cast<int32_t>(chrom1.length / resolution);
        }

        this->matrixType = matrixType;
        this->norm = norm;
        this->unit = unit;
        this->resolution = resolution;

        if (hiCFile->isHttp) {
            char *buffer2;
            int64_t bytes_to_read = hiCFile->totalFileSize - hiCFile->master;
            buffer2 = getData(hiCFile->curl, hiCFile->master, bytes_to_read);
            membuf sbuf2(buffer2, buffer2 + bytes_to_read);
            istream bufin2(&sbuf2);
            foundFooter = readFooter(bufin2, hiCFile->master, hiCFile->version, c1, c2, matrixType, norm, unit,
                                     resolution,
                                     myFilePos,
                                     c1NormEntry, c2NormEntry, expectedValues);
            delete buffer2;
        } else {
            hiCFile->fin.seekg(hiCFile->master, ios::beg);
            foundFooter = readFooter(hiCFile->fin, hiCFile->master, hiCFile->version, c1, c2, matrixType, norm,
                                     unit,
                                     resolution, myFilePos,
                                     c1NormEntry, c2NormEntry, expectedValues);
        }

        if (!foundFooter) {
            return;
        }

        if (norm != "NONE") {
            c1Norm = hiCFile->readNormalizationVectorFromFooter(c1NormEntry);
            if (c1 == c2) {
                c2Norm = c1Norm;
            } else {
                c2Norm = hiCFile->readNormalizationVectorFromFooter(c2NormEntry);
            }
        }
    }
};


/**
namespace py = pybind11;

PYBIND11_MODULE(playgroundC, m) {
m.doc() = "Fast tool for reading H files";

m.def("strawC", &straw, "get contact records");
m.def("getRecords", &getBlockRecordsWithNormalization, "get contact records using normalization info");

py::class_<contactRecord>(m, "contactRecord")
.def(py::init<>())
.def_readwrite("binX", &contactRecord::binX)
.def_readwrite("binY", &contactRecord::binY)
.def_readwrite("counts", &contactRecord::counts)
;

py::class_<chromosome>(m, "chromosome")
.def(py::init<>())
.def_readwrite("name", &chromosome::name)
.def_readwrite("index", &chromosome::index)
.def_readwrite("length", &chromosome::length)
;

#ifdef VERSION_INFO
m.attr("__version__") = VERSION_INFO;
#else
m.attr("__version__") = "dev";
#endif
}
*/

