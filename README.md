IVMP-MySQL
==========

Overview
--------

**IVMP-MySQL** is an open source module for the Grand Theft Auto IV multiplayer modification, IVMP.
It is a thin wrapper that allows native libmysql functions to be accessed through Squirrel.

Currently, there is _no_ support for threaded queries, though we aim to one day implement it.

Compiling
---------

### Compiling (Windows)
1. Ensure you have a working, complete IVMP-SDK _(see guide below)_
2. Ensure all requirements are accounted for _(see list below)_
3. Navigate to your IVMP-SDK folder
  * Typically, this is _path-you-cloned-svn-to/Binary/sdk/ivmp_sdk_module/_
  * For first time compilers, delete everything **BUT** the SDK directory
4. Paste in the contents of the module_src directory, overwrite where necessary.
5. Double click the IVMP-MySQL.sln file to open it with VC++
6. Use the drop down menu at the top to select either **Debug** or **Release** configuration
  * **Debug** should be used for developers who wish to experiment with the module
  * **Release** should be used for general users of the module
7. Right click the IVMP-MySQL project in the Solution Explorer and select _Clean_
  * This step is optional, but it assumes a clean compilation.
8. Right click the IVMP-MySQL project in the Solution Explorer and select _Build_
  * This stage normally will take around 2-3 minutes (maybe less depending on your computer), be patient!
9. Once built, you should have the file **ivmp-mysql.dll** in your _path-you-cloned-svn-to/Binary/modules_ folder
10. You can now use the IVMP-MySQL module! :)
 
### Compiling (Linux)

_To be added, experiment with the makefile if you are confident with that.
Anyone wishing to help out here should send me an email at alex(at)rathbone(dot)org(dot)uk_

Obtaining IVMP-SDK (Windows)
----------------------------

_To be added_

Requirements
------------

* IVMP-SDK
* libmysql C connector
* Visual Studio 2010 C++ _(Windows Only)_
* GNU Compiler _(Linux only)_

_Note: Whilst boost is not yet formally required, it would be a good idea to have it installed
as you will need it for future releases._

How can I contribute?
---------------------

Fork the source and start hacking around :) If you feel you've managed to do something useful,
send a merge request!