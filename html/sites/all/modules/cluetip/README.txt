// $Id: README.txt,v 1.1.4.1 2009/02/10 09:23:57 acm Exp $

Cluetip module:
-------------------------
Author - Chris Shattuck (www.impliedbydesign.com)
Co-Author - Alex McFadyen (www.openlyconnected.com)
License - GPL


Overview:
-------------------------
The cluetip module is a wrapper for the jquery cluetip plugin which can
be found here: http://plugins.learningjquery.com/cluetip/ and downloaded
here: http://plugins.jquery.com/project/cluetip/. The Cluetip plugin
provides nice, configurable hover-overs using the "title" attribute. The
plugin has many options, and includes a "demo" folder, which you should
check out to learn about how it works.

The "dimentions" and "hover intent" plugins are includes in the cluetip
plugin distribution.


As of D6 this module is now dependent on the JQ module 
(http://drupal.org/project/jq), as cluetips contains the "hover intent" 
it will be used, however if you have the dedicated "hover intent" module 
(http://drupal.org/project/hoverintent) then you can simply set the bool 
on line 18 of cluetip.module to true. 



Installation:
-------------------------
- Download the Cluetip module and copy it into your 'modules'
directory.
- Download Cluetip from http://plugins.jquery.com/project/cluetip/
, unzip and put the entire directory in the cluetip module folder. 
- Go to Administer >> Modules and enable the module.
- Go to the JQ module settings page /admin/settings/jq and enable cluetip

Example:
-------------------------
The Cluetip module will handle including all the dependent files 
required for cluetip to work. To use, use the following instructions:
- In your module, use the cluetip_load() function to load a js file
that includes the parameters for your cluetip. As an example, you can
do the following:
1. Add the following code to your module or template.php file:
jq_add('cluetip');
2. To use the cluetip, you need to add the class "cluetip-title" to 
your element, and form the title attribute in the following way:
<div class="cluetip-title" title="Header|The body of the title".


Last updated:
------------
; $Id: README.txt,v 1.1.4.1 2009/02/10 09:23:57 acm Exp $