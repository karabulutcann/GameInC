from PIL import Image

# Load the black and white grass texture
bw_texture_path = "assets/textures/grass_top.png"
bw_texture = Image.open(bw_texture_path).convert("L")  # Ensure grayscale

# Load the grass side texture to extract the green color
grass_side_path = "assets/textures/grass_side.png"
grass_side = Image.open(grass_side_path)

# Extract a representative green color from the top portion of the grass_side texture
grass_side_rgb = grass_side.convert("RGB")
width, height = grass_side_rgb.size
green_sample_region = grass_side_rgb.crop((0, 0, width, height // 3))  # Top third for green

# Compute the average color of the sampled region
pixels = list(green_sample_region.getdata())
avg_green = tuple(sum(x) // len(pixels) for x in zip(*pixels))
custom_green = (80, 203, 55)

# Apply the green tint to the black and white texture
colored_texture = Image.merge("RGB", (
    bw_texture.point(lambda p: p * custom_green[0] // 255),
    bw_texture.point(lambda p: p * custom_green[1] // 255),
    bw_texture.point(lambda p: p * custom_green[2] // 255)
))

# Save the new texture
colored_texture_path = "grass_top_colored.png"
colored_texture.save(colored_texture_path)
colored_texture_path
