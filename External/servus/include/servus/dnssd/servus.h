/* Copyright (c) 2014-2017, Stefan.Eilemann@epfl.ch
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

#ifndef _MSC_VER
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <dns_sd.h>

#include <algorithm>
#include <cassert>

#define WARN std::cerr << __FILE__ << ":" << __LINE__ << ": "

namespace servus
{
namespace dnssd
{
class Servus : public servus::Servus::Impl
{
public:
    explicit Servus(const std::string& name)
        : Servus::Impl(name)
        , _out(0)
        , _in(0)
        , _result(servus::Servus::Result::PENDING)
    {
    }

    virtual ~Servus()
    {
        withdraw();
        endBrowsing();
    }

    std::string getClassName() const { return "dnssd"; }
    servus::Servus::Result announce(const unsigned short port,
                                    const std::string& instance) final
    {
        if (_out)
            return servus::Servus::Result(servus::Servus::Result::PENDING);

        TXTRecordRef record;
        _createTXTRecord(record);

        const servus::Servus::Result result(DNSServiceRegister(
            &_out, 0 /* flags */, 0 /* all interfaces */,
            instance.empty() ? 0 : instance.c_str(), _name.c_str(),
            0 /* default domains */, 0 /* hostname */, htons(port),
            TXTRecordGetLength(&record), TXTRecordGetBytesPtr(&record),
            (DNSServiceRegisterReply)registerCBS_, this));
        TXTRecordDeallocate(&record);

        if (result)
            return _handleEvents(_out, ANNOUNCE_TIMEOUT);

        WARN << "DNSServiceRegister returned: " << result << std::endl;
        return result;
    }

    void withdraw() final
    {
        if (!_out)
            return;

        DNSServiceRefDeallocate(_out);
        _out = 0;
    }

    bool isAnnounced() const final { return _out != 0; }
    servus::Servus::Result beginBrowsing(
        const ::servus::Servus::Interface addr) final
    {
        if (_in)
            return servus::Servus::Result(servus::Servus::Result::PENDING);

        _instanceMap.clear();
        return _browse(addr);
    }

    servus::Servus::Result browse(const int32_t timeout) final
    {
        return _handleEvents(_in, timeout);
    }

    void endBrowsing() final
    {
        if (!_in)
            return;

        DNSServiceRefDeallocate(_in);
        _in = 0;
    }

    bool isBrowsing() const final { return _in != 0; }
