// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/runtime/browser/ui/native_app_window_efl.h"

#include "content/public/browser/notification_service.h"
#include "content/public/browser/web_contents.h"
#include "ui/gfx/screen.h"
#include "ewk_context.h"
#include "ewk_main.h"
#include "ewk_view.h"

#include "xwalk/runtime/common/xwalk_notification_types.h"
#include "xwalk/runtime/browser/xwalk_runner.h"

namespace xwalk {

NativeAppWindowEfl::NativeAppWindowEfl(
    const NativeAppWindow::CreateParams& create_params)
  : delegate_(create_params.delegate),
    web_contents_(create_params.web_contents),
    is_fullscreen_(false),
    minimum_size_(create_params.minimum_size),
    maximum_size_(create_params.maximum_size),
    resizable_(create_params.resizable) {
}

NativeAppWindowEfl::NativeAppWindowEfl()
  : delegate_(NULL),
    web_contents_(NULL) {
}

NativeAppWindowEfl::~NativeAppWindowEfl() {}

void NativeAppWindowEfl::Initialize() {
  int width = 800;
  int height = 600;

  window_ = ecore_evas_new("opengl_x11", 0, 0, width, height, 0);
  e_view_ = ecore_evas_get(window_);

  Ewk_Context* context = ewk_context_default_get();
  webview_ = ewk_view_add_with_context(e_view_, context);
  evas_object_resize(webview_, width, height);

  evas_object_show(webview_);

  xwalk::XWalkRunner * xwalk_runner_ = xwalk::XWalkRunner::GetInstance();
  xwalk_runner_->PreMainMessageLoopRun();

  ecore_evas_show(window_);
  ecore_main_loop_begin();
  ewk_context_unref(context);

  evas_object_del(webview_);
}

gfx::NativeWindow NativeAppWindowEfl::GetNativeWindow() const {
  return gfx::NativeWindow();
}

void NativeAppWindowEfl::UpdateIcon(const gfx::Image& icon) {
  icon_ = icon;
}

void NativeAppWindowEfl::UpdateTitle(const base::string16& title) {
  title_ = title;
}

gfx::Rect NativeAppWindowEfl::GetRestoredBounds() const {
  return GetBounds();
}

gfx::Rect NativeAppWindowEfl::GetBounds() const {
  int x, y, w, h;
  evas_object_geometry_get(webview_, &x, &y, &w, &h);
  return gfx::Rect(x, y, w, h);
}

void NativeAppWindowEfl::SetBounds(const gfx::Rect& bounds) {
  evas_object_move(webview_, bounds.x(), bounds.y());
  evas_object_resize(webview_, bounds.width(), bounds.height());
}

void NativeAppWindowEfl::Focus() {
  evas_object_focus_set(webview_, EINA_TRUE);
}

void NativeAppWindowEfl::Show() {
  evas_object_show(webview_);
}

void NativeAppWindowEfl::Hide() {
  evas_object_hide(webview_);
}

void NativeAppWindowEfl::Maximize() {
}

void NativeAppWindowEfl::Minimize() {
}

void NativeAppWindowEfl::SetFullscreen(bool fullscreen) {
  int width, height;
  if (is_fullscreen_ == fullscreen)
    return;
  is_fullscreen_ = fullscreen;
  ecore_x_window_size_get(ecore_x_window_root_first_get(), &width, &height);
  ecore_evas_size_base_set(window_, width, height);

  content::NotificationService::current()->Notify(
      xwalk::NOTIFICATION_FULLSCREEN_CHANGED,
      content::Source<NativeAppWindow>(this),
      content::NotificationService::NoDetails());
}

void NativeAppWindowEfl::Restore() {
}

void NativeAppWindowEfl::FlashFrame(bool flash) {
}

void NativeAppWindowEfl::Close() {
}

bool NativeAppWindowEfl::IsActive() const {
  return true;
}

bool NativeAppWindowEfl::IsMaximized() const {
  return true;
}

bool NativeAppWindowEfl::IsMinimized() const {
  return false;
}

bool NativeAppWindowEfl::IsFullscreen() const {
  return is_fullscreen_;
}

// static
NativeAppWindow* NativeAppWindow::Create(
    const NativeAppWindow::CreateParams& create_params) {
  NativeAppWindowEfl* window = new NativeAppWindowEfl(create_params);
  window->Initialize();
  return window;
}

// static
void NativeAppWindow::Initialize() {

  elm_init(0, NULL);
  if (!ewk_init()) {
    return ;
  }
  elm_init(0, NULL);

  NativeAppWindowEfl* window = new NativeAppWindowEfl();
  window->Initialize();
  ewk_shutdown();
}

}  // namespace xwalk
