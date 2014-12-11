// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_RUNTIME_BROWSER_UI_NATIVE_APP_WINDOW_EFL_H_
#define XWALK_RUNTIME_BROWSER_UI_NATIVE_APP_WINDOW_EFL_H_

#include <string>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Ecore_File.h>
#include <Ecore_Getopt.h>
#include <Ecore_X.h>
#include <Evas.h>
#include <Elementary.h>

#include "xwalk/runtime/browser/ui/native_app_window.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/rect.h"

namespace xwalk {

class TopViewLayout;

class NativeAppWindowEfl : public NativeAppWindow
{
 public:
  explicit NativeAppWindowEfl(const NativeAppWindow::CreateParams& params);
  NativeAppWindowEfl();
  virtual ~NativeAppWindowEfl();

  virtual void Initialize();

  // NativeAppWindow implementation.
  virtual gfx::NativeWindow GetNativeWindow() const OVERRIDE;
  virtual void UpdateIcon(const gfx::Image& icon) OVERRIDE;
  virtual void UpdateTitle(const base::string16& title) OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual void SetFullscreen(bool fullscreen) OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual void FlashFrame(bool flash) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual bool IsActive() const OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;

 private:

  NativeAppWindowDelegate* delegate_;
  content::WebContents* web_contents_;

  Ecore_Evas * window_;
  Evas * e_view_;
  Evas_Object * webview_;

  base::string16 title_;
  gfx::Image icon_;

  bool is_fullscreen_;
  gfx::Size minimum_size_;
  gfx::Size maximum_size_;
  bool resizable_;

  DISALLOW_COPY_AND_ASSIGN(NativeAppWindowEfl);
};

}  // namespace xwalk

#endif  // XWALK_RUNTIME_BROWSER_UI_NATIVE_APP_WINDOW_EFL_H_
