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

//////////////////////////////////// INPUT
//double aspect_ratio = 16.0 / 9.0;
//unsigned short image_width = 400; // px
//unsigned short image_height = static_cast<unsigned short>(image_width / aspect_ratio); // px
//unsigned short number_of_samples = 1; // For anti-aliasing. Really don't expect we'll need more than a short


//std::cout << "rows" << sizeof edge0_arr / sizeof edge0_arr[0] << std::endl;
//std::cout << "cols" << sizeof edge0_arr[0] / sizeof(double) << std::endl;


namespace py = pybind11;

void render_scene(const int image_height,
    const int image_width,
    const int number_of_samples,
    const py::list& list_of_meshes,
    const py::list& list_of_cameras) {

    // Camera test_camera{ EiVector3d(-0.5, 1.1, 1.1), EiVector3d(0, 0, -1), 90 };

     // Iterate over all cameras and render an image for each
    for (pybind11::handle element : list_of_cameras) {
        pybind11::dict camera_data = pybind11::cast<pybind11::dict>(element);
        // Get camera parameters from the dict and cast it to Eigen types so it works with existing code; by reference to avoid copying data
        // Ideally we wouldn't need to create a new pyCamera and we'd just pass the dict around, but when I try to do that, it throws back an error casting NumPy array to a C++ object?
        pyCamera test_camera{ camera_data["camera_center"].cast<Eigen::Ref<EiVector3d>>(),
            camera_data["pixel_00_center"].cast<Eigen::Ref<EiVector3d>>(),
            camera_data["matrix_pixel_spacing"].cast<Eigen::Ref<Eigen::Matrix<double, 2, 3, Eigen::StorageOptions::RowMajor>>>() };

        /*
        // Iterate over the meshes in the passed scene list
        std::cout << &list_of_meshes << std::endl;
        for (pybind11::handle element : list_of_meshes) {
            pybind11::dict mesh_dict = pybind11::cast<pybind11::dict>(element);
            // Get the data from each mesh
            pybind11::array_t<int> connectivity = pybind11::cast<pybind11::array_t<int>>(mesh_dict["connectivity"]);
            pybind11::array_t<double> node_coords = pybind11::cast<pybind11::array_t<double>>(mesh_dict["coords"]);
        }
        // Get bytes from the render function and pass back to Python to write it to a file from there
        //return render_ppm_image(test_camera, connectivity, node_coords);
        */


        render_ppm_image(test_camera, list_of_meshes, image_height, image_width, number_of_samples);
    }
}

PYBIND11_MODULE(superfastcode, a) {
    a.def("cpp_render_scene", &render_scene);
}