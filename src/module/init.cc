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
 #include <udjat/worker.h>
 #include <udjat/request.h>
 #include <udjat/factory.h>
 #include <system_error>
 #include <udjat/moduleinfo.h>
 #include <udjat/tools/mainloop.h>
 #include <udjat/tools/configuration.h>
 #include <udjat/tools/logger.h>
 #include <udjat/request.h>

 #include <udjat/module.h>
 #include <udjat/tools/protocol.h>

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

 /// @brief Template to get child properies by request.
 template<class T>
 inline void child_properties(Udjat::Request &request, Udjat::Response &response) {

	std::string name;
	request.pop(name);

	const T *object = T::find(name.c_str());
	if(!object) {
		throw system_error(ENOENT,system_category(),"Not found");
	}

	object->getProperties(response);

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

			auto action = request.getAction();

			if(!strcasecmp(action.c_str(),"modules")) {

				children_properties<Udjat::Module>(response);

			} else if(!strcasecmp(action.c_str(),"workers")) {

				children_properties<Udjat::Worker>(response);

			} else if(!strcasecmp(action.c_str(),"factories")) {

				children_properties<Udjat::Factory>(response);

			} else if(!strcasecmp(action.c_str(),"protocols"))  {

				children_properties<Udjat::Protocol>(response);

			} else if(!strcasecmp(action.c_str(),"services"))  {

				children_properties<Udjat::Service>(response);

			} else if(!strcasecmp(action.c_str(),"module")) {

				child_properties<Udjat::Module>(request,response);

			} else if(!strcasecmp(action.c_str(),"worker")) {

				child_properties<Udjat::Worker>(request,response);

			} else if(!strcasecmp(action.c_str(),"factory")) {

				child_properties<Udjat::Factory>(request,response);

			} else if(!strcasecmp(action.c_str(),"protocol"))  {

				child_properties<Udjat::Protocol>(request,response);

			} else if(!strcasecmp(action.c_str(),"service"))  {

				// child_properties<Udjat::Service>(request,response);

			} else {

				throw system_error(ENOENT,system_category(),"Not implemented");

			}

			return true;
		}

		bool getProperty(const char *key, std::string &value) const noexcept override {

			if(!strcasecmp(key,"options")) {
				value = Config::Value<std::string>("information","allowed","modules,workers,factories,protocols,services");
				return true;
			}

			return Udjat::Module::getProperty(key,value);
		}

	};

	return new Module();
 }

