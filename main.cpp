// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#include <openpanel-core/moduleapp.h>
#include <grace/file.h>
#include <grace/filesystem.h>
#include "awstatsmodule.h"
#include "cfgawstats.h"		// AwStats configuration writer

using namespace moderr;

APPOBJECT(awstatsmodule);

// ==========================================================================
// Main method.
// ==========================================================================
int awstatsmodule::main (void)
{
	string conferr;
	value q;

	// Add configuration watcher
	conf.addwatcher ("config", &awstatsmodule::confSystem);

   // Load will fail if watchers did not valiate.
    if (! conf.load ("openpanel.module.awstats", conferr))
    {   
        ferr.printf ("%% Error loading configuration: %s\n", conferr.str());
        return 1;
    } 		
     		
	caseselector (data["OpenCORE:Command"])
	{
		incaseof ("create") : 
			if (! checkconfig (data))
				return 0;

			if(! writeconfiguration (data))
				return 0;
				
			break;

		incaseof ("delete") :
			if (! checkconfig (data, true))
				return 0;

			if (! deleteconfiguration (data))
				return 0;
			break;
		 	
		incaseof ("validate") : ;
			
		defaultcase:
			sendresult (err_command, 
						"Unsupported command");
			return 0;
	}

	sendresult (moderr::ok, "");
	authd.quit ();

	return 0;
}

// ==========================================================================
// METHOD awstatsmodule::readconfiguration
// ==========================================================================
bool awstatsmodule::readconfiguration (void)
{
	return true;
}

// ==========================================================================
// METHOD awstatsmodule::deleteconfiguration
// ==========================================================================
bool awstatsmodule::deleteconfiguration (const value &v)
{
	string 	apachefile;
	string	awstatsfile;
	string	awcronfile;

	string vdir = conf["config"]["apache:vhostconfdir"];
	string cdir = conf["config"]["awstats:confdir"];
	string vhostid = v["Domain:Vhost"]["id"];
	string subdom = v["Domain:Vhost"]["subdomain"];
	string domid = v["Domain"]["id"];
	
	apachefile = "%s/%s.inc/%s.%s.awstats" %format (vdir,vhostid,subdom,domid);
	awstatsfile = "%s/awstats.%s.conf" %format (cdir,vhostid);

	// Try to remove the awstats apache configuration file
    authd.deletefile (apachefile);

	// Try to remove the awstats vhost config file
    authd.deletefile (awstatsfile);

	return true;
}

// ==========================================================================
// METHOD awstatsmodule::writeconfiguration
// ==========================================================================
bool awstatsmodule::writeconfiguration (const value &v)
{
	// AwStats configuration object
	cfgawstats acfg (conf["config"]["local:staging"]);
	string hostname = v["Domain:Vhost"]["id"];

	// Set extra parameters
	acfg.setvhostname (hostname);
	
#ifdef __FLAVOR_LINUX_REDHAT
	string logfile = "/var/log/httpd/openpanel/logs/%s.log" %format (hostname);
#else
	string logfile = "/var/log/apache2/openpanel/logs/%s.log" %format (hostname);
#endif
	acfg.setlogfile (logfile);
	acfg.seturi	(v["Vhost:AWStats"]["path"]);

	// write awstats configuration
	if (! acfg.writeawconfig ())
	{
		sendresult (err_unknown, acfg.error());
		return false;
	}
	
	// write apache vhost config
	if (! acfg.writevhostconfif ())
	{
		sendresult (err_unknown, acfg.error());
		return false;
	}
	
	// Install system files
	if (! this->installconfig (v))
	{
		sendresult (err_unknown, "Could not install configuration");
		return false;
	}
		
	if (! this->reloadservices ())
	{
		sendresult (err_unknown, "Could not reload config");
		return false;
	}
	
	return true;
}

// ==========================================================================
// METHOD awstatsmodule::installconfig
// ==========================================================================
bool awstatsmodule::installconfig (const value &v)
{
	string 	apachesrc, apachedst;	// Apache vhost extended config
	string	awstatsrc, awstatdst;	// Awstats configuration file

	string id = v["Domain:Vhost"]["id"];
	string confdir = conf["config"]["apache:vhostconfdir"];
	string cdir = conf["config"]["awstats:confdir"];
	
	apachesrc = "%s.awstats" %format (id);
	apachedst = "%s/%s.inc/" %format (confdir, id);
	awstatsrc = "awstats.%s.conf" %format (id);
	awstatdst = "%s/" %format (cdir);

	// Install the file to the apache2 directory
	if( authd.installfile (apachesrc, apachedst))
	{
		sendresult (err_authdaemon, "Error installing apache vhost file "
					"src=<%s> dst=<%s>" %format (apachesrc, apachedst));
		return false;
	}

	// Install the awstats configuration file
	if( authd.installfile (awstatsrc, awstatdst))
	{
		sendresult (err_authdaemon, "Error installing awstats config file "
					"src=<%s> dst=<%s>" %format (apachesrc, apachedst));
		return false;
	}

	return true;
}

// ==========================================================================
// METHOD awstatsmodule::reloadservices
// ==========================================================================
bool awstatsmodule::reloadservices (void)
{

	// We only have to reload the apache webserver
	if ( authd.reloadservice (conf["config"]["apache:servicename"]) )
	{
		sendresult( moderr::err_authdaemon, "Error reloading apache webserver");
		return false;
	}
	
	return true;
}

// ==========================================================================
// METHOD awstatsmodule::checkconfig
// ==========================================================================
bool awstatsmodule::checkconfig (value &v, bool fordelete)
{
	// Context should be Vhost:AwStats
	if (v["OpenCORE:Session"]["classid"] != "Vhost:AWStats")
	{
		sendresult (moderr::err_context, "Context can only be Vhost:AWStats");		
		return false;	
	}
	
	if (v.exists ("Domain"))
	{
		if (! v["Domain"].exists ("id"))
		{
			sendresult (moderr::err_notfound, "Missing parameter: Domain.id");
			return false;
		}
	}
	else
	{
		sendresult (moderr::err_notfound, "Domain class not found");		
		return false;
	}
	
	if (v.exists ("Domain:Vhost"))
	{
		if (! v["Domain:Vhost"].exists ("id"))
		{
			sendresult (moderr::err_notfound, "Missing parameter: "
						"Domain:Vhost.id");
			return false;
		}
	}
	else
	{
		sendresult (moderr::err_notfound, "Domain:Vhost class not found");
		return false;
	}
	
	if (v.exists ("Vhost:AWStats"))
	{
		value &vawstats = v["Vhost:AWStats"];
		if (! (vawstats.exists ("id") &&
			   (fordelete || vawstats.exists ("path"))))
		{
			sendresult (moderr::err_notfound, "Missing required parameter");
			return false;
		}
	}
	else
	{
		sendresult (moderr::err_notfound, "Vhost:AWStats class not found");
		return false;
	}
	
	return true;
}

// ==========================================================================
// METHOD awstatsmodule::confSystem
// ==========================================================================
bool awstatsmodule::confSystem (config::action act, keypath &kp,
                const value &nval, const value &oval)
{
	switch (act)
	{
		case config::isvalid:
			return true;

		case config::create:
			return true;		
	}

	return false;
}
