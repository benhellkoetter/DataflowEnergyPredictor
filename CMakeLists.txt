cmake_minimum_required(VERSION 3.10)
project(DataflowEnergyPredictor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(LLVM REQUIRED CONFIG) # LLVM wird von Phasar benötigt und für deine Analyse
message(STATUS "Docker-Toolchain: Gefundenes LLVM: Version ${LLVM_PACKAGE_VERSION}")
message(STATUS "Docker-Toolchain: Nutze LLVMConfig.cmake von: ${LLVM_CMAKE_DIR}")

find_package(phasar COMPONENTS llvm_ifdside REQUIRED)

message(STATUS "Docker-Toolchain: Gefundenes Phasar: Version ${Phasar_PACKAGE_VERSION}")
message(STATUS "Docker-Toolchain: Nutze PhasarConfig.cmake von: ${Phasar_CMAKE_DIR}")

add_executable(${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        ${LLVM_INCLUDE_DIRS}
        ${Phasar_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Phasar::llvm_ifdside
)