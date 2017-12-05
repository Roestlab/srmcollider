
cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)
project("srmcollider_tests")

# All executables 
set(my_test_executables
testPrecursor
testgetNonUIS
testIntegrated
testRangetree
testeUIS
testLibrary
testCombinatorics
)

## add targets for the executables
foreach(i ${my_test_executables})
  add_executable(${i} ${i}.C)
  target_link_libraries(${i} SRMCollider ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} )
endforeach(i)

ADD_TEST(testgetNonUIS testgetNonUIS)
ADD_TEST(testPrecursor testPrecursor)
ADD_TEST(testIntegrated testIntegrated)
ADD_TEST(testRangetree testRangetree)
ADD_TEST(testeUIS testeUIS)
ADD_TEST(testLibrary testLibrary)
ADD_TEST(testCombinatorics testCombinatorics)

