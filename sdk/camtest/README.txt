CamTest - Generic module template, can be compiled as remote or local

To compile as a remote module:

qibuild configure
qibuild make

To compile as a local module:

qibuild configure -D REMOTE=OFF
qibuild make
