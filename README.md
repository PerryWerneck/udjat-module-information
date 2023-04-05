# Internal data exporter for udjat

Implements a libudjat worker (for httpd and other services) showing the internals of a libudjat based application, the modules loaded, the available workers, factories and services, the agent's firstlevel and global, etc.

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![CodeQL](https://github.com/PerryWerneck/udjat-module-information/actions/workflows/codeql.yml/badge.svg?branch=develop)](https://github.com/PerryWerneck/udjat-module-information/actions/workflows/codeql.yml)
[![MinGW](https://github.com/PerryWerneck/udjat-module-information/actions/workflows/msys2.yml/badge.svg)](https://github.com/PerryWerneck/udjat-module-information/actions/workflows/msys2.yml)
[![build result](https://build.opensuse.org/projects/home:PerryWerneck:udjat/packages/udjat-module-information/badge.svg?type=percent)](https://build.opensuse.org/package/show/home:PerryWerneck:udjat/udjat-module-information)


## Example (using [civetweb module](../../../udjat-module-civetweb) as http exporter):

http://127.0.0.1:8989/api/1.0/info/modules.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>
<response>
   <item>
      <bugreport type="string">perry.werneck@gmail.com</bugreport>
      <build type="signed">20230224</build>
      <description type="string">CivetWEB 1.15 HTTP module for udjat</description>
      <filename type="string">/usr/lib64/udjat/1.1/modules/udjat-module-civetweb.so</filename>
      <locale type="string">libudjathttpd-1.0</locale>
      <module type="string">udjat-module-civetweb</module>
      <name type="string">httpd</name>
      <url type="string">https://github.com/PerryWerneck/udjat-module-civetweb</url>
      <version type="string">1.0</version>
   </item>
   <item>
      <bugreport type="string">perry.werneck@gmail.com</bugreport>
      <build type="signed">20230225</build>
      <description type="string">Module information</description>
      <filename type="string">/usr/lib64/udjat/1.1/modules/udjat-module-information.so</filename>
      <module type="string">udjat-module-information</module>
      <name type="string">information</name>
      <url type="string">https://github.com/PerryWerneck/udjat-module-information</url>
      <version type="string">1.0</version>
   </item>
</response>
```
