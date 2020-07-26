add_swift_host_tool(sil-passpipeline-dumper
  SILPassPipelineDumper.cpp
  SWIFT_COMPONENT tools
)
if(LLVM_LINK_LLVM_DYLIB)
  set(LIBS
      LLVM)
else()
  set(LIBS
      LLVMRemarks)
endif()
if(CLANG_LINK_CLANG_DYLIB)
  set(LIBS
      ${LIBS}
      clang-cpp)
else()
  set(LIBS
      ${LIBS}
      clangBasic
      clangCodeGen
      clangTooling)
endif()
target_link_libraries(sil-passpipeline-dumper PRIVATE
  ${LIBS}
  swiftFrontend
  swiftIRGen
  swiftSILGen
  swiftSILOptimizer
  # Clang libraries included to appease the linker on linux.
)
