/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2021 Perry Werneck <perry.werneck@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 #include <config.h>
 #include <udjat/module.h>
 #include <udjat/worker.h>
 #include <udjat/request.h>
 #include <udjat/factory.h>
 #include <udjat/tools/protocol.h>
 #include <system_error>
 #include <udjat/moduleinfo.h>
 #include <udjat/tools/mainloop.h>

 using namespace std;

 /// @brief Register udjat module.
 Udjat::Module * udjat_module_init() {

	static const Udjat::ModuleInfo moduleinfo { "Module information exporter" };

	class Module : public Udjat::Module, public Udjat::Worker {
	public:

		Module() : Udjat::Module("information",moduleinfo), Udjat::Worker("info",moduleinfo) {
		};

		virtual ~Module() {
		}

		bool get(Udjat::Request &request, Udjat::Response &response) const override {

			switch(request.getAction("modules","workers","factories","protocols","services",nullptr)) {
			case 0:	// Modules
				Udjat::Module::getInfo(response);
				break;

			case 1:	// Workers
				Udjat::Worker::getInfo(response);
				break;

			case 2:	// Factories
				Udjat::Factory::getInfo(response);
				break;

			case 3: // Protocols
				Udjat::Protocol::getInfo(response);
				break;

			case 4: // Services
				Udjat::MainLoop::Service::getInfo(response);
				break;

			default:
				throw system_error(ENOENT,system_category(),"Unknown action");
			}

			return true;
		}

	};

	return new Module();
 }

