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
//#include <curl/curl.h>
//#include "zlib.h"
#include "playground.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <algorithm>
using namespace std;
namespace py = pybind11;


class MZData {
public:
    int32_t c1 = 0;
    int32_t c2 = 0;

    MZData(int32_t a, int32_t b) {
        c1 = a * 2;
        c2 = b * -2;
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

class MyHFile {
public:
    int32_t a = 0;
    int32_t b = 0;

    MyHFile(int32_t a1, int32_t b1) {
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

    MZData getMZD() {
        return MZData(a, b);
    }
    auto quickTestVec(int32_t arrs, int32_t els){
        auto results = vector<vector<int32_t>>();
        // a = 10, b = 4
        //MyHFile *h1 = new MyHFile(seed, 4);
        //a = 3, b = 7
        //MyHFile *h2 = new MyHFile(3, 7);

        //arrays arrays of size els_in_arrays.
        for (auto i = 0; i < arrs; i++){
            results.push_back(vector<int32_t>(els));
        }

        for (auto i = 0; i < arrs; i++){
            for (auto j = 0; j < els; j++){
                results[i][j] = 255;
            }
        }
        //results adds 10



        return py::array(py::cast(results));
    }
};

// todo after the initial stuff is working
// instead of a vector, return a numpy array
std::vector<int32_t> quickTest1d(int32_t seed) {
    vector<int32_t> results = vector<int32_t>();
    // a = 10, b = 4
    MyHFile* h1 = new MyHFile(seed, 4);
    //a = 3, b = 7
    MyHFile* h2 = new MyHFile(3, 7);

    //results adds 10
    results.push_back(h1->getA());
    //results adds 4
    results.push_back(h1->getB());
    //results adds 3
    results.push_back(h2->getA());

    //MZData C1 = 20, C2 = -8
    MZData m1 = h1->getMZD();
    // results pushes 20
    results.push_back(m1.getC1());
    // results pushes -8
    results.push_back(m1.getC2());

    //a in h1 becomes 15
    h1->addToA(5);
    //we push 15 to results
    results.push_back(h1->getA());

    //we push 20 again
    results.push_back(m1.getC1());
    //we push -8 again
    results.push_back(m1.getC2());

    return results;
}

vector<vector<int32_t>> quickTest2d(int32_t arrays, int32_t els_in_array) {
    auto results = vector<vector<int32_t>>();
    // a = 10, b = 4
    //MyHFile *h1 = new MyHFile(seed, 4);
    //a = 3, b = 7
    //MyHFile *h2 = new MyHFile(3, 7);

    //arrays arrays of size els_in_arrays.
    for (auto i = 0; i < arrays; i++) {
        results.push_back(vector<int32_t>(els_in_array));
    }

    for (auto i = 0; i < arrays; i++) {
        for (auto j = 0; j < els_in_array; j++) {
            results[i][j] = 255;
        }
    }
    //results adds 10



    return results;
}


vector<double> slice(vector<double> &v, int64_t m, int64_t n){
    vector<double> vec;
    copy(v.begin() + m, v.begin() + n + 1, back_inserter(vec));
    return vec;
}


double median(vector<double> &v){
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}

void rollingMedian(vector<double> &initialValues, vector<double> &finalResult, int32_t window) {
    // window is actually a ~wing-span
    if (window < 1) {
        finalResult = initialValues;
        return;
    }

    finalResult.push_back(initialValues[0]);
    int64_t length = initialValues.size();
    for (int64_t index = 1; index < length; index++) {
        int64_t initialIndex;
        int64_t finalIndex;
        if (index < window){
            initialIndex = 0;
            finalIndex = 2*index;
        } else {
            initialIndex = index - window;
            finalIndex = index + window;
        }

        if(finalIndex > length - 1){
            finalIndex = length - 1;
        }

        vector<double> subVector = slice(initialValues, initialIndex, finalIndex);
        finalResult.push_back(median(subVector));
    }
}


int main(int argc, char* argv[]) {
    //std::vector<int32_t> result_test = quickTest(atoi(argv[1]));
    //for(int i=0; i < result_test.size(); i++)
     //   std::cout << result_test.at(i) << ' ';
    return 0;
}



//namespace py = pybind11;

//playgroundC is what we're importing
PYBIND11_MODULE(playgroundC, m) {
    m.doc() = "Playground for testing pybind11";

    //When taking function (not methods) we put semicolons after every definition


    //Lambda dunction, we specify the C++ type and parameter
    m.def("quickTestNumpy2d", [](int32_t arrays, int32_t els_in_array) {
        py::array out = py::cast(quickTest2d(arrays, els_in_array));
        return out;
        });
    m.def("quickTestNumpy1d", [](int32_t seed) {
        py::array out = py::cast(quickTest1d(seed));
        return out;
        });
    //We don't specify the arguments we just put & and then the function name
    m.def("quickTest", &quickTest1d);

    //Class name goes here. In arrowheads goes the C++ class name and in quotations goes the python classname we're going to use.
    //When defining methods not function we put semicolons at the end of all definitions

    //This entire class works
    py::class_<MZData>(m, "MZData")
        //Init is used for defining the constructor and within the constructor we define the types of the constructor parameters
        .def(py::init<int32_t, int32_t>())
        //The first argument of def is how we want to define the method when we call it from python
        //The second argument of def is the namespace and method we are retrieving from that namespace
        .def("getC1", &MZData::getC1)
        .def("getC2", &MZData::getC2)
        .def("addToC1", &MZData::addToC1)
        .def("addToC2", &MZData::addToC2)
        ;
    py::class_<MyHFile>(m, "MyHFile")
        //.def is used for methods while .def_readwrite is used for fields and being able to modify them
        .def(py::init<int32_t, int32_t>())
        .def("addToA", &MyHFile::addToA)
        .def("addToB", &MyHFile::addToB)
        .def("getA", &MyHFile::getA)
        .def("getB", &MyHFile::getB)
        .def("getMZD", &MyHFile::getMZD)
        .def("quickTestVec", &MyHFile::quickTestVec)
        ;
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
