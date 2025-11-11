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

void render_ppm_image(const Eigen::Ref<const EiVector3d>& camera_center,
    const Eigen::Ref<const EiVector3d>& pixel_00_center,
    const Eigen::Ref<const Eigen::Matrix<double, 2, 3, Eigen::StorageOptions::RowMajor>>& matrix_pixel_spacing,
    //std::string render_ppm_image(const Camera& camera1,
    const pybind11::list& list_of_meshes,
    const int image_height,
    const int image_width,
    const int number_of_samples);
