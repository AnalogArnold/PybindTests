import numpy as np
from math import tan, radians
from dataclasses import dataclass, field


from superfastcode import array_code
a = array_code(3,4)
print(a.shape)



'''
from pyvale import dataset
import pyvale.dataset as dataset
import pyvale.mooseherder as mh
import pyvale.sensorsim as sens

# %% SIM-DATA MESH SET-UP
# Read in the mesh data - for now from the sample SIMDATA file
data_path = dataset.render_simple_block_path() # Load sample data file with a simple rectangular block in 3D to test image rendering algorithm. Returns a file path to an exodus file
sim_data = mh.ExodusReader(data_path).read_all_sim_data() # Convert the simulation output into a SimData object
displacement_components = ("disp_x","disp_y", "disp_z")
# Scale the coordinates and displ. fields to mm
sim_data = sens.scale_length_units(scale=100.0,sim_data=sim_data,disp_comps=displacement_components)
fields_to_render = ("disp_y","disp_x") # Assume our fields to render are same as to display, although this code originally differentiates between the two
render_mesh = sens.create_render_mesh(sim_data, fields_to_render ,sim_spat_dim=3,field_disp_keys=displacement_components)
render_mesh.set_pos(np.array([0.0, 0.0, 0]))

connectivity = render_mesh.connectivity
coords = render_mesh.coords
node_coords = coords[connectivity,:3]
'''