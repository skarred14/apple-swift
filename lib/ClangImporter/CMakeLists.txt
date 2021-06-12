set(SWIFT_GYB_FLAGS
    "-DCFDatabaseFile=${SWIFT_SOURCE_DIR}/lib/ClangImporter/CFDatabase.def")

add_gyb_target(generated_sorted_cf_database
    SortedCFDatabase.def.gyb)
if(SWIFT_LINK_SHARED_CLANG)
  set(swiftClangImporter_CLANG_LINK_COMPONENTS
      clang-cpp
      LLVM)
else()
  set(swiftClangImporter_CLANG_LINK_COMPONENTS
      LLVMBitstreamReader
      clangDependencyScanning)
endif()

add_swift_host_library(swiftClangImporter STATIC
  CFTypeInfo.cpp
  ClangAdapter.cpp
  ClangDiagnosticConsumer.cpp
  ClangImporter.cpp
  ClangModuleDependencyScanner.cpp
  ClangSourceBufferImporter.cpp
  DWARFImporter.cpp
  ImportDecl.cpp
  ImportEnumInfo.cpp
  ImportMacro.cpp
  ImportName.cpp
  ImportType.cpp
  Serializability.cpp
  SwiftLookupTable.cpp
)
target_link_libraries(swiftClangImporter PRIVATE
  swiftAST
  swiftParse
  ${swiftClangImporter_CLANG_LINK_COMPONENTS})

# This property is only set by calls to clang_tablegen. It will not be set on
# standalone builds, so it can always be safely passed.
get_property(CLANG_TABLEGEN_TARGETS GLOBAL PROPERTY CLANG_TABLEGEN_TARGETS)
add_dependencies(swiftClangImporter
  generated_sorted_cf_database
  ${CLANG_TABLEGEN_TARGETS})

set_swift_llvm_is_available(swiftClangImporter)
