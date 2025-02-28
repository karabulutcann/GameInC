import os
import subprocess
import json


def is_git_installed():
    try:
        subprocess.run(["git", "--version"], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return True
    except FileNotFoundError:
        return False

def get_current_version(repo_path):
    try:
        result = subprocess.run(["git", "-C", repo_path, "rev-parse", "HEAD"], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return "Unknown"

def download_packages(file_path="project.json", dest_folder="packages"):
    if not is_git_installed():
        print("Error: Git is not installed. Please install Git and try again.")
        return
    
    if not os.path.exists(dest_folder):
        os.makedirs(dest_folder)
    
    with open(file_path, "r") as file:
        data = json.load(file)
        dependencies = data.get("dependencies", [])
        
        if isinstance(dependencies, dict):
            dependencies = [dependencies]
        
        for dep in dependencies:
            repo_url = dep.get("url")
            name = dep.get("name")
            
            if repo_url and name:
                repo_path = os.path.join(dest_folder, name)
                
                if os.path.exists(repo_path):
                    print(f"Updating {name}...")
                    subprocess.run(["git", "-C", repo_path, "pull"], check=True)
                else:
                    print(f"Cloning {name}...")
                    subprocess.run(["git", "clone", repo_url, repo_path], check=True)


if __name__ == "__main__":
    download_packages()