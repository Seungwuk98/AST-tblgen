add_executable(ast-tblgen ast-tblgen.cpp)
target_link_libraries(
  ast-tblgen 

  PUBLIC 
  ${llvm_libs}
  CXXComponents
  TableGenEmitter
)
