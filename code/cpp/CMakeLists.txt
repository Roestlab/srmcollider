project("SRMCollider")
cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Release)



# CGAL is used header-only, no need for the libraries
find_package(CGAL REQUIRED)
message("Include dirs of CGAL: " ${CGAL_INCLUDE_DIRS} )
# message("Libs of CGAL: " ${CGAL_LIBRARIES} )

FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
message("Include dirs of Python: " ${PYTHON_INCLUDE_DIRS} )
message("Libs of Python: " ${PYTHON_LIBRARIES} )

find_package(Boost COMPONENTS filesystem system unit_test_framework python REQUIRED)
message("Include dirs of boost: " ${Boost_INCLUDE_DIRS} )
message("Libs of boost: " ${Boost_LIBRARIES} )

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${CGAL_INCLUDE_DIRS}
  "./src/"
  "./"
)

# cgal requires the -frounding-math option
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math -Wall") 

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------
add_subdirectory(src)

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------
ENABLE_TESTING()
add_subdirectory(test)


