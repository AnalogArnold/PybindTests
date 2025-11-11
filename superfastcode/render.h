#pragma once

#include <array>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <string>
#include<vector>

#include "eigen_types.h"
#include "camera.h"
#include "ray.h"


EiVector3d return_ray_color(const Ray& ray,
    const pybind11::list& list_of_meshes);
//const pybind11::array_t<int>& connectivity,
//const pybind11::array_t<double>& node_coords);



//pybind11::bytes render_ppm_image(const Camera& camera1,
void render_ppm_image(pyCamera& camera1,
    const pybind11::list& list_of_meshes,
    const int image_height,
    const int image_width,
    const int number_of_samples);
//const pybind11::array_t<int>& connectivity,
// const pybind11::array_t<double>& node_coords);