cmake_minimum_required(VERSION 3.10)  # Adjust the version as needed
project(ServerProject)  # Set the project name

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Define the source files
set(SOURCES
    src/file.c
    src/linkedlist.c
    src/mime.c
    src/hashtable.c
    src/cache.c
    src/net.c
    src/server.c
)

# Create an executable target
add_executable(server ${SOURCES})

# Link against the Ws2_32 library (Windows specific)
if(WIN32)
    target_link_libraries(server Ws2_32)
endif()
