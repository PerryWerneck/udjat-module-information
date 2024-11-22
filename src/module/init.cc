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
 #include <udjat/tools/worker.h>
 #include <udjat/factory.h>
 #include <system_error>
 #include <udjat/moduleinfo.h>
 #include <udjat/tools/mainloop.h>
 #include <udjat/tools/configuration.h>
 #include <udjat/tools/logger.h>
 #include <udjat/tools/interface.h>
 
 #include <udjat/module.h>
 #include <udjat/tools/protocol.h>

 using namespace std;
 using namespace Udjat;

 /// @brief Show class or object properties.
 template<class T>
 inline void show_properties(Udjat::Value &response) {
	response.reset(Value::Array);
	T::for_each([&response](const T &object){
		object.getProperties(response.append(Value::Object));
		return false;
	});
 }

 /// @brief Register udjat module.
 Udjat::Module * udjat_module_init() {

	static const Udjat::ModuleInfo moduleinfo { "Module information" };

	class Module : public Udjat::Module, public Udjat::Interface {
	public:

		Module() : Udjat::Module("information",moduleinfo), Udjat::Interface("appinfo") {
		};

		virtual ~Module() {
		}

		void trace_paths(const char *url_prefix) const noexcept override {

			for(string &value : Config::Value<std::vector<std::string>>("information","paths","modules,workers,factories,protocols,services")) {
				Logger::String{"Service info available on ",url_prefix,"info/",value.c_str()}.trace("info");
			}

		}

		bool for_each(const std::function<bool(const size_t index, bool input, const char *name, const Value::Type type)> &) const override {
			return false;	
		}

		void call(const char *path, Udjat::Value &response) override  {

			switch(String{path}.select("modules","workers","factories","protocols","services",nullptr)) {
			case 0: // modules
				show_properties<Udjat::Module>(response);
				break;

			case 1: // workers
				show_properties<Udjat::Worker>(response);
				break;

			case 2: // factories
				show_properties<Udjat::Factory>(response);
				break;

			case 3: // protocols
				show_properties<Udjat::Protocol>(response);
				break;

			case 4: // services
				show_properties<Udjat::Service>(response);
				break;

			default:
				throw system_error(ENOENT,system_category(),"Not implemented");
			}

		}

		bool getProperty(const char *key, std::string &value) const noexcept override {

			if(!strcasecmp(key,"options")) {
				value = Config::Value<std::string>("information","paths","modules,workers,factories,protocols,services");
				return true;
			}

			return Udjat::Module::getProperty(key,value);
		}

	};

	return new Module();
 }

