from ast import List
import numpy as np
from math import tan, radians
from dataclasses import dataclass, field

'''
from superfastcode import array_code
a = array_code(3,4)
print(a.shape)
'''

def normalise_vector(vector: np.ndarray) -> np.ndarray:
    '''Returns the normalised vector, i.e., with length 1.0.'''
    return vector / np.sqrt(vector.dot(vector))

#%% Scene builder
# Input: meshes for the objects + object coordinates in world coordinate system + cameraS(!) position (lights to be added)
# Functions: load meshes, specify materials (later), specify cameras and their params, create the scene
# Output: a scene dataclass containing all information to render the scene + an option to save the scene dataclass to file for later use

# %% SIM-DATA MESH SET-UP
# Read in the mesh data - for now from the sample SIMDATA file

import pyvale.dataset as dataset
import pyvale.mooseherder as mh
import pyvale.sensorsim as sens

# 1. PASSING A FLAT NUMPY ARRAY TO C++ AND GETTING ITS ROWS BACK

# data_path = dataset.render_simple_block_path() # Load sample data file with a simple rectangular block in 3D to test image rendering algorithm. Returns a file path to an exodus file
# sim_data = mh.ExodusReader(data_path).read_all_sim_data() # Convert the simulation output into a SimData object
# displacement_components = ("disp_x","disp_y", "disp_z")
# # Scale the coordinates and displ. fields to mm
# sim_data = sens.scale_length_units(scale=100.0,sim_data=sim_data,disp_comps=displacement_components)
# fields_to_render = ("disp_y","disp_x") # Assume our fields to render are same as to display, although this code originally differentiates between the two
# render_mesh = sens.create_render_mesh(sim_data, fields_to_render ,sim_spat_dim=3,field_disp_keys=displacement_components)
# render_mesh.set_pos(np.array([0.0, 0.0, 0]))

# connectivity = render_mesh.connectivity
# coords = render_mesh.coords
# node_coords = coords[connectivity,:3]
# node_coords_flat = node_coords.reshape(node_coords.shape[0],node_coords.shape[1]*node_coords.shape[2]) # Shape to have a 2D array that can be mapped to Eigen matrix. Size is (num_elements, num_nodes_per_element*3) for triangles
# #print(node_coords)
# #print(node_coords_flat) 


# from superfastcode import cpp_simdata_rows
# print(cpp_simdata_rows(node_coords_flat))

def simdata_to_mesh(pypath, field_components, fields_to_render, scale):
    # Convert the simulation output into a SimData object
    sim_data = mh.ExodusReader(pypath).read_all_sim_data()
    # Scale the coordinates and displ. fields to mm
    sim_data = sens.scale_length_units(scale=scale,sim_data=sim_data,disp_comps=field_components)
    render_mesh = sens.create_render_mesh(sim_data, fields_to_render ,sim_spat_dim=3,field_disp_keys=field_components)
    return render_mesh

def get_mesh_data(pypath, field_components=("disp_x","disp_y", "disp_z"), fields_to_render = ("disp_y", "disp_x"), world_position = None, scale = 100.0):
    '''Returns the mesh data as a numpy array.'''
    render_mesh = simdata_to_mesh(data_path, field_components, fields_to_render, scale)
    if world_position is not None:
        render_mesh.set_pos(world_position)
    connectivity = render_mesh.connectivity
    coords = render_mesh.coords[:,:3]
    #node_coords = coords[connectivity,:3]
    x_disp_node_vals = render_mesh.fields_render[:,1, 1] # Field displacement_x at timestep 1 for all nodes. Use this for coloring somehow
    x_disp_node_norm = (x_disp_node_vals - x_disp_node_vals.min())/(x_disp_node_vals.max()-x_disp_node_vals.min()) # Normalize displacement values, scaling them to range [0,1] so they can map to color intensities
    # Approach 2 - taking averages and stacking them together
    node_colors = np.column_stack((x_disp_node_norm, x_disp_node_norm, x_disp_node_norm))  # Convert each scalar to an RGB triplet
    face_colors = np.mean(node_colors[connectivity],axis=1)  # Compute each face's colour as the average of its 3 node colours
    # Approach 1 - using a colour map to assign an rgb value
    # cmap = plt.get_cmap('viridis')
    # face_colors = cmap(x_disp_node_norm)[:,:3]
    return {"connectivity": connectivity, "coords": coords, "face_colors": face_colors}


