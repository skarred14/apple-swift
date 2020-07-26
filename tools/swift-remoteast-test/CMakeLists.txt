add_swift_host_tool(swift-remoteast-test
  swift-remoteast-test.cpp
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
      clangTooling)
endif()
target_link_libraries(swift-remoteast-test
                      PRIVATE
		        ${LIBS}
                        swiftFrontendTool
                        swiftRemoteAST)
set_target_properties(swift-remoteast-test PROPERTIES ENABLE_EXPORTS 1)

# If building as part of clang, make sure the headers are installed.
if(NOT SWIFT_BUILT_STANDALONE)
  add_dependencies(swift-frontend clang-resource-headers)
endif()

