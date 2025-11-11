from ast import List
import numpy as np
from dataclasses import dataclass, field

import pyvale.dataset as dataset
import timeit

from camera import Camera, repack_camera_data
from simdata_loader import get_mesh_data

#################################################### INPUT #####################################################
# Output image dimensions
image_width = 400  # px
aspect_ratio = 16.0 / 9.0
image_height = int(image_width / aspect_ratio)  # px
number_of_samples = 50; # for anti-aliasing
# Assume single camera for now - but can be extended to multiple cameras later
camera_center = np.array([-0.5, 1.1, 1.1])
camera_target = np.array([0, 0, -1])
angle_vertical_view = 90  # degrees

################################################ SCENE BUILDER ###############################################
# Targets:
# Input: meshes for the objects + object coordinates in world coordinate system + cameraS(!) position (lights to be added)
# Functions: load meshes, specify materials (later), specify cameras and their params, create the scene
# Output: a scene dataclass containing all information to render the scene + an option to save the scene dataclass to file for later use

# Tests: For now just load simple sample data, assuming 1 of each element

# Create a camera
camera1 = Camera(image_width, image_height, camera_center, camera_target, angle_vertical_view) # Camera for tests
#camera1 = Camera(image_width, image_height) # Default camera (parameters i.e., at world origin, no funny angles) for tests
cameras = list()
cameras.append(repack_camera_data(camera1))


# Load sample data file with a simple rectangular block in 3D to test image rendering algorithm. Returns a file path to an exodus file
data_path = dataset.render_simple_block_path() 
rect_block = get_mesh_data(data_path)
data_path = dataset.render_simple_block_path() 
rect_block2 = get_mesh_data(data_path, world_position=[-2.0, -10.0, -2.0], scale=500)

# Create and populate the scene with meshes
scene = list()
scene.append(rect_block)
#scene.append(rect_block2)

# Lights - to be added later


#print(camera_center.flags['C_CONTIGUOUS'])
#print(camera1.pixel_00_center.flags['C_CONTIGUOUS'])
#print(camera1.matrix_pixel_spacing.flags['C_CONTIGUOUS'])
#print(rect_block['connectivity'].flags['C_CONTIGUOUS'])
#print(rect_block['coords'].flags['C_CONTIGUOUS'])
#print(rect_block['face_colors'].flags['C_CONTIGUOUS'])

from superfastcode import cpp_render_scene
#cpp_render_scene(image_height, image_width, number_of_samples, scene, cameras)
#with (open("image.ppm", "wb") as f):
#    f.write(cpp_simdata_dictlist(scene))
print(timeit.timeit("cpp_render_scene(image_height, image_width, number_of_samples, scene, cameras)", globals=globals(), number=1))