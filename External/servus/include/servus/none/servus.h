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

namespace servus
{
namespace none
{
class Servus : public servus::Servus::Impl
{
public:
    explicit Servus(const std::string& name)
        : servus::Servus::Impl(name)
    {
    }
    virtual ~Servus() {}
    std::string getClassName() const { return "none"; }
    servus::Servus::Result announce(const unsigned short,
                                    const std::string&) final
    {
        return servus::Servus::Result(servus::Servus::Result::NOT_SUPPORTED);
    }

    void withdraw() final {}
    bool isAnnounced() const final { return false; }
    servus::Servus::Result beginBrowsing(const servus::Servus::Interface) final
    {
        return servus::Servus::Result(servus::Servus::Result::NOT_SUPPORTED);
    }

    servus::Servus::Result browse(const int32_t) final
    {
        return servus::Servus::Result(servus::Servus::Result::NOT_SUPPORTED);
    }

    void endBrowsing() final {}
    bool isBrowsing() const final { return false; }
    void _updateRecord() final {}
};
}
}
