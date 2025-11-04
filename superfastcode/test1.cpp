#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "./Eigen/Dense"
#include <cstdint>
#include <corecrt_math_defines.h> // Otherwise M_PI doesn't work in VS. cmath alon works in CLion, though.


double degreesToRadians(double angleDeg) {
    // Converts degrees to radians. Used to convert the angle of vertical view.
    return angleDeg * M_PI / 180;
}

// Aliases for the commonly used Eigen types with row-major storage order
using EiMatrix4d = Eigen::Matrix<double, 4, 4, Eigen::StorageOptions::RowMajor>; // 4x4 matrix
using EiVector3d = Eigen::Matrix<double, 1, 3, Eigen::StorageOptions::RowMajor>; // row vector (3D)
using RmMatrixXd = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>; // row-major dynamic-size matrix, since MatrixXd is column-major by default

namespace py = pybind11;



int getSimData(Eigen::Ref<RmMatrixXd> node_coords) {
    return node_coords.rows();
}

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

/*
PYBIND11_MODULE(superfastcode, a) {
    a.def("cpp_simdata_rows", &getSimData);
}
*/