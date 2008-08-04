// --------------------------------------------------------------------------
// OpenPanel - The Open Source Control Panel
// Copyright (c) 2006-2007 PanelSix
//
// This software and its source code are subject to version 2 of the
// GNU General Public License. Please be aware that use of the OpenPanel
// and PanelSix trademarks and the IconBase.com iconset may be subject
// to additional restrictions. For more information on these restrictions
// and for a copy of version 2 of the GNU General Public License, please
// visit the Legal and Privacy Information section of the OpenPanel
// website on http://www.openpanel.com/
// --------------------------------------------------------------------------

#include "cfgawstats.h"

#include <grace/file.h>
#include <grace/filesystem.h>


// ===========================================================================
// METHOD cfgawstats::writeawconfig
// ===========================================================================
bool cfgawstats::writeawconfig (void)
{
	file 	f;
	string fname = "%s/awstats.%s.conf" %format (_stagingpath, _vhostname);
	
	// Copy the template to the staging directory under 
	// it's own unique name
	fs.cp ("rsrc:awstats.conf.tmpl", fname);
	
	// Open the file in the staging directory for
	// appending, we should add some directives 
	// before it can be used by awstats
	if (! f.openappend (fname))
	{
		_error = "Error opening file for append: <%s>" %format (fname);
		return false;
	}
	
	try
	{
#ifdef __FLAVOR_LINUX_DEBIAN
		f.printf ("\nInclude \"/etc/awstats/awstats.conf.local\"\n\n");
#endif		
		f.printf ("\n");
		f.printf ("#-----------------------------------------------------------------------------\n");
		f.printf ("# DIRECTIVES BY OPENPANEL\n");
		f.printf ("# AWStats.module\n");
		f.printf ("#-----------------------------------------------------------------------------\n\n");
		f.writeln ("SiteDomain=\"%s\"" %format (_vhostname));
		f.writeln ("LogFile=\"%s\"" %format (_logfile));
	}
	catch (exception e)
	{
		_error = "File append exception: %s" %format (e.description);
		f.close ();
		return false;
	}
	
	// No need to go over aliases, they end up at the same logfile
	// apparently already stripped of their vhost.
	// So we'll just close the file and return success.
	f.close ();
	return true;
}

// ===========================================================================
// METHOD cfgawstats::getfirstruncmd
// ===========================================================================
string *cfgawstats::getfirstruncmd ()
{
	returnclass (string) ret retain;

	// This command needs to be executed after all files are
	// installed on the system
	ret.printf ("su - www-data -c \"/usr/bin/perl /usr/lib/cgi-bin/awstats.pl ");
	ret.printf ("-update -config=%s\"", _vhostname.str());
	
	return &ret;
}

// ===========================================================================
// METHOD cfgawstats::writevhostconfif
// ===========================================================================
bool cfgawstats::writevhostconfif (void)
{
	file f;
	string fname = "%s/%s.awstats" %format (_stagingpath, _vhostname);
	
	if (! f.openwrite (fname))
	{
		_error = "Error opening file for writing: <%s>" %format (fname);
		return false;
	}

#ifdef __FLAVOR_LINUX_DEBIAN
	f.printf ("Alias /awstatsclasses \"/usr/share/awstats/lib/\"\n");
	f.printf ("Alias /awstats-icon/ \"/usr/share/awstats/icon/\"\n");
	f.printf ("Alias /awstatscss \"/usr/share/doc/awstats/examples/css\"\n");
	f.printf ("ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/\n");
    // f.printf ("ScriptAlias /%s/ /usr/share/awstats/\n", _uri.str());
    f.printf ("RewriteEngine On\n");
    f.printf ("RewriteRule /%s/ /cgi-bin/awstats.pl [R]\n", _uri.str());
	f.printf ("Options ExecCGI -MultiViews +SymLinksIfOwnerMatch\n");
#else
  #ifdef __FLAVOR_LINUX_REDHAT
	f.printf ("Alias /awstats/icon/ /var/www/awstats/icon/\n\n");
	f.printf ("Alias /%s/ /var/www/awstats/\n", _uri.str());
	f.printf ("<Directory /var/www/awstats/>\n");
	f.printf ("        DirectoryIndex awstats.pl\n");
	f.printf ("        Options ExecCGI\n");
	f.printf ("        AddType cgi-script .pl\n");
	f.printf ("        order allow,deny\n");
	f.printf ("        allow from all\n");
	f.printf ("#       allow from 127.0.0.1\n");
	f.printf ("</Directory>\n\n");
	f.printf ("# Alias /css/ /var/www/awstats/css/\n");
	f.printf ("# Alias /js/ /var/www/awstats/js/\n");
  #endif
#endif

	f.close ();
	return true;
}
