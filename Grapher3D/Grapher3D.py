import plotly.io as pio
import plotly.graph_objects as go
import numpy as np

# Define original reference point and vectors with start and end coordinates
origin = np.array([0, 0, 0])
beam_start = np.array([0, 0, -5])
beam_end = np.array([0, 0, 5])
r_beam_start = origin
r_beam_end = r_beam_start + np.array([0, 2, 0])
p_part_start = np.array([0, 0, -3])
# Rotate the direction vector around the Z-axis (which is X in this system) by -30 degrees
theta = np.radians(0)
direction = np.array([0, 2, 3])
rotation_matrix = np.array([
    [np.cos(theta), -np.sin(theta), 0],
    [np.sin(theta),  np.cos(theta), 0],
    [0,              0,             1]
])
rotated_direction = rotation_matrix @ direction
p_part_end = p_part_start + 2 * rotated_direction

# Compute the projected and perpendicular components of r_beam along p_part
p_part_vec = p_part_end - p_part_start
p_part_unit = p_part_vec / np.linalg.norm(p_part_vec)
r_beam_vec = r_beam_end - r_beam_start
r_proj = np.dot(r_beam_vec, p_part_unit) * p_part_unit
r_proj_end = r_beam_start + r_proj
r_perp = r_beam_vec - r_proj
r_perp_end = r_beam_start + r_perp

# List of vectors to be drawn, including their colors and LaTeX-formatted labels
vector_data = [
    (beam_start, beam_end, 'black', r'$\hat{z}$'),
    (r_beam_start, r_beam_end, 'red', r'$\vec{r}_{beam}$'),
    (p_part_start, p_part_end, 'blue', r'$\vec{p}_{part}$')
]

vector_data.append((r_beam_start, r_proj_end, 'red', r'$\vec{r}_{beam,\parallel}$'))
vector_data.append((r_beam_start, r_perp_end, 'red', r'$\vec{r}_{bean,\perp}$'))

# Initialize 3D figure
fig = go.Figure()

for start, end, color, label in vector_data:
    # Add each vector as a shortened line (to leave space for arrowhead), remapped from (x, y, z) to (x->Z, z->Y, y->X)
    end_trimmed = start + 0.95 * (end - start)  # Shorten line to 95% of vector length to avoid overlap with arrowhead
    fig.add_trace(go.Scatter3d(
        x=[start[0], end_trimmed[0]],  # X → Z-axis (labeled "Z")
        y=[start[2], end_trimmed[2]],  # Z → Y-axis (labeled "Y")
        z=[start[1], end_trimmed[1]],  # Y → X-axis (labeled "X")
        mode='lines',
        line=dict(color=color, width=3),
        marker=dict(size=4)
    ))

# Add cone-shaped arrowheads to each vector tip
for start, end, color, _ in vector_data:
    direction = end - start
    unit_dir = direction / np.linalg.norm(direction)  # Normalize vector direction
    cone_pos = end
    fig.add_trace(go.Cone(
        x=[cone_pos[0]], y=[cone_pos[2]], z=[cone_pos[1]],  # Remap coordinates for cone placement (X->Z, Z->Y, Y->X)
        u=[unit_dir[0]], v=[unit_dir[2]], w=[unit_dir[1]],  # Remap direction vector components accordingly
        sizemode="absolute", sizeref=0.6, anchor="tip",
        showscale=False, colorscale=[[0, color], [1, color]]
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
        showarrow=False,
        x=end[0], y=end[2], z=end[1],  # remap (x, y, z) -> (x->Z, z->Y, y->X)
        text=label,
        font=dict(color=color, size=16),
        xanchor='left'
    ))

annotations.append(dict(
    showarrow=False,
    x=r_beam_start[0], y=r_beam_start[2], z=r_beam_start[1]-0.5,
    text=r'$\vec{V}_{z,\mathrm{true}}$',
    font=dict(color='green', size=16),
    xanchor='left'
))

annotations.append(dict(
    showarrow=False,
    x=p_part_start[0], y=p_part_start[2], z=p_part_start[1]-0.5,
    text=r'$\vec{V}_{z,\mathrm{rec}}$',
    font=dict(color='purple', size=16),
    xanchor='left'
))

fig.add_trace(go.Scatter3d(
    x=[r_beam_start[0]],
    y=[r_beam_start[2]],
    z=[r_beam_start[1]],
    mode='markers',
    marker=dict(size=5, color='green'),
    showlegend=False
))

fig.add_trace(go.Scatter3d(
    x=[p_part_start[0]],
    y=[p_part_start[2]],
    z=[p_part_start[1]],
    mode='markers',
    marker=dict(size=5, color='purple'),
    showlegend=False
))

# Configure scene layout, axis titles and directions, and camera view
fig.update_layout(
    scene=dict(
        xaxis=dict(title="Z", range=[-6, 6], autorange="reversed"),  # X axis shows Z component reversed
        yaxis=dict(title="X", range=[-6, 6]),  # Y axis shows X component
        zaxis=dict(title="Y", range=[-6, 6]),  # Z axis shows Y component
        camera=dict(eye=dict(x=1.2*1.4, y=0.7*1.4, z=0.03*1.4)),  # Set camera position for better view
        annotations=annotations
    ),
    title="Z-axis → Right, Y-axis → Up",
    showlegend=False,
    width=800,
    height=700
)

# Display the interactive 3D plot
fig.show()