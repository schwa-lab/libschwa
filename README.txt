1: Symbolically link a Makefile

	ln -s Makefile.linux Makefile

2: Include the schwa home directory as external

	ln -s /n/schwafs/home/schwa/64 ext

3: Compile

	make

4: Build the python APIs

	python2.6 setup.py build

5: Add the python APIs to the PYTHONPATH and LD_LIBRARY_PATH. The following example
	is from schwapy:env.sh and assumes that there is a bash function called pathprepend.

	# Add the schwa repo.
	SCHWA_BUILD=$SCHWA_WORKSPACE/schwa/build/lib.linux-x86_64-2.6
	SCHWA_LIB=$SCHWA_WORKSPACE/schwa/lib
	if [ -d "$SCHWA_BUILD" ]; then
			echo "Adding $SCHWA_BUILD to PYTHONPATH" >&2
			pathprepend $SCHWA_BUILD PYTHONPATH
	else
			echo "Not adding $SCHWA_BUILD to PYTHONPATH (missing directory)" >&2
	fi
	if [ -d "$SCHWA_LIB" ]; then
			echo "Adding $SCHWA_LIB to LD_LIBRARY_PATH" >&2
			pathprepend $SCHWA_LIB LD_LIBRARY_PATH
	else
			echo "Not adding $SCHWA_LIB to LD_LIBRARY_PATH (missing directory)" >&2
	fi

6: Test in interpreter

	wradford@ch6:~$ python
	Python 2.6.5 (r265:79063, Apr 16 2010, 13:57:41)
	[GCC 4.4.3] on linux2
	Type "help", "copyright", "credits" or "license" for more information.
	>>> from schwa import dr
	>>> from schwa import tokenizer
