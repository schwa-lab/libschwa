1: Symbolically link a Makefile
	ln -s Makefile.linux Makefile

2: Include the schwa home directory as external
	ln -s /n/schwafs/home/schwa/64 ext

3: Compile
	make
