#pragma once

#include <array>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <string>

#include "eigen_types.h"
#include "ray.h"
#include "camera.h"


EiVector3d return_ray_color(const Ray& ray,
    const pybind11::array_t<int>& connectivity,
    const pybind11::array_t<double>& node_coords);

void render_ppm_image(const Camera& camera1,
//void render_ppm_image(const Camera& camera1,
    const pybind11::array_t<int>& connectivity,
    const pybind11::array_t<double>& node_coords);
