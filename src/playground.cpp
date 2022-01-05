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
    int32_t a = 0;
    int32_t b = 0;

    MyHFile(int32_t a1, int32_t b1){
        a = a1;
        b = b1;
    }

    void addToA(int32_t x1) {
        a = a + x1;
    }

    void addToB(int32_t x2) {
        b = b + x2;
    }

    int32_t getA() {
        return a;
    }

    int32_t getB() {
        return b;
    }

    MZData getMZD(){
        return MZData(a, b);
    }
};

class MZData {
public:
    int32_t c1 = 0;
    int32_t c2 = 0;

    MZData(int32_t a, int32_t b) {
        c1 = a*2;
        c2 = b*-2;
    }

    int32_t getC1() {
        return c1;
    }

    int32_t getC2() {
        return c2;
    }

    void addToC1(int32_t x1) {
        c1 = c1 + x1;
    }

    void addToC2(int32_t x2) {
        c2 = c2 + x2;
    }
};

std::vector<int32_t> quickTest(int32_t seed){
    vector<int32_t> results = vector<int32_t>();
    MyHFile h1 = new MyHFile(9, 4);
    MyHFile h2 = new MyHFile(3, 7);

    results.push_back(h1.getA());
    results.push_back(h1.getB());
    results.push_back(h2.getA());
    
    MZData m1 = h1.getMZD();
    results.push_back(m1.getC1());
    results.push_back(m1.getC2());

    h1.addToA(5);
    results.push_back(h1.getA());

    results.push_back(m1.getC1());
    results.push_back(m1.getC2());

    return results;
}


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

