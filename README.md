# apriltag-cmake-example
A MWE of using the apriltag library with cmake to generate makefiles.

I had a very hard time getting the apriltag library to work with cmake.  I use cmake all the time, so decided to put together a little minimal working example.  I'm being sort of brief, but if you have questions, just open an issue.

Contents:
- a working CMakeLists.txt.
- example `opencv_demo.cc` from the apriltag `example` directory.
- example `apriltag_demo.c` from the apriltag `example` directory.
- these instructions. 
- I also added `opencv-demo-read.**`, so you can read from a directory, or use a camera.

## Install apriltag
Install the [apriltag lib](https://github.com/AprilRobotics/apriltag.git). Set the install path.  I do this by:

````bash
cmake .
cmake-gui . 
````
 
the first `cmake` gets everything set up with the defaults, then in the `cmake-gui` step, you can easily alter the install path without remembering flags.  I'll use `/usr/local/apriltag` are the install path.  Then push the `configure` and `generate` buttons.  Done!

Then ... 

````bash
make
sudo make install
````

## clone this repo

````bash
git clone https://github.com/amy-tabb/apriltag-cmake-example.git

````

then 

````bash
cd apriltag-cmake-example
mkdir build
cd build
````

## Make sure pkg-config can find apriltag
Here, you need to do some searching we're going to use [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) to do all of the work of setting the compiler and linker options.  Trouble is, sometimes the apriltag pkg-config file is not on the `PKG_CONFIG_PATH`. 

First, what's your path?

````bash
 echo $PKG_CONFIG_PATH
````

See anything with apriltag in there?  If not, you will need to add the folder with `apriltag.pc`:

````bash
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/apriltag/lib/pkgconfig
````

to make this stick across bash sessions, put an `export PKG_CONFIG_PATH=` in `~/.profile` (more commentary on setting environment variables in Ubuntu [here](https://help.ubuntu.com/community/EnvironmentVariables)), but use the output of  `echo $PKG_CONFIG_PATH`.  Otherwise, the `PKG_CONFIG_PATH=$PKG_CONFIG_PATH` will result in a longer and longer string.  (Note, I have never had to do this will other libraries.)


## cross your fingers and try cmake  

Now you can try to generate a Makefile with cmake. I am totally skipping OpenCV, but I am assuming that you have OpenCV installed.  If you don't, no worries.  Comment out all of the OpenCV stuff in the CMakeLists.txt file using a #:

- find_package(OpenCV REQUIRED)
- (all 3 lines) add_executable(opencv_demo
opencv_demo.cc
)
- target_link_libraries(opencv_demo ${apriltag_LIBRARIES} ${OpenCV_LIBS})

The OpenCV demo will not be built, but the apriltag one will.  

Then, there are a few different options for cmake, 

- `cmake ../src`  (basic)
- `cmake  -DCMAKE_BUILD_TYPE=Release ../src` (Release configuration)
- `cmake  -DCMAKE_BUILD_TYPE=Debug ../src` (Debug configuration)
- `cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../src/` (Create an Eclipse project to import -- it will be in the build folder)

In case you have installed OpenCV and cmake can't find it, you need to specify the location of `OpenCVConfig.cmake`.  Don't know where it is?   Find out with `locate OpenCVConfig.cmake`.  Then append

`-DCMAKE_PREFIX_PATH=dir`

to the command line.  Alternatively, you could use the `cmake-gui` option I used earlier.

## Make and test

Then make and test, 

````bash
make
````

you should have two files (if you used the OpenCV option too), opencv_demo and apriltag_demo.

run as usual, `./apriltag_demo`.

The new demo is `opencv-demo-read`.  Run that one with the help by `./opencv-demo-read --help`.

## Other resources

My source for this working `CMakeLists.txt` was this other [`CMakeLists.txt`](https://github.com/AprilRobotics/apriltag_ros/blob/master/apriltag_ros/CMakeLists.txt) from the `apriltag_ros` project. FYI if you have a more complex project. 

