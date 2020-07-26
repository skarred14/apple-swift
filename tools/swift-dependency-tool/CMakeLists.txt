add_swift_host_tool(swift-dependency-tool
  swift-dependency-tool.cpp
  SWIFT_COMPONENT tools
)
if(CLANG_LINK_CLANG_DYLIB)
  set(LIBS
      clang-cpp)
else()
  set(LIBS
      clangTooling)
endif()
target_link_libraries(swift-dependency-tool
                      PRIVATE
		        ${LIBS}
                        swiftAST
                        swiftParse
                        swiftClangImporter)

