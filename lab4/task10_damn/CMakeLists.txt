cmake_minimum_required(VERSION 3.10)

project(MyProject)

add_executable(main 
  main.c 
  operation_syntax.c 
  operation_type.c 
  errors.c 
  interpreter.c 
  Trie.c 
  settings_work.c
  debug.c
)

target_link_libraries(main m)