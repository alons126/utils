"""
Grapher3D.py

This script uses Plotly to generate an interactive 3D visualization of vectors in space, including custom coordinate mappings, projection decomposition, and annotated labels.
The coordinate system is remapped so the Z-axis points right and Y-axis points up, matching a user-defined display orientation. Vectors include beam direction, a reconstructed particle momentum, and components of another vector projected along and perpendicular to the particle momentum. Labels and markers are added for clarity.

"""

import plotly.io as pio  # Plotly input/output utilities
import plotly.graph_objects as go  # Core Plotly graphing functionality
import numpy as np  # Numerical operations and array handling

# Define original reference point and vectors with start and end coordinates
origin = np.array([0, 0, 0])  # Origin point in 3D space
beam_start = np.array([0, 0, -5])  # Start point of the beam vector along Z-axis
beam_end = np.array([0, 0, 5])  # End point of the beam vector along Z-axis
r_beam_start = origin  # Start point of r_beam vector at origin
r_beam_end = r_beam_start + np.array([0, 2, 0])  # End point of r_beam vector shifted along Y-axis
p_part_start = np.array([0, 0, -3])  # Start point of particle momentum vector

# Rotate the direction vector around the Z-axis (which is X in this system) by -30 degrees
theta = np.radians(0)  # Rotation angle in radians (currently zero)
direction = np.array([0, 2, 3])  # Direction vector to be rotated
rotation_matrix = np.array([
    [np.cos(theta), -np.sin(theta), 0],  # Rotation matrix row 1
    [np.sin(theta),  np.cos(theta), 0],  # Rotation matrix row 2
    [0,              0,             1]   # Rotation matrix row 3 (Z-axis unchanged)
])
rotated_direction = rotation_matrix @ direction  # Apply rotation to direction vector
p_part_end = p_part_start + 2 * rotated_direction  # Scale and translate rotated vector to get end point

# Compute the projected and perpendicular components of r_beam along p_part
p_part_vec = p_part_end - p_part_start  # Vector representing particle momentum
p_part_unit = p_part_vec / np.linalg.norm(p_part_vec)  # Unit vector in direction of particle momentum
r_beam_vec = r_beam_end - r_beam_start  # Vector representing r_beam
r_proj = np.dot(r_beam_vec, p_part_unit) * p_part_unit  # Projection of r_beam onto p_part
r_proj_end = r_beam_start + r_proj  # End point of the projected vector
r_perp = r_beam_vec - r_proj  # Component of r_beam perpendicular to p_part
r_perp_end = r_beam_start + r_perp  # End point of the perpendicular component

# List of vectors to be drawn, including their colors and LaTeX-formatted labels
vector_data = [
    (beam_start, beam_end, 'black', r'$\hat{z}$'),  # Beam vector in black labeled z-hat
    (r_beam_start, r_beam_end, 'red', r'$\vec{r}_{beam}$'),  # r_beam vector in red
    (p_part_start, p_part_end, 'blue', r'$\vec{p}_{part}$')  # Particle momentum vector in blue
]

# Append projected and perpendicular components of r_beam to vector list
vector_data.append((r_beam_start, r_proj_end, 'red', r'$\vec{r}_{beam,\parallel}$'))  # Parallel component
vector_data.append((r_beam_start, r_perp_end, 'red', r'$\vec{r}_{bean,\perp}$'))  # Perpendicular component (note typo in label)

# Initialize 3D figure for plotting
fig = go.Figure()

# Add each vector as a shortened line (to leave space for arrowhead), remapped from (x, y, z) to (x->Z, z->Y, y->X)
for start, end, color, label in vector_data:
    end_trimmed = start + 0.95 * (end - start)  # Shorten line to 95% of vector length to avoid overlap with arrowhead
    fig.add_trace(go.Scatter3d(
        x=[start[0], end_trimmed[0]],  # X coordinate mapped to Z-axis (labeled "Z")
        y=[start[2], end_trimmed[2]],  # Z coordinate mapped to Y-axis (labeled "Y")
        z=[start[1], end_trimmed[1]],  # Y coordinate mapped to X-axis (labeled "X")
        mode='lines',  # Draw lines between points
        line=dict(color=color, width=3),  # Set line color and width
        marker=dict(size=4)  # Marker size (not used for lines but included)
    ))

