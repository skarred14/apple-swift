set_swift_llvm_is_available()
add_swift_host_library(swiftFrontendTool STATIC
  FrontendTool.cpp
  ImportedModules.cpp
  ScanDependencies.cpp
  TBD.cpp)
add_dependencies(swiftFrontendTool
  swift-syntax-generated-headers
  SwiftOptions)
if(CLANG_LINK_CLANG_DYLIB)
  set(LIBS
      clang-cpp)
else()
  set(LIBS
      clangAPINotes
      clangBasic)
endif()
target_link_libraries(swiftFrontendTool INTERFACE
                      ${LIBS})
target_link_libraries(swiftFrontendTool PRIVATE
    swiftClangImporter
    swiftDemangling
    swiftFrontend
    swiftIDE
    swiftImmediate
    swiftIndex
    swiftIRGen
    swiftOption
    swiftPrintAsObjC
    swiftSerialization
    swiftSIL
    swiftSILGen
    swiftSILOptimizer
    swiftTBDGen)
