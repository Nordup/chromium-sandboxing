// Copyright 2009 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_WIN_SRC_WINDOW_H_
#define SANDBOX_WIN_SRC_WINDOW_H_

#include <string>

#include "base/win/windows_types.h"
#include "sandbox/win/src/sandbox_types.h"

namespace sandbox {

// Creates a window station. The name is generated by the OS. The security
// descriptor is based on the security descriptor of the current window
// station.
ResultCode CreateAltWindowStation(HWINSTA* winsta);

// Creates a desktop. The name is a static string followed by the pid of the
// current process. The security descriptor on the new desktop is based on the
// security descriptor of the desktop associated with the current thread.
// If a winsta is specified, the function will switch to it before creating
// the desktop. If the functions fails the switch back to the current winsta,
// the function will return SBOX_ERROR_FAILED_TO_SWITCH_BACK_WINSTATION.
ResultCode CreateAltDesktop(HWINSTA winsta, HDESK* desktop);

// Returns the name of the desktop referenced by |desktop|. If a window
// station is specified, the name is prepended with the window station name,
// followed by a backslash. This name can be used as the lpDesktop parameter
// to CreateProcess.
std::wstring GetFullDesktopName(HWINSTA winsta, HDESK desktop);

}  // namespace sandbox

#endif  // SANDBOX_WIN_SRC_WINDOW_H_
