# caenvx2750-nscldaq
NSCLDAQ support for the CAEN Vx2750 digitizer.

This repository will contain initial code for an add on to the NSCLDAQ code that
supports the CAEN Vx2750 digitizer.   Once this code is working the repository
will be frozen and the code migrated into the mainline NSCLDAQ code base.

# Installation:


1.   After unwrapping the tarball, cd to the src subdirectory.
2.   Choose a version of NSCLDAQ and source its environment setting script e.g.
````
     . /usr/opt/daq/12.0-pre5/daqsetup.bash
````
3.   Choose a version of SpecTcl and set SPECTCLHOME to point to it e.g.:
````
     export SPECTCLHOME=/usr/opt/spectcl/5.12-08
````
4.   Build the two libraries:
````
     make all
````     
5.  Install it somewhere e.g.:
````
     make install PREFIX=/usr/opt/lbnl
````    
6.  Read the documentation that was installed at $PREFIX/html e.g.
````
     firefox /usr/opt/lbnl/html/index.html
````    


