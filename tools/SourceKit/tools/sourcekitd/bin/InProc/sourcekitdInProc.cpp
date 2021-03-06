//===--- sourcekitdInProc.cpp ---------------------------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#include "sourcekitd/Internal.h"

#include "SourceKit/Support/Concurrency.h"
#include "SourceKit/Support/Logging.h"
#include "SourceKit/Support/UIdent.h"

#include "llvm/ADT/SmallString.h"
#include "llvm/Support/Mutex.h"
#include "llvm/Support/Path.h"

// FIXME: Portability ?
#include <Block.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using namespace SourceKit;

static void postNotification(sourcekitd_response_t Notification);

static void getToolchainPrefixPath(llvm::SmallVectorImpl<char> &Path) {
#if defined(_WIN32)
  MEMORY_BASIC_INFORMATION mbi;
  char path[MAX_PATH + 1];
  if (!VirtualQuery(reinterpret_cast<void *>(sourcekitd_initialize), &mbi,
                    sizeof(mbi)))
    llvm_unreachable("call to VirtualQuery failed");
  if (!GetModuleFileNameA(static_cast<HINSTANCE>(mbi.AllocationBase), path,
                          MAX_PATH))
    llvm_unreachable("call to GetModuleFileNameA failed");
  auto parent = llvm::sys::path::parent_path(path);
  Path.append(parent.begin(), parent.end());
#else
  // This silly cast below avoids a C++ warning.
  Dl_info info;
  if (dladdr((void *)(uintptr_t)sourcekitd_initialize, &info) == 0)
    llvm_unreachable("Call to dladdr() failed");
  // We now have the path to the shared lib, move to the parent prefix path.
  auto parent = llvm::sys::path::parent_path(info.dli_fname);
  Path.append(parent.begin(), parent.end());
#endif

#if defined(SOURCEKIT_UNVERSIONED_FRAMEWORK_BUNDLE)
  // Path points to e.g. "usr/lib/sourcekitdInProc.framework/"
  const unsigned NestingLevel = 2;
#elif defined(SOURCEKIT_VERSIONED_FRAMEWORK_BUNDLE)
  // Path points to e.g. "usr/lib/sourcekitdInProc.framework/Versions/Current/"
  const unsigned NestingLevel = 4;
#else
  // Path points to e.g. "usr/lib/"
  const unsigned NestingLevel = 1;
#endif

  // Get it to "usr"
  for (unsigned i = 0; i < NestingLevel; ++i)
    llvm::sys::path::remove_filename(Path);
}

static std::string getRuntimeLibPath() {
  llvm::SmallString<128> libPath;
  getToolchainPrefixPath(libPath);
  llvm::sys::path::append(libPath, "lib");
  return libPath.str().str();
}

static std::string getDiagnosticDocumentationPath() {
  llvm::SmallString<128> docPath;
  getToolchainPrefixPath(docPath);
  llvm::sys::path::append(docPath, "share", "doc", "swift", "diagnostics");
  return docPath.str().str();
}

void sourcekitd_initialize(void) {
  if (sourcekitd::initializeClient()) {
    LOG_INFO_FUNC(High, "initializing");
    sourcekitd::initializeService(getRuntimeLibPath(),
                                  getDiagnosticDocumentationPath(),
                                  postNotification);
  }
}

void sourcekitd_shutdown(void) {
  if (sourcekitd::shutdownClient()) {
    LOG_INFO_FUNC(High, "shutting down");
    sourcekitd::shutdownService();
  }
}

void sourcekitd::set_interrupted_connection_handler(
                          llvm::function_ref<void()> handler) {
}

//===----------------------------------------------------------------------===//
// sourcekitd_request_sync
//===----------------------------------------------------------------------===//

sourcekitd_response_t sourcekitd_send_request_sync(sourcekitd_object_t req) {
  Semaphore sema(0);

  sourcekitd_response_t ReturnedResp;
  sourcekitd::handleRequest(req, [&](sourcekitd_response_t resp) {
    ReturnedResp = resp;
    sema.signal();
  });

  sema.wait();
  return ReturnedResp;
}

void sourcekitd_send_request(sourcekitd_object_t req,
                             sourcekitd_request_handle_t *out_handle,
                             sourcekitd_response_receiver_t receiver) {
  // FIXME: Implement request handle.

  sourcekitd_request_retain(req);
  receiver = Block_copy(receiver);
  WorkQueue::dispatchConcurrent([=]{
    sourcekitd::handleRequest(req, [=](sourcekitd_response_t resp) {
      // The receiver accepts ownership of the response.
      receiver(resp);
      Block_release(receiver);
    });
    sourcekitd_request_release(req);
  });
}

void sourcekitd_cancel_request(sourcekitd_request_handle_t handle) {
  // FIXME: Implement cancelling.
}

void
sourcekitd_set_interrupted_connection_handler(
                          sourcekitd_interrupted_connection_handler_t handler) {
  // This is only meaningful in an IPC implementation.
}

static sourcekitd_response_receiver_t NotificationReceiver;

void
sourcekitd_set_notification_handler(sourcekitd_response_receiver_t receiver) {
  sourcekitd_response_receiver_t newReceiver = Block_copy(receiver);
  WorkQueue::dispatchOnMain([=]{
    Block_release(NotificationReceiver);
    NotificationReceiver = newReceiver;
  });
}

void postNotification(sourcekitd_response_t Notification) {
  WorkQueue::dispatchOnMain([=]{
    if (!NotificationReceiver) {
      sourcekitd_response_dispose(Notification);
      return;
    }
    // The receiver accepts ownership of the notification object.
    NotificationReceiver(Notification);
  });
}
