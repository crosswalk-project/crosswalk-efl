// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/runtime/browser/xwalk_runner_impl.h"

#include <string>
#include <vector>
#include "base/command_line.h"
#include "base/logging.h"
#include "content/public/browser/render_process_host.h"
#include "xwalk/application/browser/application.h"
#include "xwalk/application/browser/application_service.h"
#include "xwalk/application/browser/application_system.h"
#include "xwalk/extensions/browser/xwalk_extension_service.h"
#include "xwalk/extensions/common/xwalk_extension_switches.h"
#include "xwalk/runtime/browser/application_component.h"
#include "xwalk/runtime/browser/devtools/remote_debugging_server.h"
#include "xwalk/runtime/browser/xwalk_browser_context.h"
#include "xwalk/runtime/browser/storage_component.h"
#include "xwalk/runtime/browser/sysapps_component.h"
#include "xwalk/runtime/browser/xwalk_app_extension_bridge.h"
#include "xwalk/runtime/browser/xwalk_browser_main_parts.h"
#include "xwalk/runtime/browser/xwalk_component.h"
#include "xwalk/runtime/browser/xwalk_content_browser_client.h"
#include "xwalk/runtime/common/xwalk_runtime_features.h"
#include "xwalk/runtime/common/xwalk_switches.h"

#if defined(OS_ANDROID)
#include "xwalk/runtime/browser/xwalk_runner_android.h"
#elif defined(OS_TIZEN)
#include "xwalk/runtime/browser/xwalk_runner_tizen.h"
#endif

namespace xwalk {

namespace {

XWalkRunnerImpl* g_xwalk_runner = NULL;

}  // namespace

XWalkRunnerImpl::XWalkRunnerImpl() {
  VLOG(1) << "Creating XWalkRunnerImpl object.";
  DCHECK(!g_xwalk_runner);
  g_xwalk_runner = this;

  XWalkRuntimeFeatures::GetInstance()->Initialize(
      CommandLine::ForCurrentProcess());

  // Initializing after the g_xwalk_runner is set to ensure
  // XWalkRunnerImpl::GetInstance() can be used in all sub objects if needed.
  content_browser_client_.reset(new XWalkContentBrowserClient(this));
}

XWalkRunnerImpl::~XWalkRunnerImpl() {
  DCHECK(g_xwalk_runner);
  g_xwalk_runner = NULL;
  VLOG(1) << "Destroying XWalkRunnerImpl object.";
}

// static
XWalkRunnerImpl* XWalkRunnerImpl::GetInstance() {
  return g_xwalk_runner;
}

// static
XWalkRunner* XWalkRunner::GetInstance() {
  return g_xwalk_runner;
}

XWalkBrowserContext* XWalkRunnerImpl::browser_context() {
  return runtime_context_.get();
}

void XWalkRunnerImpl::PreMainMessageLoopRun() {
  runtime_context_.reset(new XWalkBrowserContext);
  XWalkRunner::PreMainMessageLoopRun();
}

void XWalkRunnerImpl::PostMainMessageLoopRun() {
  runtime_context_.reset();
}

// static
scoped_ptr<XWalkRunner> XWalkRunner::Create() {
  XWalkRunnerImpl* runner = NULL;
#if defined(OS_ANDROID)
  runner = new XWalkRunnerAndroid;
#elif defined(OS_TIZEN)
  runner = new XWalkRunnerTizen;
#else
  runner = new XWalkRunnerImpl;
#endif
  return scoped_ptr<XWalkRunner>(runner);
}

content::ContentBrowserClient* XWalkRunnerImpl::GetContentBrowserClient() {
  return content_browser_client_.get();
}

}  // namespace xwalk