# Add cone-shaped arrowheads to each vector tip to indicate direction
for start, end, color, _ in vector_data:
    direction = end - start  # Vector direction
    unit_dir = direction / np.linalg.norm(direction)  # Normalize vector direction
    cone_pos = end  # Position of cone arrowhead at vector tip
    fig.add_trace(go.Cone(
        x=[cone_pos[0]], y=[cone_pos[2]], z=[cone_pos[1]],  # Remap coordinates for cone placement (X->Z, Z->Y, Y->X)
        u=[unit_dir[0]], v=[unit_dir[2]], w=[unit_dir[1]],  # Remap direction vector components accordingly
        sizemode="absolute", sizeref=0.6, anchor="tip",  # Cone size and anchor settings
        showscale=False, colorscale=[[0, color], [1, color]]  # Color of cone matching vector
    ))

# Add LaTeX-rendered labels as 3D annotations at the tip of each vector
annotations = []
for start, end, color, label in [
    (beam_start, beam_end, 'black', r'$\hat{z}$'),
    (r_beam_start, r_beam_end, 'red', r'$\vec{r}_{beam}$'),
    (p_part_start, p_part_end, 'blue', r'$\vec{p}_{part}$'),
    (r_beam_start, r_proj_end, 'red', r'$\vec{r}_{beam,\parallel}$'),
    (r_beam_start, r_perp_end, 'red', r'$\vec{r}_{bean,\perp}$'),
]:
    annotations.append(dict(
        showarrow=False,  # Do not show arrow for annotation
        x=end[0], y=end[2], z=end[1],  # Remap coordinates for annotation position (x->Z, z->Y, y->X)
        text=label,  # LaTeX label text
        font=dict(color=color, size=16),  # Font color and size matching vector
        xanchor='left'  # Anchor text to left of point
    ))

# Add additional annotations for true and reconstructed velocity vectors
annotations.append(dict(
    showarrow=False,
    x=r_beam_start[0], y=r_beam_start[2], z=r_beam_start[1]-0.5,  # Position slightly offset below r_beam start
    text=r'$\vec{V}_{z,\mathrm{true}}$',  # Label for true velocity
    font=dict(color='green', size=16),
    xanchor='left'
))

annotations.append(dict(
    showarrow=False,
    x=p_part_start[0], y=p_part_start[2], z=p_part_start[1]-0.5,  # Position slightly offset below particle momentum start
    text=r'$\vec{V}_{z,\mathrm{rec}}$',  # Label for reconstructed velocity
    font=dict(color='purple', size=16),
    xanchor='left'
))

# Add markers at the start points of r_beam and particle momentum vectors for emphasis
fig.add_trace(go.Scatter3d(
    x=[r_beam_start[0]],
    y=[r_beam_start[2]],
    z=[r_beam_start[1]],
    mode='markers',
    marker=dict(size=5, color='green'),  # Green marker for true velocity start
    showlegend=False
))

fig.add_trace(go.Scatter3d(
    x=[p_part_start[0]],
    y=[p_part_start[2]],
    z=[p_part_start[1]],
    mode='markers',
    marker=dict(size=5, color='purple'),  # Purple marker for reconstructed velocity start
    showlegend=False
))

# Configure scene layout, axis titles and directions, and camera view
fig.update_layout(
    scene=dict(
        xaxis=dict(title="Z", range=[-6, 6], autorange="reversed"),  # X axis shows Z component reversed
        yaxis=dict(title="X", range=[-6, 6]),  # Y axis shows X component
        zaxis=dict(title="Y", range=[-6, 6]),  # Z axis shows Y component
        camera=dict(eye=dict(x=1.2*1.4, y=0.7*1.4, z=0.03*1.4)),  # Set camera position for better view
        annotations=annotations  # Add annotations to scene
    ),
    title="Z-axis → Right, Y-axis → Up",  # Title describing coordinate remapping
    showlegend=False,  # Disable legend display
    width=800,  # Width of the figure in pixels
    height=700  # Height of the figure in pixels
)

# Display the interactive 3D plot
fig.show()