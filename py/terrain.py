import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, Slider
from noise import pnoise2
from scipy.interpolate import PchipInterpolator

# Size
width, height = 200, 200

# Noise generation parameters
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

# Influence curve generator using monotonic cubic interpolation (PCHIP)
def create_influence_curve(points, resolution=256):
    if len(points) < 2:
        return np.linspace(0, 1, resolution), np.zeros(resolution)
    points = sorted(points)
    x, y = zip(*points)
    spline = PchipInterpolator(x, y)
    curve_x = np.linspace(0, 1, resolution)
    curve_y = spline(curve_x)
    curve_y = np.clip(curve_y, 0, 1)
    return curve_x, curve_y

# Setup UI layout early so we can use it in class
ax_maps = {}
for key in ["height", "erosion", "continental", "peaks", "combined"]:
    fig = plt.figure(figsize=(6, 6))
    ax = fig.add_subplot(1,1,1)
    ax.set_title(f"{key.title()} Map")
    ax.axis("off")
    ax_maps[key] = ax

# Create separate windows for influence curves
fig, axs = plt.subplots(3, 1, figsize=(6, 8))

ax_curves = {
    "erosion": axs[0],
    "continental": axs[1],
    "peaks": axs[2]
}

# Placeholder for editors, to be filled after class definition
editors = {}
ims = {}

# Combined terrain display

# Interactive influence curve editor
class InfluenceEditor:
    def __init__(self, name, ax_curve):
        self.name = name
        self.points = [(0.0, 0.0), (1.0, 1.0)]
        self.selected_point = None
        self.ax = ax_curve

        self.line, = self.ax.plot([], [], label=f"{name.title()} Curve", lw=2)
        self.scatter = self.ax.scatter([], [], color='red', picker=True, s=100, zorder=3)
        self.ax.set_xlim(0, 1)
        self.ax.set_ylim(0, 1)
        self.ax.set_title(f"{name.title()} Influence Curve")

        self.dragging = False

        self.update_curve()
        self.cid_click = self.ax.figure.canvas.mpl_connect('button_press_event', self.on_click)
        self.cid_release = self.ax.figure.canvas.mpl_connect('button_release_event', self.on_release)
        self.cid_motion = self.ax.figure.canvas.mpl_connect('motion_notify_event', self.on_motion)

    def update_curve(self):
        self.points = sorted(self.points)
        x, y = zip(*self.points)
        cx, cy = create_influence_curve(self.points)
        self.line.set_data(cx, cy)
        self.scatter.set_offsets(np.column_stack((x, y)))
        self.ax.relim()
        self.ax.autoscale_view()
        self.ax.figure.canvas.draw_idle()
        # update_combined_terrain()

    def on_click(self, event):
        if event.inaxes != self.ax:
            return
        contains, attr = self.scatter.contains(event)
        if contains:
            ind = attr['ind'][0]
            self.selected_point = ind
            self.dragging = True
        else:
            self.points.append((event.xdata, event.ydata))
            self.update_curve()

    def on_release(self, event):
        self.dragging = False
        self.selected_point = None

    def on_motion(self, event):
        if not self.dragging or self.selected_point is None:
            return
        if event.inaxes != self.ax:
            return
        x = np.clip(event.xdata, 0, 1)
        y = np.clip(event.ydata, 0, 1)
        self.points[self.selected_point] = (x, y)
        self.update_curve()

    def get_curve(self):
        _, y = create_influence_curve(self.points)
        return y

    def get_curve_array(self):
        return self.get_curve()[np.linspace(0, 255, width * height).astype(int).reshape(height, width)]

# Generate all noise maps
maps = {key: generate_noise(**params[key]) for key in params}
maps["combined"] = np.ones((height, width))  # Initialize combined map
editors.update({name: InfluenceEditor(name, ax) for name, ax in ax_curves.items()})

# Display each map
for key, ax in ax_maps.items():
    im = ax.imshow(maps[key], cmap="terrain")
    ax.set_title(f"{key.title()} Map")
    ims[key] = im

# Initialize editors after class definition
def update_combined_terrain():
    base = maps["height"]
    normalized = (base - base.min()) / (base.max() - base.min())
    idx = (normalized * 255).astype(int)

    erosion_influence = editors["erosion"].get_curve()[idx]
    continental_influence = editors["continental"].get_curve()[idx]
    peaks_influence = editors["peaks"].get_curve()[idx]

    combined = base + maps["erosion"] * erosion_influence \
                   + maps["continental"] * continental_influence \
                   + maps["peaks"] * peaks_influence
    combined = (combined - combined.min()) / (combined.max() - combined.min())
    ims["combined"].set_data(base)
    ax_maps["combined"].set_title("Combined Terrain (Live Update)")
    ims["combined"].figure.canvas.draw_idle()
    
update_combined_terrain()

plt.tight_layout()
plt.show()