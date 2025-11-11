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
#include "render.h"


namespace py = pybind11;

void render_scene(const int image_height,
    const int image_width,
    const int number_of_samples,
    const std::vector<py::array_t<double>> scene_coords,
    std::vector<py::array_t<int>> scene_connectivity,
    const py::list& list_of_cameras) {

    // Camera test_camera{ EiVector3d(-0.5, 1.1, 1.1), EiVector3d(0, 0, -1), 90 };

     // Iterate over all cameras and render an image for each
    for (pybind11::handle element : list_of_cameras) {
        pybind11::dict camera_data = pybind11::cast<pybind11::dict>(element);
        // Get camera parameters from the dict and cast it to Eigen types so it works with existing code; by rference to avoid copying data
      
        Eigen::Ref<const EiVector3d> camera_center = camera_data["camera_center"].cast<Eigen::Ref<const EiVector3d>>();
        Eigen::Ref<const EiVector3d> pixel_00_center = camera_data["pixel_00_center"].cast<Eigen::Ref<const EiVector3d>>();
		Eigen::Ref<const Eigen::Matrix<double, 2, 3, Eigen::StorageOptions::RowMajor>> matrix_pixel_spacing = camera_data["matrix_pixel_spacing"].cast<Eigen::Ref<const Eigen::Matrix<double, 2, 3, Eigen::StorageOptions::RowMajor>>>();

        // Get bytes from the render function and pass back to Python to write it to a file from there
        //return render_ppm_image(test_camera, connectivity, node_coords);
        render_ppm_image(camera_center, pixel_00_center, matrix_pixel_spacing, scene_coords, scene_connectivity, image_height, image_width, number_of_samples);
    }
}

PYBIND11_MODULE(superfastcode, a) {
    a.def("cpp_render_scene", &render_scene);
}