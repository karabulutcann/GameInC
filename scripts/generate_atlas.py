from PIL import Image
import os

def generate_texture_atlas(input_folder, output_file, atlas_size):
    # Get all image file names in the input folder
    texture_files = [f for f in os.listdir(input_folder) if f.endswith(('.png', '.jpg'))]
    
    if not texture_files:
        print("No texture files found in the input folder.")
        return
    
    # Open the first image to get dimensions
    first_image = Image.open(os.path.join(input_folder, texture_files[0]))
    texture_size = first_image.size[0]  # Assuming square textures
    
    atlas_width = atlas_size[0] * texture_size
    atlas_height = atlas_size[1] * texture_size
    
    # Create a blank atlas image
    atlas = Image.new('RGBA', (atlas_width, atlas_height))
    
    # Place each texture in the atlas
    for index, texture_file in enumerate(texture_files):
        if index >= atlas_size[0] * atlas_size[1]:
            break  # Stop if we exceed the atlas grid
        
        texture = Image.open(os.path.join(input_folder, texture_file))
        x = (index % atlas_size[0]) * texture_size
        y = (index // atlas_size[0]) * texture_size
        atlas.paste(texture, (x, y))
    
    # Save the atlas image
    atlas.save(output_file)
    print(f"Texture atlas saved as {output_file}")


if __name__ == "__main__":
    generate_texture_atlas("assets/textures", "assets/textures/texture_atlas.png", (4, 4))  # 4x4 atlas