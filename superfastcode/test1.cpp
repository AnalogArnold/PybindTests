#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <Windows.h>
#include <cmath>
#include "./Eigen/Dense"
#include <cstdint>

// Struct for retrieving array dimensions from Python
struct arrayDimensions {
    uint32_t rows;
    uint32_t cols;
};

// Aliases for the commonly used Eigen types with row-major storage order
using EiMatrix4d = Eigen::Matrix<double, 4, 4, Eigen::StorageOptions::RowMajor>; // 4x4 matrix
using EiVector3d = Eigen::Matrix<double, 1, 3, Eigen::StorageOptions::RowMajor>; // row vector (3D)


namespace py = pybind11;

void getSimData(Eigen::Ref<Eigen::MatrixXd> matrix) {
    dims.rows = matrix.rows();
    dims.cols = matrix.cols();
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
    a.def("array_code", &make_array);
}
