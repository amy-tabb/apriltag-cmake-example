# apriltag-cmake-example
A MWE of using the apriltag library with cmake to generate makefiles.

I had a very hard time getting the apriltag library to work with cmake.  I use cmake all the time, so decided to put together a little minimal working example.  I'm being sort of brief, but if you have questions, just open an issue.

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






First, make sure the package-config path is correct:

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/apriltag/lib/pkgconfig


sudo to make this stick.


- `cmake ../src`  (basic)
- `cmake  -DCMAKE_BUILD_TYPE=Release ../src` (Release configuration)
- `cmake  -DCMAKE_BUILD_TYPE=Debug ../src` (Debug configuration)
- `cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../src/` (Create an Eclipse project to import -- it will be in the build folder)

In case you have installed OpenCV and cmake can't find it, you need to specify the location of `OpenCVConfig.cmake`.  Don't know where it is?   Find out with `locate OpenCVConfig.cmake`.  Then append

`-DCMAKE_PREFIX_PATH=dir`

https://github.com/AprilRobotics/apriltag_ros/blob/master/apriltag_ros/CMakeLists.txt
