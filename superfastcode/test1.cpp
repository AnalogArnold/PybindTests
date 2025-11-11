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
#include "camera.h"
#include "render.h"

//////////////////////////////////// INPUT
double aspect_ratio = 16.0 / 9.0;
unsigned short image_width = 400; // px
unsigned short image_height = static_cast<unsigned short>(image_width / aspect_ratio); // px
unsigned short number_of_samples = 50; // For anti-aliasing. Really don't expect we'll need more than a short


//std::cout << "rows" << sizeof edge0_arr / sizeof edge0_arr[0] << std::endl;
//std::cout << "cols" << sizeof edge0_arr[0] / sizeof(double) << std::endl;
/*
int main() {
    //Camera test_camera{EiVector3d(0, 1, 1), EiVector3d(0, 0, -1), 90};
    Camera test_camera{ EiVector3d(-0.5, 1.1, 1.1), EiVector3d(0, 0, -1), 90 };
    // Mesh from simdata. Copied by force for now.
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
*/

namespace py = pybind11;

// WIP, ordering of this loop will not work for many meshes
void process_list_of_dicts(py::list list_of_dicts) {
    // Loop over the meshes in the scene
    Camera test_camera{ EiVector3d(-0.5, 1.1, 1.1), EiVector3d(0, 0, -1), 90 };
    py::array_t<int> connectivity;
    py::array_t<double> node_coords;

    for (auto element : list_of_dicts) {
        py::dict dict = py::cast<py::dict>(element);
        // Get the data from each mesh
        connectivity = py::cast<py::array_t<int>>(dict["connectivity"]);
        node_coords = py::cast<py::array_t<double>>(dict["coords"]);
    }
    // Get bytes from the render function and pass back to Python to write it to a file from there
    render_ppm_image(test_camera, connectivity, node_coords);
    
}


PYBIND11_MODULE(superfastcode, a) {
    a.def("cpp_simdata_dictlist", &process_list_of_dicts);
}
