cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)
project("srmcollider_src")

# All classes that will go into the library
set(my_lib_sources
  calculate_eUIS.cpp
  combinatorics.h
  srmcolliderLib.cpp
  integratedrun.cpp
  rangetree.cpp
)

add_library(SRMCollider SHARED ${my_lib_sources})

# All executables
set(my_executables
run_integrated
)

## add targets for the executables
foreach(i ${my_executables})
  add_executable(${i} ${i}.C)
  target_link_libraries(${i} SRMCollider ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} )
endforeach(i)
