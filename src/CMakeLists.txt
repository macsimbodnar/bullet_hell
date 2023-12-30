file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

add_executable(Bullet-Hell ${SRCS})

target_link_libraries(Bullet-Hell pixello)
target_include_directories(Bullet-Hell PRIVATE ../pixello/src)

# Add the binary as test so we can run it with ctest --verbose
add_test(NAME Bullet-Hell
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Bullet-Hell
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..)
