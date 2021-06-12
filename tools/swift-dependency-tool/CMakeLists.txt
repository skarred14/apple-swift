add_swift_host_tool(swift-dependency-tool
  swift-dependency-tool.cpp
  SWIFT_COMPONENT tools
)
if(SWIFT_LINK_SHARED_CLANG)
  target_link_libraries(swift-dependency-tool
                        PRIVATE
			clang-cpp)
else()
  target_link_libraries(swift-dependency-tool
                        PRIVATE
                        clangTooling)
endif()
target_link_libraries(swift-dependency-tool
                      PRIVATE
                        swiftAST
                        swiftParse
                        swiftClangImporter)

