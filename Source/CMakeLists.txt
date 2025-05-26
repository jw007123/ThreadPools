add_library(Lib_Pool Locking.cpp
				   	 Semaphore.cpp
				     Futex.cpp) 

target_include_directories (Lib_Pool PUBLIC "${CMAKE_SOURCE_DIR}/Include")
target_include_directories (Lib_Pool PUBLIC "${CMAKE_SOURCE_DIR}/External/eigen")
target_include_directories (Lib_Pool PUBLIC "${CMAKE_SOURCE_DIR}/External/tracy/public")