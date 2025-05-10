#1/bin/bash

DIRPATH="Build"
(
	mkdir $DIRPATH
	cd $DIRPATH
	cmake ..
	make
)
