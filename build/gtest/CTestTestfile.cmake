# CMake generated Testfile for 
# Source directory: D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest
# Build directory: D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/build/gtest
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(ExampleTest "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/build/gtest/Debug/ExampleTest.exe")
  set_tests_properties(ExampleTest PROPERTIES  _BACKTRACE_TRIPLES "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;15;add_test;D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(ExampleTest "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/build/gtest/Release/ExampleTest.exe")
  set_tests_properties(ExampleTest PROPERTIES  _BACKTRACE_TRIPLES "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;15;add_test;D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(ExampleTest "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/build/gtest/MinSizeRel/ExampleTest.exe")
  set_tests_properties(ExampleTest PROPERTIES  _BACKTRACE_TRIPLES "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;15;add_test;D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(ExampleTest "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/build/gtest/RelWithDebInfo/ExampleTest.exe")
  set_tests_properties(ExampleTest PROPERTIES  _BACKTRACE_TRIPLES "D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;15;add_test;D:/All/Studia/Zajecia/Semestr_7/Inzynierka/TEST/TSP-MAPD/gtest/CMakeLists.txt;0;")
else()
  add_test(ExampleTest NOT_AVAILABLE)
endif()