data_path = dataset.render_simple_block_path() # Load sample data file with a simple rectangular block in 3D to test image rendering algorithm. Returns a file path to an exodus file
rect_block = get_mesh_data(data_path)
rect_block2 = get_mesh_data(data_path, world_position=[-2.0, -10.0, -2.0], scale=500)
scene = list()
scene.append(rect_block)
scene.append(rect_block2)

from superfastcode import cpp_simdata_dictlist
cpp_simdata_dictlist(scene)


'''
@dataclass(slots=True)
class Scene:
    A dataclass to hold all information about a 3D scene to be rendered
    # World data (image, viewport, camera)
    image_width: int
    image_height: int
    camera_center: np.ndarray
    camera_target: np.ndarray
    angle_vertical_view: int
    matrix_camera_to_world: np.ndarray = field(init=False)
    matrix_world_to_camera: np.ndarray = field(init=False)
    matrix_pixel_spacing: np.ndarray = field(init=False)
    viewport_upper_left: np.ndarray = field(init=False)
    pixel_00_center: np.ndarray = field(init=False)
    pixel_centers: np.ndarray = field(init=False) # Array of pixel center coordinates (on the viewport) in the world coordinate system
    # Mesh data (to be expanded later))
    node_coords: np.ndarray


    def __post_init__(self) -> None:
        self.create_basis_matrices()
        i_indices, j_indices = np.meshgrid(np.arange(self.image_width), np.arange(self.image_height), indexing='xy')
        pixel_centers = (self.pixel_00_center[np.newaxis, np.newaxis, :] +
                     i_indices[:, :, np.newaxis] * self.matrix_pixel_spacing[0] +
                     j_indices[:, :, np.newaxis] * self.matrix_pixel_spacing[1])


     def create_basis_matrices(self) -> None:
       
        Creates camera-to-world matrix.
        
        self.matrix_camera_to_world = np.zeros((4, 4))
        basis_vector_forward, basis_vector_right, basis_vector_up, focal_length = self._compute_camera_basis_vectors()
        self.matrix_camera_to_world[:, :3] = np.array(
            [basis_vector_right, basis_vector_up, basis_vector_forward, self.camera_center])
        self.matrix_camera_to_world[3] = np.array([0.0, 0.0, 0.0, 1.0])
        self.matrix_world_to_camera = np.linalg.inv(self.matrix_camera_to_world)
        self._create_viewport(basis_vector_forward, basis_vector_right, basis_vector_up, focal_length)
        # return self.matrix_camera_to_world, self.matrix_world_to_camera

    def _compute_camera_basis_vectors(self):  # camera_center = lookfrom, point_camera_target = lookat
        Creates the camera basis vectors from the camera center and the point the camera is looking at.
        basis_vector_forward = self.camera_center - self.point_camera_target
        focal_length = np.sqrt(basis_vector_forward.dot(basis_vector_forward))
        vector_view_up = np.array([0.0, 1.0,
                                   0.0])  # View up vector orthogonal to basis_vector_right. Defines sideways tilt. Value can be changed, this is the default for the camera to be straight.
        basis_vector_right = np.cross(vector_view_up, basis_vector_forward)
        basis_vector_up = np.cross(basis_vector_forward, basis_vector_right)
        return normalise_vector(basis_vector_forward), normalise_vector(basis_vector_right), normalise_vector(
            basis_vector_up), focal_length

    def _create_viewport(self, basis_vector_forward, basis_vector_right, basis_vector_up, focal_length) -> None:
        #Creates the viewport from the camera basis vectors and the focal length.
        Returns pixel spacing vectors and the 0,0-positions for the pixel and the upper left corner of the viewport.
        h_temp = tan(self.angle_vertical_view / 2)
        viewport_height = 2 * h_temp * focal_length  # world units (arbitrary)
        viewport_width = viewport_height * (image_width / image_height)  # world units (arbitrary)
        # Viewport basis vectors
        vector_viewport_x_axis = viewport_width * basis_vector_right  # Vu
        vector_viewport_y_axis = (-viewport_height) * basis_vector_up  # Vw
        # Pixel spacing vectors (delta vectors)
        vector_pixel_spacing_x = vector_viewport_x_axis / image_width  # Delta u
        vector_pixel_spacing_y = vector_viewport_y_axis / image_height  # Delta v
        self.matrix_pixel_spacing = np.array([vector_pixel_spacing_x, vector_pixel_spacing_y])  # Store as an array
        # 0,0-positions
        self.viewport_upper_left = self.camera_center - basis_vector_forward - vector_viewport_x_axis / 2 - vector_viewport_y_axis / 2
        self.pixel_00_center = self.viewport_upper_left + 0.5 * (vector_pixel_spacing_x + vector_pixel_spacing_y)

'''