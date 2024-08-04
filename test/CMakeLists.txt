add_executable(UnitTest PrintProgramTests.cpp)

target_link_libraries(UnitTest 
    PUBLIC 
    doctest 
    ${llvm_libs}
    CXXComponents
)
