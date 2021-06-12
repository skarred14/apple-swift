add_sourcekit_library(SourceKitSupport
  Concurrency-libdispatch.cpp
  Logging.cpp
  ImmutableTextBuffer.cpp
  ThreadSafeRefCntPtr.cpp
  Tracing.cpp
  UIDRegistry.cpp)
if(SWIFT_LINK_SHARED_CLANG)
  target_link_libraries(SourceKitSupport PRIVATE clang-cpp)
else()
  target_link_libraries(SourceKitSupport PRIVATE
    clangBasic
    clangRewrite)
endif()
target_link_libraries(SourceKitSupport PRIVATE
  swiftBasic
  swiftSyntax)
if(NOT CMAKE_SYSTEM_NAME STREQUAL Darwin)
  target_link_libraries(SourceKitSupport INTERFACE
    dispatch
    BlocksRuntime)
endif()

