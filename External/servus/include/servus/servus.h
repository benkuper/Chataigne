/* Copyright (c) 2012-2017, Human Brain Project
 *                          Stefan.Eilemann@epfl.ch
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

#ifndef SERVUS_SERVUS_H
#define SERVUS_SERVUS_H

#include <servus/api.h>
#include <servus/result.h> // nested base class
#include <servus/types.h>

#include <map>
#include <memory>

namespace servus
{
/** Service name to be used by unit tests. All Servus instances with this name
 * "communicate" through global data, that is, announced key-value pairs are
 * communicated to all browsing instances in the same process. */
static const std::string TEST_DRIVER{"_servus._test"};

/**
 * Simple wrapper for ZeroConf key/value pairs.
 *
 * The servus class allows simple announcement and discovery of key/value pairs
 * using ZeroConf networking. The same instance can be used to announce and/or
 * to browse a ZeroConf service. If the Servus library is compiled without
 * zeroconf support (@sa isAvailable()), this class does not do anything useful.
 *
 * Example: @include tests/servus.cpp
 */
class Servus
{
public:
    enum Interface
    {
        IF_ALL = 0, //!< use all interfaces
        // (uint32_t) -1 == kDNSServiceInterfaceIndexLocalOnly
        IF_LOCAL = (unsigned)(-1) //!< only local interfaces
    };

    /**
     * The ZeroConf operation result code.
     *
     * The result code is either one of kDNSServiceErr_ or one of static
     * constants defined by this class
     */
    class Result : public servus::Result
    {
    public:
        explicit Result(const int32_t code)
            : servus::Result(code)
        {
        }
        virtual ~Result() {}
        SERVUS_API std::string getString() const override;

        /** operation did not complete. */
        static const int32_t PENDING = -1;
        /** Servus compiled without ZeroConf support. */
        static const int32_t NOT_SUPPORTED = -2;
        /** Error during polling for event. */
        static const int32_t POLL_ERROR = -3;
    };

    /** @return true if a usable implementation is available. */
    SERVUS_API static bool isAvailable();

    /**
     * Create a new service handle.
     *
     * @param name the service descriptor, e.g., "_hwsd._tcp"
     * @version 1.1
     */
    SERVUS_API explicit Servus(const std::string& name);

    /** Destruct this service. @version 1.1 */
    SERVUS_API virtual ~Servus();

    /** @return the service name. @version 1.1 */
    SERVUS_API const std::string& getName() const;

    /**
     * Set a key/value pair to be announced.
     *
     * Keys should be at most eight characters, and values are truncated to 255
     * characters. The total length of all keys and values cannot exceed 65535
     * characters. Setting a value on an announced service causes an update
     * which needs some time to propagate after this function returns, that is,
     * calling discover() immediately afterwards will very likely not contain
     * the new key/value pair.
     *
     * @version 1.1
     */
    SERVUS_API void set(const std::string& key, const std::string& value);

    /** @return all (to be) announced keys. @version 1.1 */
    SERVUS_API Strings getKeys() const;

    /** @return the value to the given (to be) announced key. @version 1.1 */
    SERVUS_API const std::string& get(const std::string& key) const;

    /**
     * Start announcing the registered key/value pairs.
     *
     * @param port the service IP port in host byte order.
     * @param instance a host-unique instance name, hostname is used if empty.
     * @return the success status of the operation.
     * @version 1.1
     */
    SERVUS_API Result announce(const unsigned short port,
                               const std::string& instance);

    /** Stop announcing the registered key/value pairs. @version 1.1 */
    SERVUS_API void withdraw();

    /** @return true if the local data is announced. @version 1.1 */
    SERVUS_API bool isAnnounced() const;

    /**
     * Discover all announced key/value pairs.
     *
     * @param addr the scope of the discovery
     * @param browseTime the browse time, in milliseconds, to wait for new
     *                   records.
     * @return all instance names found during discovery.
     * @sa beginBrowsing(), browse(), endBrowsing()
     * @version 1.1
     */
    SERVUS_API Strings discover(const Interface addr,
                                const unsigned browseTime);

    /**
     * Begin the discovery of announced key/value pairs.
     *
     * @param addr the scope of the discovery
     * @return the success status of the operation.
     * @version 1.1
     */
    SERVUS_API Result beginBrowsing(const servus::Servus::Interface addr);

    /**
     * Browse and process discovered key/value pairs.
     *
     * @param timeout The time to spend browsing.
     * @return the success status of the operation.
     * @version 1.1
     */
    SERVUS_API Result browse(int32_t timeout = -1);

    /** Stop a discovery process and return all results. @version 1.1 */
    SERVUS_API void endBrowsing();

    /** @return true if the local data is browsing. @version 1.1 */
    SERVUS_API bool isBrowsing() const;

    /** @return all instances found during the last discovery. @version 1.1 */
    SERVUS_API Strings getInstances() const;

    /** @return all keys discovered on the given instance. @version 1.1 */
    SERVUS_API Strings getKeys(const std::string& instance) const;

    /** @return the host corresponding to the given instance. @version 1.3 */
    SERVUS_API const std::string& getHost(const std::string& instance) const;

    /** @return true if the given key was discovered. @version 1.1 */
    SERVUS_API bool containsKey(const std::string& instance,
                                const std::string& key) const;

    /** @return the value of the given key and instance. @version 1.1 */
    SERVUS_API const std::string& get(const std::string& instance,
                                      const std::string& key) const;

    /**
     * Add a listener which is invoked according to its supported callbacks.
     *
     * @param listener the listener to be added, must not be nullptr
     * @version 1.2
     */
    SERVUS_API void addListener(Listener* listener);

    /**
     * Remove a listener to stop invokation on its supported callbacks.
     *
     * @param listener the listener to be removed, must not be nullptr
     * @version 1.2
     */
    SERVUS_API void removeListener(Listener* listener);

    /** @internal */
    typedef std::map<std::string, std::map<std::string, std::string> > Data;

    /** @internal */
    SERVUS_API void getData(Data& data);

    class Impl; //!< @internal

private:
    Servus(const Servus&) = delete;
    Servus& operator=(const Servus&) = delete;

    std::unique_ptr<Impl> _impl;

    friend SERVUS_API std::ostream& operator<<(std::ostream&, const Servus&);
};

/** @return the local hostname. */
SERVUS_API std::string getHostname();

/** Output the servus instance in human-readable format. */
SERVUS_API std::ostream& operator<<(std::ostream&, const Servus&);

/** Output the servus interface in human-readable format. */
SERVUS_API std::ostream& operator<<(std::ostream&, const Servus::Interface&);
}

#endif // SERVUS_SERVUS_H
