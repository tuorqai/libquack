
set(EXECUTABLES
    hello
    keyboard
    primitives
    textures
    utf8-title
    sounds
    blend-modes
    music)

foreach(EXE ${EXECUTABLES})
    add_executable(${EXE} ${EXE}.c)
    target_link_libraries(${EXE} PRIVATE libquack)
    install(TARGETS ${EXE} RUNTIME DESTINATION ${QU_TEST_PATH})
endforeach()

install(DIRECTORY assets DESTINATION ${QU_TEST_PATH})
