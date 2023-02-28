#!/bin/env python

import subprocess, os

subprocess.run("mkdir build", shell=True)
build_path = os.path.join(os.getcwd(), "build")
os.chdir(build_path)

subprocess.run("cmake ..", shell=True)
subprocess.run("make", shell=True)
