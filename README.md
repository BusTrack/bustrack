BusTrack
======

BusTrack is an experimental bus tracking and monitoring system. This project
is part of CS3249 User Interface Development, National University of Singapore.

It is created and developed by the following people:

 * Ko Wan Ling (Year 2, CM)
 * Kek Yan Rong (Year 3, CM)
 * Terence Then (Year 3, CS)
 * [Wong Yong Jie](http://yjwong.name) (Year 3, CM)

The project is supervised by [A/P Leow Wee Kheng](http://www.comp.nus.edu.sg/~leowwk/).

Features
--------

 * Receive bus stop crowdedness information from fixed terminals at bus stops.
 * Receive bus location and crowdedness information from mobile terminals
   installed in buses.
 * User interface for users to obtain arrival times at each bus stop.
 * Management of bus services, routes and fleet.

Downloading
-----------

To obtain the source for BusTrack, simply perform the following:

    git clone --recursive https://github.com/BusTrack/bustrack.git

Pre-requisites
--------------

BusTrack requires [Qt4](http://qt-project.org/), a working C++ compiler and
[CMake](http://cmake.org/).

For Debian-based distributions such as Ubuntu, the dependencies can be installed
easily:

    sudo apt-get install build-essential cmake libqt4-dev

Note that for Ubuntu 12.04 LTS, updated CMake packages are required. The
updated packages can be found
[on this PPA](https://launchpad.net/~yjwong/+archive/cmake). To add the PPA,
use:

    sudo add-apt-repository ppa:yjwong/cmake
    sudo apt-get update

Then install the dependencies as shown above.

Development
-----------

The project is divided into 3 parts:

 * client: the user interface for the client
 * libbustrack: common data structures used by the client and server
 * server: the daemon that services requests from client and receives bus
   information

The build system used by BusTrack is `cmake`. To build the project, simply run
`cmake` in a build directory:

    mkdir build && cd build
    cmake ..
    make -jN

Where N is the desired number of jobs to use for building.

Documentation
-------------

No user documentation is available at the moment.

