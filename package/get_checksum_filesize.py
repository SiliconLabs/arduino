import sys
import os
import hashlib

"""
Utility script for getting the file size in bytes and the SHA-256 checksum for all files in a directory
"""

def main():
    try:
        rootdir = sys.argv[1]
    except Exception:
        print("Please provide a target directory path in the first argument!")
        exit(-1)

    print(f"{'Filename':<70}{'Size (bytes)':<30}{'SHA-256 checksum':<30}")
    print("-"*165)
    print()

    for root, dirs, filenames in os.walk(rootdir):
        for filename in filenames:
            filepath = root + os.sep + filename
            stat = os.stat(filepath)
            shasum = sha256sum(filepath)
            print(f"{filename:<70}{stat.st_size:<30}{shasum:<30}")
            print("-"*165)
        break


def sha256sum(filename):
    h = hashlib.sha256()
    with open(filename, 'rb') as file:
        while True:
            chunk = file.read(h.block_size)
            if not chunk:
                break
            h.update(chunk)
    return h.hexdigest()


if __name__ == "__main__":
    main()
