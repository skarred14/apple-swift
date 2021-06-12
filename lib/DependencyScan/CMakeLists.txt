
if(SWIFT_LINK_SHARED_CLANG)
  set(swiftDependencyScan_CLANG_LINK_COMPONENTS clang-cpp)
else()
  set(swiftDependencyScan_CLANG_LINK_COMPONENTS
      clangBasic)
endif()

add_swift_host_library(swiftDependencyScan STATIC
  DependencyScanningTool.cpp
  ModuleDependencyCacheSerialization.cpp
  ScanDependencies.cpp
  StringUtils.cpp)

target_link_libraries(swiftDependencyScan PRIVATE
    swiftClangImporter
    swiftFrontend
    ${swiftDependencyScan_CLANG_LINK_COMPONENTS})
