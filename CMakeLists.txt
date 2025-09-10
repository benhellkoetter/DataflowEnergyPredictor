cmake_minimum_required(VERSION 3.10)
project(DataflowEnergyPredictor LANGUAGES C CXX)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM 15 REQUIRED CONFIG COMPONENTS core irreader analysis)
message(STATUS "Found: LLVM ${LLVM_PACKAGE_VERSION}")

find_package(phasar COMPONENTS llvm_ifdside REQUIRED)
message(STATUS "Found: Phasar ${Phasar_PACKAGE_VERSION}")

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/EnergyOfValueAnalysis.cpp
  src/EnergyEdgeFunctions.cpp
  src/EnergyModel.cpp
  src/ProfileLoader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  ${LLVM_INCLUDE_DIRS}
)

llvm_map_components_to_libnames(LLVM_LIBS analysis)

target_link_libraries(${PROJECT_NAME} PUBLIC
  phasar::llvm_ifdside
  ${LLVM_LIBS}
)