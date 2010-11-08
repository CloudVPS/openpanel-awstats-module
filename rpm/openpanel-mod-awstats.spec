# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

%define version 0.8.16

%define libpath /usr/lib
%ifarch x86_64
  %define libpath /usr/lib64
%endif

Summary: Configure awstats per (sub) domain
Name: openpanel-mod-awstats
Version: %version
Release: 1
License: GPLv2
Group: Development
Source: http://packages.openpanel.com/archive/openpanel-mod-awstats-%{version}.tar.gz
Patch1: openpanel-mod-awstats-00-makefile
BuildRoot: /var/tmp/%{name}-buildroot
Requires: openpanel-core >= 0.8.3
Requires: openpanel-mod-apache2
Requires: awstats

%description
Configure awstats per (sub) domain 
Openpanel awstats management module

%prep
%setup -q -n openpanel-mod-awstats-%version
%patch1 -p0 -b .buildroot

%build
BUILD_ROOT=$RPM_BUILD_ROOT
./configure
make

%install
BUILD_ROOT=$RPM_BUILD_ROOT
rm -rf ${BUILD_ROOT}
mkdir -p ${BUILD_ROOT}/var/opencore/modules/AWStats.module
cp -rf ./awstatsmodule.app ${BUILD_ROOT}/var/opencore/modules/AWStats.module/
cp module.xml ${BUILD_ROOT}/var/opencore/modules/AWStats.module/module.xml
install -m 755 verify ${BUILD_ROOT}/var/opencore/modules/AWStats.module/verify
mkdir -p ${BUILD_ROOT}/etc/awstats

%post
ln -sf awstatsmodule.app/exec /var/opencore/modules/AWStats.module/action
mkdir -p /var/opencore/conf/staging/AWStats
chown opencore:authd /var/opencore/conf/staging/AWStats

%files
%defattr(-,root,root)
/
