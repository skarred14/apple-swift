add_sourcekit_library(SourceKitSupport
  Concurrency-libdispatch.cpp
  Logging.cpp
  ImmutableTextBuffer.cpp
  ThreadSafeRefCntPtr.cpp
  Tracing.cpp
  UIDRegistry.cpp)
if(CLANG_LINK_CLANG_DYLIB)
  set(LIBS
      clang-cpp)
else()
  set(LIBS
      clangBasic
      clangRewrite)
endif()
target_link_libraries(SourceKitSupport PRIVATE
  swiftBasic
  swiftSyntax
  ${LIBS})
if(NOT CMAKE_SYSTEM_NAME STREQUAL Darwin)
  target_link_libraries(SourceKitSupport INTERFACE
    dispatch
    BlocksRuntime)
endif()

