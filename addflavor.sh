#!/bin/sh

# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

if grep ^CXXFLAGS makeinclude | grep -q LINUX_REDHAT ; then
 cp rsrc/openpanel.module.awstats.conf.xml.redhat rsrc/openpanel.module.awstats.conf.xml
 cp rsrc/awstats.conf.tmpl.redhat rsrc/awstats.conf.tmpl
elif grep ^CXXFLAGS makeinclude | grep -q LINUX_DEBIAN ; then
 cp rsrc/openpanel.module.awstats.conf.xml.debian  rsrc/openpanel.module.awstats.conf.xml
 cp rsrc/awstats.conf.tmpl.debian rsrc/awstats.conf.tmpl
else
 echo "No compatible environment found."
 exit 1
fi

