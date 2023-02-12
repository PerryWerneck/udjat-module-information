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
 using namespace Udjat;

 /// @brief Template to enumerate children with their properties.
 template<class T>
 inline void children_properties(Udjat::Response &response) {
	response.reset(Value::Array);
	T::for_each([&response](const T &object){
		object.getProperties(response.append(Value::Object));
		return false;
	});
 }

 /// @brief Register udjat module.
 Udjat::Module * udjat_module_init() {

	static const Udjat::ModuleInfo moduleinfo { "Module information" };

	class Module : public Udjat::Module, public Udjat::Worker {
	public:

		Module() : Udjat::Module("information",moduleinfo), Udjat::Worker("info",moduleinfo) {
		};

		virtual ~Module() {
		}

		bool get(Udjat::Request &request, Udjat::Response &response) const override {


			switch(request.getAction("modules","workers","factories","protocols","services",nullptr)) {
			case 0:	// Modules
				children_properties<Udjat::Module>(response);
				break;

			case 1:	// Workers
				children_properties<Udjat::Worker>(response);
				break;

			case 2:	// Factories
				children_properties<Udjat::Factory>(response);
				break;

			case 3: // Protocols
				// children_properties<Udjat::Protocol>(response);
				break;

			case 4: // Services
				// children_properties<Udjat::MainLoop::Service>(response);
				break;

			default:
				throw system_error(ENOENT,system_category(),"Unknown action");
			}

			return true;
		}

		bool getProperty(const char *key, std::string &value) const noexcept override {

			if(!strcasecmp(key,"options")) {
				value = "modules,workers,factories,protocols,services";
				return true;
			}

			return Udjat::Module::getProperty(key,value);
		}

	};

	return new Module();
 }