private:
    DNSServiceRef _out; //!< used for announce()
    DNSServiceRef _in;  //!< used to browse()
    int32_t _result;
    std::string _browsedName;

    servus::Servus::Result _browse(const ::servus::Servus::Interface addr)
    {
        assert(!_in);
        const DNSServiceErrorType error =
            DNSServiceBrowse(&_in, 0, addr, _name.c_str(), "",
                             (DNSServiceBrowseReply)_browseCBS, this);

        if (error != kDNSServiceErr_NoError)
        {
            WARN << "DNSServiceDiscovery error: " << error << " for " << _name
                 << " on " << addr << std::endl;
            endBrowsing();
        }
        return servus::Servus::Result(error);
    }

    void _updateRecord() final
    {
        if (!_out)
            return;

        TXTRecordRef record;
        _createTXTRecord(record);

        const DNSServiceErrorType error =
            DNSServiceUpdateRecord(_out, 0, 0, TXTRecordGetLength(&record),
                                   TXTRecordGetBytesPtr(&record), 0);
        TXTRecordDeallocate(&record);
        if (error != kDNSServiceErr_NoError)
            WARN << "DNSServiceUpdateRecord error: " << error << std::endl;
    }

    void _createTXTRecord(TXTRecordRef& record)
    {
        TXTRecordCreate(&record, 0, 0);
        for (const auto& i : _data)
        {
            const std::string& key = i.first;
            const std::string& value = i.second;
            const uint8_t valueSize =
                value.length() > 255 ? 255 : uint8_t(value.length());
            TXTRecordSetValue(&record, key.c_str(), valueSize, value.c_str());
        }
    }

    servus::Servus::Result _handleEvents(DNSServiceRef service,
                                         const int32_t timeout = -1)
    {
        assert(service);
        if (!service)
            return servus::Servus::Result(kDNSServiceErr_Unknown);

        const int fd = DNSServiceRefSockFD(service);
        const int nfds = fd + 1;

        assert(fd >= 0);
        if (fd < 0)
            return servus::Servus::Result(kDNSServiceErr_BadParam);

        while (_result == servus::Servus::Result::PENDING)
        {
            fd_set fdSet;
            FD_ZERO(&fdSet);
            FD_SET(fd, &fdSet);

            struct timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = (timeout % 1000) * 1000;

            const int result =
                ::select(nfds, &fdSet, 0, 0, timeout < 0 ? 0 : &tv);
            switch (result)
            {
            case 0: // timeout
                _result = kDNSServiceErr_NoError;
                break;

            case -1: // error
                WARN << "Select error: " << strerror(errno) << " (" << errno
                     << ")" << std::endl;
                if (errno != EINTR)
                {
                    withdraw();
                    _result = errno;
                }
                break;

            default:
                if (FD_ISSET(fd, &fdSet))
                {
                    const DNSServiceErrorType error =
                        DNSServiceProcessResult(service);

                    if (error != kDNSServiceErr_NoError)
                    {
                        WARN << "DNSServiceProcessResult error: " << error
                             << std::endl;
                        withdraw();
                        _result = error;
                    }
                }
                break;
            }
        }

        const servus::Servus::Result result(_result);
        _result = servus::Servus::Result::PENDING; // reset for next operation
        return result;
    }

    static void registerCBS_(DNSServiceRef, DNSServiceFlags,
                             DNSServiceErrorType error, const char* name,
                             const char* type, const char* domain,
                             Servus* servus)
    {
        servus->registerCB_(name, type, domain, error);
    }

    void registerCB_(const char* /*name*/, const char* /*type*/,
                     const char* /*domain*/, DNSServiceErrorType error)
    {
        // if( error == kDNSServiceErr_NoError)
        //    LBINFO << "Registered " << name << "." << type << "." << domain
        //              << std::endl;

        if (error != kDNSServiceErr_NoError)
        {
            WARN << "Register callback error: " << error << std::endl;
            withdraw();
        }
        _result = error;
    }

    static void _browseCBS(DNSServiceRef, DNSServiceFlags flags,
                           uint32_t interfaceIdx, DNSServiceErrorType error,
                           const char* name, const char* type,
                           const char* domain, Servus* servus)
    {
        servus->browseCB_(flags, interfaceIdx, error, name, type, domain);
    }

    void browseCB_(DNSServiceFlags flags, uint32_t interfaceIdx,
                   DNSServiceErrorType error, const char* name,
                   const char* type, const char* domain)
    {
        if (error != kDNSServiceErr_NoError)
        {
            WARN << "Browse callback error: " << error << std::endl;
            return;
        }

        if (flags & kDNSServiceFlagsAdd)
        {
            _browsedName = name;

            DNSServiceRef service = 0;
            const DNSServiceErrorType resolve =
                DNSServiceResolve(&service, 0, interfaceIdx, name, type, domain,
                                  (DNSServiceResolveReply)resolveCBS_, this);
            if (resolve != kDNSServiceErr_NoError)
                WARN << "DNSServiceResolve error: " << resolve << std::endl;

            if (service)
            {
                _handleEvents(service, 500);
                DNSServiceRefDeallocate(service);
            }
        }
        else // dns_sd.h: callback with the Add flag NOT set indicates a Remove
        {
            _instanceMap.erase(name);
            for (Listener* listener : _listeners)
                listener->instanceRemoved(name);
        }
    }

    static void resolveCBS_(DNSServiceRef, DNSServiceFlags,
                            uint32_t /*interfaceIdx*/,
                            DNSServiceErrorType error, const char* /*name*/,
                            const char* host, uint16_t /*port*/,
                            uint16_t txtLen, const unsigned char* txt,
                            Servus* servus)
    {
        if (error == kDNSServiceErr_NoError)
            servus->resolveCB_(host, txtLen, txt);
        servus->_result = error;
    }

    void resolveCB_(const char* host, uint16_t txtLen, const unsigned char* txt)
    {
        ValueMap& values = _instanceMap[_browsedName];
        values["servus_host"] = host;

        char key[256] = {0};
        const char* value = 0;
        uint8_t valueLen = 0;

        uint16_t i = 0;
        while (TXTRecordGetItemAtIndex(txtLen, txt, i, sizeof(key), key,
                                       &valueLen, (const void**)(&value)) ==
               kDNSServiceErr_NoError)
        {
            values[key] = std::string(value, valueLen);
            ++i;
        }
        for (Listener* listener : _listeners)
            listener->instanceAdded(_browsedName);
    }
};
}
}
