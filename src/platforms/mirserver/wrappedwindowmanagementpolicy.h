/*
 * Copyright (C) 2016-2017 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranties of MERCHANTABILITY,
 * SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WRAPPEDWINDOWMANAGEMENTPOLICY_H
#define WRAPPEDWINDOWMANAGEMENTPOLICY_H

#include "qtmir/windowmanagementpolicy.h"
#include "appnotifier.h"
#include "qteventfeeder.h"
#include "windowcontroller.h"
#include "windowmodelnotifier.h"
#include "workspacecontroller.h"


using namespace mir::geometry;

class QtEventFeeder;

class WrappedWindowManagementPolicy : public qtmir::WindowManagementPolicy
{
public:
    WrappedWindowManagementPolicy(const miral::WindowManagerTools &tools,
                                  qtmir::WindowModelNotifier &windowModel,
                                  qtmir::WindowController &windowController,
                                  qtmir::WorkspaceController &workspaceController,
                                  qtmir::AppNotifier &appNotifier,
                                  const std::shared_ptr<QtEventFeeder> &eventFeeder,
                                  const qtmir::WindowManagmentPolicyBuilder &wmBuilder);

    // From WindowManagementPolicy
    auto place_new_window(const miral::ApplicationInfo &app_info,
                           const miral::WindowSpecification &request_parameters)
        -> miral::WindowSpecification override;

    void handle_window_ready(miral::WindowInfo &windowInfo) override;
    void handle_modify_window(
            miral::WindowInfo &windowInfo,
            const miral::WindowSpecification &modifications) override;
    void handle_raise_window(miral::WindowInfo &windowInfo) override;

    auto confirm_placement_on_display(
        const miral::WindowInfo &window_info,
        MirWindowState new_state,
        const mir::geometry::Rectangle &new_placement) -> mir::geometry::Rectangle override;

    bool handle_keyboard_event(const MirKeyboardEvent *event) override;
    bool handle_touch_event(const MirTouchEvent *event) override;
    bool handle_pointer_event(const MirPointerEvent *event) override;

    void advise_begin() override;
    void advise_end() override;

    void advise_new_app(miral::ApplicationInfo &application) override;
    void advise_delete_app(const miral::ApplicationInfo &application) override;

    void advise_new_window(const miral::WindowInfo &windowInfo) override;
    void advise_focus_lost(const miral::WindowInfo &info) override;
    void advise_focus_gained(const miral::WindowInfo &info) override;
    void advise_state_change(const miral::WindowInfo &info, MirWindowState state) override;
    void advise_move_to(const miral::WindowInfo &windowInfo, Point topLeft) override;
    void advise_resize(const miral::WindowInfo &info, const Size &newSize) override;
    void advise_delete_window(const miral::WindowInfo &windowInfo) override;
    void advise_raise(const std::vector<miral::Window> &windows) override;

    void handle_request_drag_and_drop(miral::WindowInfo &window_info) override;
    void handle_request_move(miral::WindowInfo &window_info, const MirInputEvent *input_event) override;
    void handle_request_resize(miral::WindowInfo &window_info, const MirInputEvent *input_event, MirResizeEdge edge) override;

    Rectangle confirm_inherited_move(const miral::WindowInfo &windowInfo, Displacement movement) override;

    void advise_adding_to_workspace(const std::shared_ptr<miral::Workspace> &workspace,
                                    const std::vector<miral::Window> &windows) override;
    void advise_removing_from_workspace(const std::shared_ptr<miral::Workspace> &workspace,
                                        const std::vector<miral::Window> &windows) override;

    // Methods for consumption by WindowControllerInterface
    void deliver_keyboard_event(const MirKeyboardEvent *event, const miral::Window &window);
    void deliver_touch_event   (const MirTouchEvent *event,    const miral::Window &window);
    void deliver_pointer_event (const MirPointerEvent *event,  const miral::Window &window);

    void activate(const miral::Window &window);
    void resize(const miral::Window &window, const Size size);
    void move  (const miral::Window &window, const Point topLeft);
    void raise(const miral::Window &window);
    void requestState(const miral::Window &window, const Mir::State state);

    void ask_client_to_close(const miral::Window &window);
    void forceClose(const miral::Window &window);

    void set_window_confinement_regions(const QVector<QRect> &regions);
    void set_window_margins(MirWindowType windowType, const QMargins &margins);

    // Methods for consumption by WorkspaceControllerInterface
    void for_each_window_in_workspace(const std::shared_ptr<miral::Workspace> &workspace,
                                      std::function<void(miral::Window const&)> const& callback);
    void move_worspace_content_to_workspace(const std::shared_ptr<miral::Workspace> &to,
                                            const std::shared_ptr<miral::Workspace> &from);
    void move_window_to_workspace(const miral::Window &window,
                                  const std::shared_ptr<miral::Workspace> &workspace);

private:
    std::shared_ptr<qtmir::WindowManagementPolicy> m_wrapper;
};

#endif // WRAPPEDWINDOWMANAGEMENTPOLICY_H