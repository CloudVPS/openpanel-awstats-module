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

#ifndef _CFGAWSTATS_H
#define _CFGAWSTATS_H 1

#include <grace/str.h>
#include <grace/value.h>
#include <grace/array.h>


/// Awstats configuration 
/// class
/// Used to create awstats configuration files
/// 
///
/// This class uses a default configuration file for awstats
/// and appends some virtual host specific lines to it
/// The basic configuration file is: awstats.conf.bin
///
class cfgawstats
{

	public:
	
					 /// Constructor
					 cfgawstats (const string &stagingpath)
					 {
					 	_stagingpath 	= stagingpath;
					 	_uri			= "awstats";
					 }
					 
					 /// Destructor
					~cfgawstats (void) { }
	
	
	
					 // ========================================================
					 // Properties
					 // ========================================================
					 
					 /// Set webstats uri
		void		 seturi (const string &uri)
					 {
					 	_uri = uri.trim ("/");
					 }
					 
					 /// get webstats uri
		string		&geturi (void)
					 {
					 	return _uri;
					 }
					 
					 
					 /// Set virual host name
					 /// \param vhost virtual hostname string
		void		 setvhostname (const string &vhost)
					 {
					 	_vhostname = vhost;
					 }
					 
					 /// get virtual host name
					 /// \return current virtual hostname string
		string 		&getvhostname (void)
					 {
					 	return _vhostname;
					 }
		
		
					 /// Set the apache log file to use
					 /// \param logfile path to apache access log file
		void		 setlogfile (const string &logfile)
					 {
					 	_logfile = logfile;
					 }
					 
					 /// Get the logfile name
					 /// \return apache access log file name string
		string		&getlogfile (void)
					 {
					 	return _logfile;
					 }
				
		string		&error (void)
					 {
					 	return _error;
					 }
	
					 // ========================================================
					 // Config create functions
					 // ========================================================
					 
					 /// Write awstats configuration file
		bool		 writeawconfig (void);
		
					 /// write the apache config file
					 /// fo inclusion in its vhost file
		bool		 writevhostconfif (void);

		
					 /// Get the commands to make a first 
					 /// run
					 /// return string to execute through a shell
		string		*getfirstruncmd ();
	
	
	private:
		
		string		 	_vhostname;			///< Name of the vhost to monitor
		string		 	_logfile;			///< The apache log file to use
		string			_uri;				///< uri used for webstats
		
		string		 	_stagingpath;		///< Path to staging directory
		array <string>	_hostaliases;		///< Host aliases used for this dom.
	
		string 			_error;				///< Last error
};

#endif 
