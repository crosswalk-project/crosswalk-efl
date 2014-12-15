// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if !defined(XWALK_EFL)
#include "xwalk/runtime/app/xwalk_main_delegate.h"
#include "content/public/app/content_main.h"

#if defined(OS_MACOSX)
#include "xwalk/runtime/app/xwalk_content_main.h"
#endif

#if defined(OS_WIN)
#include "content/public/app/startup_helper_win.h"
#include "sandbox/win/src/sandbox_types.h"
#endif

#else // XWALK_EFL
#include "ewk/efl_integration/public/ewk_context.h"
#include "ewk/efl_integration/public/ewk_main.h"
#include "ewk/efl_integration/public/ewk_view.h"
#include "xwalk/runtime/browser/ui/native_app_window.h"
#include "xwalk/runtime/browser/ui/native_app_window_efl.h"
#endif // XWALK_EFL

#if defined(OS_WIN)
int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
#else
int main(int argc, const char** argv) {
#endif
#if defined(XWALK_EFL)
  ewk_set_arguments(argc, (char**)argv);
  xwalk::NativeAppWindow::Initialize();
  return 0;
#else
#if defined(OS_MACOSX)
    // Do the delegate work in xwalk_content_main to avoid having to export the
  // delegate types.
  return ::ContentMain(argc, argv);
#else
  xwalk::XWalkMainDelegate delegate;
  content::ContentMainParams params(&delegate);

#if defined(OS_WIN)
  sandbox::SandboxInterfaceInfo sandbox_info = {0};
  content::InitializeSandboxInfo(&sandbox_info);
  params.instance = instance;
  params.sandbox_info = &sandbox_info;
#else
  params.argc = argc;
  params.argv = argv;
#endif
  return content::ContentMain(params);
#endif
#endif // XWALK_EFL
}
