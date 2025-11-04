#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "./Eigen/Dense"
#include <cstdint>

#include <fstream>
#include <iostream>
#include <array>
#include <iostream>
#include <vector>
#include <chrono>
#include "eigen_types.h"


namespace py = pybind11;


void process_list_of_dicts(py::list list_of_dicts) {
    for (auto element : list_of_dicts) {
        py::dict dict = py::cast<py::dict>(element);
        // Process each dictionary
        for (auto item : dict) {
            // Access key-value pairs
            py::print(item);
        }
    }
}


// Creates a dynamic-size Eigen matrix of given shape (filled with zeros for demo)
Eigen::MatrixXd make_eigen_matrix(int nrows, int ncols) {
    return Eigen::MatrixXd::Zero(nrows, ncols);
}

// A function that creates a 2D NumPy array (initialized with zeros)
py::array_t<int> make_array(int nrows, int ncols) {
    py::array_t<int> arr({ nrows, ncols });
    return arr;
}


PYBIND11_MODULE(superfastcode, a) {
    a.def("cpp_simdata_dictlist", &process_list_of_dicts);
}