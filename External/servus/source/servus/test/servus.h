/* Copyright (c) 2017, Stefan.Eilemann@epfl.ch
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

#include <algorithm>
#include <mutex>
#include <set>
#include <iterator>

namespace servus
{
namespace test
{
class Servus;

namespace
{
struct
{
    std::mutex mutex;
    std::set<Servus*> instances;
} _directory;
}

class Servus : public servus::Servus::Impl
{
public:
    Servus()
        : servus::Servus::Impl(servus::TEST_DRIVER)
    {
    }

    virtual ~Servus()
    {
        withdraw();
        endBrowsing();
    }

    std::string getClassName() const { return "test"; }
    servus::Servus::Result announce(const unsigned short port,
                                    const std::string& instance) final
    {
        std::lock_guard<std::mutex> lock(_directory.mutex);

        _port = port;
        if (instance.empty())
            _instance = getHostname();
        else
            _instance = instance;
        _directory.instances.insert(this);
        _announced = true;
        return servus::Servus::Result(servus::Result::SUCCESS);
    }

    void withdraw() final
    {
        std::lock_guard<std::mutex> lock(_directory.mutex);

        _announced = false;
        _directory.instances.erase(this);
        _port = 0;
        _instance.clear();
    }

    bool isAnnounced() const final { return _announced; }
    servus::Servus::Result beginBrowsing(
        const ::servus::Servus::Interface) final
    {
        if (_browsing)
            return servus::Servus::Result(servus::Servus::Result::PENDING);

        _instances.clear();
        _browsing = true;
        return servus::Servus::Result(servus::Servus::Result::SUCCESS);
    }

    servus::Servus::Result browse(const int32_t) final
    {
        std::lock_guard<std::mutex> lock(_directory.mutex);

        std::vector<Servus*> diff;
        std::set_symmetric_difference(_directory.instances.begin(),
                                      _directory.instances.end(),
                                      _instances.begin(), _instances.end(),
                                      back_inserter(diff));

        _instanceMap.clear();
        for (auto i : _directory.instances)
        {
            ValueMap& values = _instanceMap[i->_instance];
            values.clear();
            values["servus_host"] = "localhost";

            for (const auto& j : i->_data)
                values[j.first] = j.second;
        }

        for (auto i : diff)
        {
            if (_instances.count(i) == 0)
            {
                _instances.insert(i);
                for (Listener* listener : _listeners)
                    listener->instanceAdded(i->_instance);
            }
            else
            {
                for (Listener* listener : _listeners)
                    listener->instanceRemoved(i->_instance);
                _instances.erase(i);
            }
        }
        return servus::Servus::Result(servus::Servus::Result::SUCCESS);
    }

    void endBrowsing() final
    {
        _browsing = false;
        _instances.clear();
    }

    bool isBrowsing() const final { return _browsing; }
private:
    std::string _instance;
    unsigned short _port{0};
    bool _announced{false};
    bool _browsing{false};

    std::set<Servus*> _instances;

    void _updateRecord() final { /*nop*/}
};
}
}
