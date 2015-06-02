// Copyright (c) 2015 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/runtime/browser/ui/native_app_window_efl.h"

#include <Elementary.h>

#include "base/command_line.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/browser/web_contents.h"
#include "efl/window_factory.h"
#include "ui/gfx/screen_efl.h"
#include "ui/gfx/geometry/rect.h"

namespace xwalk {

namespace {
const int kDefaultTestWindowWidthDip = 800;
const int kDefaultTestWindowHeightDip = 600;

const char kXwalkHostWindowBounds[] = "xwalk-host-window-bounds";
}

NativeAppWindowEfl::NativeAppWindowEfl(
    const NativeAppWindow::CreateParams& params)
        : window_(NULL)
        , web_contents_(params.web_contents) {
  window_ = efl::WindowFactory::GetHostWindow(web_contents_);
  DCHECK(window_);

  gfx::Size size;
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(kXwalkHostWindowBounds)) {
    const std::string size_str =
        command_line->GetSwitchValueASCII(kXwalkHostWindowBounds);
    int width, height;
    if (sscanf(size_str.c_str(), "%d,%d", &width, &height) == 2)
      size = gfx::Size(width, height);
  }

  if (size.IsEmpty())
    size = gfx::Size(kDefaultTestWindowWidthDip, kDefaultTestWindowHeightDip);

  evas_object_smart_callback_add(window_, "delete,request",
      OnWindowDelRequest, this);

  evas_object_resize(window_, size.width(), size.height());

  Evas_Object *box = elm_box_add(window_);
  evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_win_resize_object_add(window_, box);
  evas_object_show(box);

  Evas_Object* view = static_cast<Evas_Object*>(web_contents_->GetNativeView());
  evas_object_size_hint_align_set(view, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set(view, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_box_pack_end(box, view);

  web_contents_->Focus();
}

void NativeAppWindowEfl::LockOrientation(
    blink::WebScreenOrientationLockType lock) {
  NOTIMPLEMENTED();
}

gfx::NativeWindow NativeAppWindowEfl::GetNativeWindow() const {
  return window_;
}

void NativeAppWindowEfl::UpdateIcon(const gfx::Image& icon) {
  NOTIMPLEMENTED();
}

void NativeAppWindowEfl::UpdateTitle(const base::string16& title) {
  elm_win_title_set(window_, UTF16ToUTF8(title).c_str());
}

gfx::Rect NativeAppWindowEfl::GetRestoredBounds() const {
  NOTIMPLEMENTED();
  return gfx::Rect();
}

gfx::Rect NativeAppWindowEfl::GetBounds() const {
  int x, y, w, h;
  evas_object_geometry_get(window_, &x, &y, &w, &h);
  return gfx::Rect(x, y, w, h);
}

void NativeAppWindowEfl::SetBounds(const gfx::Rect& bounds) {
  evas_object_geometry_set(window_, bounds.x(), bounds.y(),
      bounds.width(), bounds.height());
}

void NativeAppWindowEfl::Focus() {
  NOTIMPLEMENTED();
}

void NativeAppWindowEfl::Show() {
  evas_object_show(window_);
}

void NativeAppWindowEfl::Hide() {
  evas_object_hide(window_);
}

void NativeAppWindowEfl::Maximize() {
  elm_win_maximized_set(window_, EINA_TRUE);
}

void NativeAppWindowEfl::Minimize() {
  elm_win_maximized_set(window_, EINA_FALSE);
}

void NativeAppWindowEfl::SetFullscreen(bool fullscreen) {
  elm_win_fullscreen_set(window_, fullscreen);
}

void NativeAppWindowEfl::Restore() {
  NOTIMPLEMENTED();
}

void NativeAppWindowEfl::FlashFrame(bool flash) {
  NOTIMPLEMENTED();
}

void NativeAppWindowEfl::Close() {
  if (window_)
    evas_object_del(window_);
  window_ = NULL;
}

bool NativeAppWindowEfl::IsActive() const {
  return true;
}

bool NativeAppWindowEfl::IsMaximized() const {
  return elm_win_maximized_get(window_);
}

bool NativeAppWindowEfl::IsMinimized() const {
  return !elm_win_maximized_get(window_);
}

bool NativeAppWindowEfl::IsFullscreen() const {
  return elm_win_fullscreen_get(window_);
}

// static
void NativeAppWindowEfl::OnWindowDelRequest(
    void* data, Evas_Object*, void*) {
  NativeAppWindowEfl* thiz = static_cast<NativeAppWindowEfl*>(data);
  if (!elm_win_autodel_get(thiz->window_))
    thiz->Close();
  thiz->window_ = NULL;
  thiz->web_contents_->Close();
}

// static
NativeAppWindow* NativeAppWindow::Create(
    const NativeAppWindow::CreateParams& create_params) {
  return new NativeAppWindowEfl(create_params);
}

// static
void NativeAppWindow::Initialize() {
  ui::InstallScreenInstance();
}
}

