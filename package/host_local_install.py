import os
import shutil
import subprocess
import hashlib
import json
import http.server
import socket

"""
Little helper script to host a local install server for Arduino IDE core installs
It creates a brand new core zip and hosts it on the local network, so it can be installed in the Arduino IDE
You need to provide the compiler and other tools by hand as of now (just copy them to the host_dir)
"""

core_version = "3.0.0"
host_dir = "install_host/"
core_zip_name = "silabs_arduino_core-"+ core_version + ".zst"
package_json_name = "package_arduinosilabs_index.json"


def main():
    # Package the core
    package_process = subprocess.Popen(["sh", "package_core.sh", "--skip-zip"])
    package_process.communicate(timeout=180)

    # Check if we're in the 'package' directory
    if package_process.returncode == 127:
        print("Make sure you're running this script directly from the 'package' directory!")
        quit(-1)

    # Create the host directory if it doesn't already exist
    if not (os.path.exists(host_dir) and os.path.isdir(host_dir)):
        os.mkdir(host_dir)

    # Remove any old packaged cores
    if os.path.exists(host_dir + core_zip_name):
        os.remove(host_dir + core_zip_name)

    # Copy the packaged file to the host_dir
    shutil.move("../" + core_zip_name, host_dir)

    # Get the filesize
    core_size = os.stat(host_dir + core_zip_name).st_size
    print(f"Core size: {core_size} bytes")

    # Get the SHA-256 of the file
    sha256_hash = hashlib.sha256()
    with open(host_dir + core_zip_name, "rb") as f:
        # Read and update hash string value in blocks of 4K
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
        core_sha256 = sha256_hash.hexdigest()
    print(f"Core SHA256: {core_sha256}")

    # Grab a copy of the core package JSON
    if os.path.exists(host_dir + package_json_name):
        os.remove(host_dir + package_json_name)
    shutil.copy(package_json_name, host_dir)

    # Update the size and the hash of the core zip in the file
    with open(host_dir + package_json_name, "r") as jsonFile:
        data = json.load(jsonFile)

    data["packages"][-1]["platforms"][-1]["size"] = core_size
    data["packages"][-1]["platforms"][-1]["checksum"] = "SHA-256:" + core_sha256

    with open(host_dir + package_json_name, "w") as jsonFile:
        json.dump(data, jsonFile)

    # Update the local IP in the JSON
    port = 80
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    local_ip = s.getsockname()[0]
    s.close()

    if port == 80:
        ip_str = str(local_ip)
    else:
        ip_str = str(local_ip) + ":" + port

    with open(host_dir + package_json_name, 'r') as file:
        data = file.read()
        data = data.replace("!!local-install-url", "http://" + ip_str)

    with open(host_dir + package_json_name, 'w') as file:
        file.write(data)

    # Host a HTTP server
    os.chdir(host_dir)
    httpd = http.server.HTTPServer(("", port), http.server.SimpleHTTPRequestHandler)
    print(f"Enter the following in your Arduino IDE's 'Additional Boards Manager URLs': http://{ip_str}/{package_json_name}")
    print("Press CTRL+C to stop the server");
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        quit(0)

if __name__ == "__main__":
    main()
