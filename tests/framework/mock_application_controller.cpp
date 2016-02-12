/*
 * Copyright (C) 2015 Canonical, Ltd.
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

#include "mock_application_controller.h"

namespace qtmir
{

MockApplicationController::MockApplicationController()
{
    using namespace ::testing;
    ON_CALL(*this, primaryPidForAppId(_))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doPrimaryPidForAppId));

    ON_CALL(*this, appIdHasProcessId(_, _))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doAppIdHasProcessId));

    ON_CALL(*this, findDesktopFileForAppId(_))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doFindDesktopFileForAppId));

    ON_CALL(*this, stopApplicationWithAppId(_))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doStopApplicationWithAppId));

    ON_CALL(*this, startApplicationWithAppIdAndArgs(_, _))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doStartApplicationWithAppIdAndArgs));

    ON_CALL(*this, pauseApplicationWithAppId(_))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doPauseApplicationWithAppId));

    ON_CALL(*this, resumeApplicationWithAppId(_))
            .WillByDefault(
                Invoke(this, &MockApplicationController::doResumeApplicationWithAppId));
}

MockApplicationController::~MockApplicationController()
{

}

pid_t MockApplicationController::doPrimaryPidForAppId(const QString &appId)
{
    auto it = children.find(appId);
    if (it == children.end())
        return -1;

    return it->pid();
}


bool MockApplicationController::doAppIdHasProcessId(pid_t pid, const QString &appId)
{
    auto it = children.find(appId);
    if (it == children.end())
        return -1;

    return it->pid() == pid;
}


QFileInfo MockApplicationController::doFindDesktopFileForAppId(const QString &appId) const
{
    QString path = QString("/usr/share/applications/%1.desktop").arg(appId);
    return QFileInfo(path);
}


bool MockApplicationController::doStopApplicationWithAppId(const QString &appId)
{
    Q_UNUSED(appId);

    return false;
}


bool MockApplicationController::doStartApplicationWithAppIdAndArgs(const QString &appId, const QStringList &args)
{
    Q_UNUSED(args);

    auto child = core::posix::fork([]()
    {
        while (true);

        return core::posix::exit::Status::success;
    }, core::posix::StandardStream::empty);

    if (child.pid() > 0)
    {
        children.insert(appId, child);
        return true;
    }

    return false;
}


bool MockApplicationController::doPauseApplicationWithAppId(const QString &appId)
{
    Q_UNUSED(appId);

    return false;
}

bool MockApplicationController::doResumeApplicationWithAppId(const QString &appId)
{
    Q_UNUSED(appId);

    return false;
}

} // namespace qtmir