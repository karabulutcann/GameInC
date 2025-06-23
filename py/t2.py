import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from noise import pnoise2

# Size
width, height = 200, 200

# Initial parameters for each map
params = {
    "height":      {"scale": 100, "octaves": 4, "persistence": 0.5, "lacunarity": 2.0},
    "erosion":     {"scale": 50, "octaves": 3, "persistence": 0.4, "lacunarity": 2.5},
    "continental": {"scale": 150, "octaves": 2, "persistence": 0.6, "lacunarity": 1.8},
    "peaks":       {"scale": 30, "octaves": 6, "persistence": 0.7, "lacunarity": 2.2}
}

# Vectorized Perlin noise generator
def generate_noise(scale, octaves, persistence, lacunarity):
    x_idx, y_idx = np.meshgrid(np.arange(width), np.arange(height))
    x_vals = x_idx / scale
    y_vals = y_idx / scale
    noise_func = np.vectorize(lambda x, y: pnoise2(x, y, octaves=octaves,
                                                   persistence=persistence,
                                                   lacunarity=lacunarity,
                                                   repeatx=1024, repeaty=1024))
    data = noise_func(x_vals, y_vals)
    data = (data - data.min()) / (data.max() - data.min())
    return data

# Initial maps
maps = {key: generate_noise(**params[key]) for key in params}

# Figure setup
fig, ax = plt.subplots()
plt.subplots_adjust(left=0.1, bottom=0.35)
im = ax.imshow(maps["height"], cmap="terrain")
ax.set_title("Combined Terrain")
ax.axis("off")

# Blending weight sliders
blend_sliders = {}
blend_axes = {
    "erosion":     plt.axes([0.1, 0.22, 0.65, 0.02]),
    "continental": plt.axes([0.1, 0.18, 0.65, 0.02]),
    "peaks":       plt.axes([0.1, 0.14, 0.65, 0.02]),
}

blend_sliders["erosion"]     = Slider(blend_axes["erosion"],     'Erosion Weight', 0.0, 2.0, valinit=1.0)
blend_sliders["continental"] = Slider(blend_axes["continental"], 'Continentalness Weight', 0.0, 2.0, valinit=1.0)
blend_sliders["peaks"]       = Slider(blend_axes["peaks"],       'Peaks & Valleys Weight', 0.0, 2.0, valinit=1.0)

# Combine maps with blending
def blend_maps():
    base = maps["height"]
    e = maps["erosion"] * blend_sliders["erosion"].val
    c = maps["continental"] * blend_sliders["continental"].val
    p = maps["peaks"] * blend_sliders["peaks"].val

    combined = base + e + c + p
    combined = (combined - combined.min()) / (combined.max() - combined.min())
    return combined

# Update on slider change
def update(val):
    new_map = blend_maps()
    im.set_data(new_map)
    # im.figure.canvas.draw_idle()
    fig.canvas.draw_idle()

for slider in blend_sliders.values():
    slider.on_changed(update)

# Initial render
update(None)
plt.show()
