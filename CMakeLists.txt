cmake_minimum_required(VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(spacy
  GIT_REPOSITORY https://github.com/kiddersmxj/spacy
  GIT_TAG master
)
 
FetchContent_GetProperties(spacy)
if(NOT spacy_POPULATED)
  FetchContent_Populate(spacy)
  add_subdirectory(${spacy_SOURCE_DIR} ${spacy_BINARY_DIR})
endif()
 
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)

project(assimilate
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(assimilate src/assimilate.cpp)

find_package(std-k REQUIRED)

target_include_directories(assimilate
    PRIVATE inc
    SHARED ${spacy_SOURCE_DIR}/inc
)

target_sources(assimilate
    PRIVATE src/child.cpp
    PRIVATE src/understanding.cpp
    PRIVATE src/execute.cpp
    PRIVATE src/commands.cpp
)

install(
    TARGETS assimilate
    DESTINATION /usr/local/bin/
)

link_directories(${spacy_BINARY_DIR})
target_link_libraries(assimilate
    PUBLIC std-k
    PUBLIC spacy
    PRIVATE nlohmann_json::nlohmann_json
)

target_compile_features(assimilate PUBLIC cxx_std_20)


# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

