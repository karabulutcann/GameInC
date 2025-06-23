import subprocess
import os
import shutil
import argparse

def run_cmake_and_build(args):
    try:
        # Set the generator based on the chosen build tool
        if args.build_tool == 'ninja':
            generator = "Ninja"
        elif args.build_tool == 'vscode':
            generator = "Visual Studio Code"
        else:
            raise ValueError(f"Unsupported build tool: {args.build_tool}")


        if args.project == "project":
            print("Building project...")
            cwd = os.getcwd()
        elif args.project == "playground":
            print("Building playground...")
            cwd = os.path.join(os.getcwd(), "playground")
        elif args.project == "test":
            print("Building test...")
            cwd = os.path.join(os.getcwd(), "test")
        else:
            raise ValueError(f"Unsupported project: {args.project}")
        
        # before running cmake delete the build folder if old one is compiled with different generator check the build.ninja file is exist in the build folder
        if os.path.exists(os.path.join(cwd, "build")) and ((args.build_tool != 'ninja' and os.path.exists(os.path.join(cwd, "build/build.ninja"))) or (args.build_tool == 'ninja' and not os.path.exists(os.path.join(cwd, "build/build.ninja")))):
            print("build.ninja file found in the build folder, deleting the build folder...")
            shutil.rmtree(os.path.join(cwd, "build"))

        # Run cmake to generate the build files using the chosen build tool
        print(f"Running cmake with {generator}...")
        subprocess.run(["cmake", "-B", "build", "-G", generator], check=True, cwd=cwd)
        print(f"CMake configuration completed using {generator}.")

        # Run cmake to build the project
        subprocess.run(["cmake", "--build", "build"], check=True,cwd=cwd)
        print("Build completed successfully.")

        # Check if the compile_commands.json exists in the build folder
        if args.project == "project":
            compile_commands_path = os.path.join("build", "compile_commands.json")
            if os.path.exists(compile_commands_path):
                shutil.copy(compile_commands_path, ".")
                print("compile_commands.json copied to the current directory.")
            else:
                print("compile_commands.json not found.")

        # Check if the TestApp.exe exists in the Debug folder
        app_path = os.path.join(cwd,"build", "app.exe") if args.build_tool == "ninja" else os.path.join("build", "Debug", "app.exe")
        if os.path.exists(app_path):
            subprocess.run([app_path], check=True)
            print("app.exe executed successfully.")
        else:
            print("app.exe not found.")

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

    parser.add_argument(
        "--project",
        choices=["project","playground","test"],
        default="project",
        help="Choose the project to build: 'project', 'playground', or 'test'. Default is 'project'."
    )

    return parser.parse_args()

if __name__ == "__main__":
    # Parse command-line arguments
    args = parse_args()
    
    # Run the build process with the chosen build tool
    run_cmake_and_build(args)
