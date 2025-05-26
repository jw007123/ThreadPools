add_executable(Tests "Main.cpp")

target_include_directories (Lib_Pool PUBLIC "${CMAKE_SOURCE_DIR}/Include")
target_include_directories (Lib_Pool PUBLIC "${CMAKE_SOURCE_DIR}/External/eigen")
target_include_directories (Lib_Pool PUBLIC "${CMAKE_SOURCE_DIR}/External/tracy/public")

target_link_libraries(Tests PUBLIC TracyClient)
target_link_libraries(Tests PUBLIC Lib_Pool)