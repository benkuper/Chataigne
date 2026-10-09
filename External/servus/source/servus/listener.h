/* Copyright (c) 2015, Human Brain Project
 *                     Daniel.Nachbaur@epfl.ch
 *
 * This file is part of Servus <https://github.com/HBPVIS/Servus>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SERVUS_LISTENER_H
#define SERVUS_LISTENER_H

namespace servus
{
/**
 * A listener interface which can be implemented to listen on changes of a
 * Servus instance.
 *
 * @version 1.2
 */
class Listener
{
public:
    virtual ~Listener() {}
    /**
     * Called after a new instance was added after being discovered.
     *
     * @param instance the name of the new instance
     * @version 1.2
     */
    virtual void instanceAdded(const std::string& instance) = 0;

    /**
     * Called after an instance was removed from the list of discovered
     * instances.
     *
     * @param instance the name of the instance that was removed.
     * @version 1.2
     */
    virtual void instanceRemoved(const std::string& instance) = 0;
};
}

#endif // SERVUS_LISTENER_H
