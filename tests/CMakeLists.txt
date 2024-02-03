
set(EXECUTABLES
    hello
    keyboard)

foreach(EXE ${EXECUTABLES})
    add_executable(${EXE} ${EXE}.c)
    target_link_libraries(${EXE} PRIVATE libquack)
endforeach()
