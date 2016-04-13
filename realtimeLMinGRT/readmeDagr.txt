************************************************************************************************
To Compile and Execute,
	Compiling. from /build
		cmake	CMakeLists.txt
		make
	Running the leapd service:
		$	LeapControlPanel
		$	sudo leapd           or   $ sudo service leapd restart
	Executing from /build
		./mainAll

************************************************************************************************
About this version:

About the gesture,
It is a simple binary classification that determines whether all the fingers of the hand are together (and extended) or if they're separated one from each other.
In this proyect, I'll look for the way of classifying the data received alive from LM and previously trained through the file archivo dataFromLM.csv
