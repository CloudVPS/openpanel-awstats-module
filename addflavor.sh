#!/bin/sh
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

