#!/usr/bin/python3
# coding: utf-8

# Builds extensions module API docs
#
# Requires Python 3.4+
# Run with --help for usage
#
# This is adapted from Voxel Tools' doc tooling: it keeps the Godot doctool
# step (-d) and the XML-to-Markdown step (-a), but drops the mkdocs site
# generation.

import sys

if sys.version_info < (3, 4):
    print("Please upgrade python to version 3.4 or higher.")
    print("Your version: %s\n" % sys.version_info)
    sys.exit(1)

import xml_to_markdown
import subprocess
import getopt
import os
import platform
from pathlib import Path

# Directory that contains the compiled Godot editor with this module enabled.
# Overridable via -g.
DEFAULT_ENGINE_BINDIR = Path(
    "D:/Code/Godot/sources_stable/godot-4.7.2-stable/bin")


def update_classes_xml(custom_godot_path, godot_repo_root, verbose=False):
    godot_executable = custom_godot_path
    if godot_executable is None or godot_executable == "":
        godot_executable = find_godot(DEFAULT_ENGINE_BINDIR)
        if godot_executable is None:
            print("Godot executable not found")
            return

    if verbose:
        print("Found Godot at: %s" % godot_executable)

    # Dump XML files from Godot
    args = [str(godot_executable), ' --doctool ', str(godot_repo_root)]
    if verbose:
        print("Running: ", args)
    result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            universal_newlines=True)
    if verbose:
        print(result.stdout)
        print("Disregard Godot's errors about files unless they are about extensions classes.")


def find_godot(bindir):  # bindir: Path
    prefix = "godot"
    os_prefix = ""
    suffix = ""
    if sys.platform == "win32" or sys.platform == "cygwin":
        os_prefix = ".windows"
        suffix = ".exe"
    elif sys.platform == "darwin":
        os_prefix = ".macos"
    else:
        os_prefix = ".linuxbsd"

    arch = ".x86_64"
    if platform.machine().lower() == "arm64":
        arch = ".arm64"
    if platform.machine().lower() == "riscv64":
        arch = ".rv64"

    # Names to try
    names = [
        prefix + os_prefix + ".editor.dev" + arch + suffix,
        prefix + os_prefix + ".editor" + arch + suffix,
        prefix + os_prefix + ".editor.dev.double" + arch + suffix,
    ]

    binaries = []
    for name in names:
        path = bindir / name
        if path.is_file():
            mtime = os.path.getmtime(path)
            binaries.append((path, mtime))

    if len(binaries) == 0:
        print("Error: Godot binary not specified and none suitable found in %s" % bindir)
        return None

    binaries = sorted(binaries, key=lambda tup: tup[1])
    return binaries[-1][0]


def print_usage():
    print("\nUsage: ", sys.argv[0], "[-d] [-a] [-h] [-v] [-g path_to_godot]")
    print()
    print("\t-d -> Execute Godot doctool to update XML class data")
    print("\t-a -> Update Markdown API files from XML class data")
    print("\t-h, --help -> Prints help")
    print("\t-v -> Verbose. Print more details when running.")
    print("\t-g -> Specify custom path to Godot. Otherwise will use a compiled executable under the engine's bin directory.")
    print()


###########################
# Main()

def main():
    # Default paths are determined from the location of this script
    my_path = Path(os.path.realpath(__file__))

    # Default parameters
    verbose = False
    must_run_doctool = False
    must_update_md_from_xml = False
    godot_executable = ""

    # Root that contains the extensions module source tree; used by doctool.
    godot_repo_root = my_path.parents[4]
    # Markdown output folder.
    md_path = my_path.parents[1] / 'source' / 'api'
    # Folder where doctool writes the class XML files.
    xml_path = my_path.parents[1] / 'classes'

    # Parse command line arguments
    try:
        opts, args = getopt.getopt(sys.argv[1:], "dahvg:", "help")
    except getopt.error as msg:
        print("Error: ", msg)
        print_usage()
        return

    did_something = False

    for opt, arg in opts:
        if opt == '-d':
            must_run_doctool = True
        if opt == '-a':
            must_update_md_from_xml = True
        if opt in ('-h', '--help'):
            print_usage()
            did_something = True
        if opt == '-v':
            verbose = True
        if opt == '-g':
            godot_executable = arg

    if must_run_doctool:
        update_classes_xml(godot_executable, godot_repo_root, verbose)
        did_something = True

    if must_update_md_from_xml:
        xml_to_markdown.process_xml_folder(xml_path, md_path, verbose)
        did_something = True

    if not did_something:
        print("No operation specified.")
        print_usage()


# If called from command line
if __name__ == "__main__":
    main()