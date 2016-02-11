gsltest - call a shared library module locally on the robot

To use:

	cross compile a module for the NAOqi to load locally, eg.

		cd {git path}/sdk/moduletest
	
		qibuild configure -c {cc toolchain} -D REMOTE=OFF
		qibuild make -c {cc toolchain}
	
	copy the resulting library to the NAOqi, eg.
		
		scp ./build-{cc toolchain}/sdk/lib/libmoduletest.so nao@{NAO IP}:{some path}

	add the library to the NAOqi's autoload.ini, located at 
	/home/nao/naoqi/preferences/autoload.ini on the robot, eg.
		
		[user]
		/path/to/libmoduletest.so

	now cross compile gsltest for the robot, and copy it over

		cd {git path}/sdk/gsltest
		
		qibuild configure -c {cc toolchain}
		qibuild make -c {cc toolchain}

		scp ./build-{cc toolchain}/sdk/bin/gsltest nao@{NAO IP}:

	ssh into the robot and start naoqi-bin

		ssh nao@{NAO IP}

		naoqi-bin
	
	finally, run gsltest

		./gsltest
	
This should successfully demonstrate:
	- Loading a library.
	- Creating a module instance from that library. 
	- Calling a function from the module instance.
	- Deleting the module instance.
