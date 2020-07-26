add_swift_host_tool(swift-llvm-opt
  LLVMOpt.cpp
  SWIFT_COMPONENT tools
)
if(LLVM_LINK_LLVM_DYLIB)
  set(LIBS
      LLVM)
else()
  set(LIBS
      LLVMAggressiveInstCombine
      LLVMCodeGen
      LLVMInstCombine
      LLVMScalarOpts
      LLVMVectorize
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
target_link_libraries(swift-llvm-opt
                      PRIVATE
		        ${LIBS}
                        swiftIRGen
                        # Swift libraries included to appease the linker on linux.
                        swiftSema
                        swiftAST
                        # Clang libraries included to appease the linker on linux.
                        # LLVM libraries included to appease the linker on linux.
                        )
