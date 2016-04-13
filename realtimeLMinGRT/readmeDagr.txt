************************************************************************************************
Para compilar y ejecutar,
	Compilando. Desde /build
		cmake	CMakeLists.txt
		make
	Correr el leapd:
		$	LeapControlPanel
		$	sudo leapd           o   $ sudo service leapd restart
	Ejecutando. Desde /build
		./mainAll

************************************************************************************************
Notas de 1a versión:

Sobre el gesto, es una clasificación simple binaria que determina si los dedos de la mano están juntos entre sí, o están separados uno de otro.
En éste proyecto buscaré la forma de clasificar datos en vivo recibidos de LM y ya entrenados anteriormente a través del archivo dataFromLM.csv
