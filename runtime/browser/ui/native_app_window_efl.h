// Copyright (c) 2015 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_RUNTIME_BROWSER_UI_NATIVE_APP_WINDOW_EFL_H_
#define XWALK_RUNTIME_BROWSER_UI_NATIVE_APP_WINDOW_EFL_H_

#include <Evas.h>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "third_party/WebKit/public/platform/WebScreenOrientationLockType.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/native_widget_types.h"
#include "xwalk/runtime/browser/ui/native_app_window.h"

namespace content {
class WebContents;
}

namespace xwalk {

// Base window class for native application.
class NativeAppWindowEfl: public NativeAppWindow {
 public:
  explicit NativeAppWindowEfl(const NativeAppWindow::CreateParams&);

  static void Initialize();
  static NativeAppWindow* Create(const CreateParams& params);

  void LockOrientation(
      blink::WebScreenOrientationLockType orientations);

  gfx::NativeWindow GetNativeWindow() const override;
  void UpdateIcon(const gfx::Image& icon) override;
  void UpdateTitle(const base::string16& title) override;
  gfx::Rect GetRestoredBounds() const override;
  gfx::Rect GetBounds() const override;
  void SetBounds(const gfx::Rect& bounds) override;

  void Focus() override;
  void Show() override;
  void Hide() override;
  void Maximize() override;
  void Minimize() override;
  void SetFullscreen(bool fullscreen) override;
  void Restore() override;
  void FlashFrame(bool flash) override;
  void Close() override;

  bool IsActive() const override;
  bool IsMaximized() const override;
  bool IsMinimized() const override;
  bool IsFullscreen() const override;

 private:
  ~NativeAppWindowEfl() override {};

  static void OnWindowDelRequest(void* data, Evas_Object*, void*);

  Evas_Object* window_;
  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(NativeAppWindowEfl);
};

#if defined(OS_TIZEN)
inline NativeAppWindowEfl* ToNativeAppWindowTizen(NativeAppWindow* window) {
  return static_cast<NativeAppWindowEfl*>(window);
}
#endif // OS_TIZEN

} // namespace xwalk

#endif // XWALK_RUNTIME_BROWSER_UI_NATIVE_APP_WINDOW_EFL_H_


