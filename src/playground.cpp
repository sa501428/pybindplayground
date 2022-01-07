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
#include "playground.h"
//#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>
using namespace std;


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

// todo after the initial stuff is working
// instead of a vector, return a numpy array
std::vector<int32_t> quickTest(int32_t seed){
    vector<int32_t> results = vector<int32_t>();
    MyHFile *h1 = new MyHFile(seed, 4);
    MyHFile *h2 = new MyHFile(3, 7);

    results.push_back(h1->getA());
    results.push_back(h1->getB());
    results.push_back(h2->getA());

    MZData m1 = h1->getMZD();
    results.push_back(m1.getC1());
    results.push_back(m1.getC2());

    h1->addToA(5);
    results.push_back(h1->getA());

    results.push_back(m1.getC1());
    results.push_back(m1.getC2());

    return results;
}

int main(int argc, char *argv[]){
    quickTest(10);
}



namespace py = pybind11;

//playgroundC is what we're importing
PYBIND11_MODULE(playgroundC, m) {
m.doc() = "Playground for testing pybind11";

//When taking function (not methods) we put semicolons after every definition
//m.def("", &straw, "get contact records");

//Class name goes here. In arrowheads goes the C++ class name and in quotations goes the python classname we're going to use.
//When defining methods not function we put semicolons at the end of all definitions
py::class_<MZData>(m, "MZData")
    //Init is used for defining the constructor and within the constructor we define the types of the constructor parameters
    .def(py::init<int, int>())
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
;
#ifdef VERSION_INFO
m.attr("__version__") = VERSION_INFO;
#else
m.attr("__version__") = "dev";
#endif
}


