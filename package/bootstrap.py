import os
import subprocess

print("Setting up the development environment...")

subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)

api_path = "../cores/gecko/api"
if os.path.islink(api_path) or os.path.exists(api_path):
    os.remove(api_path)

if os.name == 'nt':
    subprocess.run(["mklink", "/D", api_path, "..\\..\\extra\\core-api\\api"], shell=True, check=True)
else:
    os.symlink("../../extra/core-api/api", api_path)

print("Development environment setup finished")
