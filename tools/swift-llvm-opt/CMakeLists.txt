add_swift_host_tool(swift-llvm-opt
  LLVMOpt.cpp
  SWIFT_COMPONENT tools
)
target_link_libraries(swift-llvm-opt
                      PRIVATE
                       clangTooling
                       LLVMRemarks
                        swiftIRGen
                        # Swift libraries included to appease the linker on linux.
                        swiftSema
                        swiftAST
                        # Clang libraries included to appease the linker on linux.
                        clangBasic
                        clangCodeGen
                        # LLVM libraries included to appease the linker on linux.
                        LLVMAggressiveInstCombine
                        LLVMCodeGen
                        LLVMInstCombine
                        LLVMScalarOpts
                        LLVMVectorize
                        )
