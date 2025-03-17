import subprocess
import os
import shutil
import argparse

def run_cmake_and_build(build_tool):
    try:
        # Set the generator based on the chosen build tool
        if build_tool == 'ninja':
            generator = "Ninja"
        elif build_tool == 'vscode':
            generator = "Visual Studio Code"
        else:
            raise ValueError(f"Unsupported build tool: {build_tool}")

        # Run cmake to generate the build files using the chosen build tool
        print(f"Running cmake with {generator}...")
        subprocess.run(["cmake", "-B", "build", "-G", generator], check=True)
        print(f"CMake configuration completed using {generator}.")

        # Run cmake to build the project
        subprocess.run(["cmake", "--build", "build"], check=True)
        print("Build completed successfully.")

        # Check if the compile_commands.json exists in the build folder
        compile_commands_path = os.path.join("build", "compile_commands.json")
        if os.path.exists(compile_commands_path):
            shutil.copy(compile_commands_path, ".")
            print("compile_commands.json copied to the current directory.")
        else:
            print("compile_commands.json not found.")

        # Check if the TestApp.exe exists in the Debug folder
        test_app_path = os.path.join("build", "TestApp.exe") if build_tool == "ninja" else os.path.join("build", "Debug", "TestApp.exe")
        if os.path.exists(test_app_path):
            subprocess.run([test_app_path], check=True)
            print("TestApp.exe executed successfully.")
        else:
            print("TestApp.exe not found.")

    except subprocess.CalledProcessError as e:
        print(f"An error occurred during the process: {e}")
    except ValueError as e:
        print(e)

def parse_args():
    # Create the argument parser
    parser = argparse.ArgumentParser(description="Build and run the project.")
    
    # Add argument for choosing the build tool
    parser.add_argument(
        "--build-tool", 
        choices=["ninja", "vscode"], 
        default="ninja", 
        help="Choose the build tool: 'ninja' or 'vscode'. Default is 'ninja'."
    )

    return parser.parse_args()

if __name__ == "__main__":
    # Parse command-line arguments
    args = parse_args()
    
    # Run the build process with the chosen build tool
    run_cmake_and_build(args.build_tool)
