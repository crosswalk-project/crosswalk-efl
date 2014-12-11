// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_RUNTIME_BROWSER_XWALK_RUNNER_IMPL_H_
#define XWALK_RUNTIME_BROWSER_XWALK_RUNNER_IMPL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/values.h"

#include "xwalk/runtime/browser/storage_component.h"
#include "xwalk/runtime/browser/xwalk_runner.h"

namespace content {
class ContentBrowserClient;
class RenderProcessHost;
}

class XWalkTestSuiteInitializer;

namespace xwalk {

class ApplicationComponent;
class RemoteDebuggingServer;
class SysAppsComponent;
class XWalkBrowserContext;
class XWalkComponent;
class XWalkContentBrowserClient;
class XWalkAppExtensionBridge;

namespace application {
class Application;
class ApplicationSystem;
}

namespace extensions {
class XWalkExtensionService;
};

// Main object for the Browser Process execution in Crosswalk. It is created and
// owned by XWalkMainDelegate. It's role is to own, setup and teardown all the
// subsystems of Crosswalk.
class XWalkRunnerImpl : public XWalkRunner {
 public:
  // Read the comments below before using this. Relying too much on this
  // accessor makes the code harder to change and harder to reason about.
  static XWalkRunnerImpl* GetInstance();

  virtual ~XWalkRunnerImpl();

  // All sub objects should have their dependencies passed during their
  // initialization, so that these accessors below are not frequently accessed.
  // Instead of calling these, consider explicitly passing the dependencies
  // to the objects that need them.
  //
  // For example, if "Application System" needs to use "Runtime Context", we
  // should pass the "Runtime Context" to "Application System" instead of
  // making "Application System" ask XWalkRunnerImpl for its dependency.
  //
  // Scenarios when it is fine to use the accessors:
  //
  // - Prototyping solutions, in which we want to see the solution working, and
  //   all dependencies are still not clear. It avoids writing down a lot of
  //   code just to test something out.
  //
  // - In situations where you don't control the creation of a certain
  //   object. Certain APIs doesn't allow us to pass the dependencies, so we
  //   need to reach them some way.
  virtual XWalkBrowserContext* browser_context() OVERRIDE;

  // Stages of main parts. See content/browser_main_parts.h for description.
  virtual void PreMainMessageLoopRun() OVERRIDE;
  virtual void PostMainMessageLoopRun() OVERRIDE;
  virtual content::ContentBrowserClient* GetContentBrowserClient() OVERRIDE;

 protected:
  XWalkRunnerImpl();

 private:
  friend class XWalkMainDelegate;
  friend class ::XWalkTestSuiteInitializer;

  // To track OnRenderProcessHostGone.
  friend class application::Application;

  // This class acts as an "arm" of XWalkRunnerImpl to fulfill Content API needs,
  // it may call us back in some situations where the a more wider view of the
  // objects is necessary, e.g. during render process lifecycle callbacks.
  friend class XWalkContentBrowserClient;
  friend class XWalkRunner;

  scoped_ptr<XWalkContentBrowserClient> content_browser_client_;
  scoped_ptr<XWalkBrowserContext> runtime_context_;

  // XWalkRunnerImpl uses the XWalkComponent interface to be able to handle
  // different subsystems and call them in specific situations, e.g. when
  // extensions need to be created.
  ScopedVector<XWalkComponent> components_;

  ApplicationComponent* app_component_;

  // Remote debugger server.
  scoped_ptr<RemoteDebuggingServer> remote_debugging_server_;

  DISALLOW_COPY_AND_ASSIGN(XWalkRunnerImpl);
};

}  // namespace xwalk

#endif  // XWALK_RUNTIME_BROWSER_XWALK_RUNNER_IMPL_H_
