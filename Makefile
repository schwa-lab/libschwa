include Makefile.release
include Makefile.$(shell uname -s | tr '[:upper:]' '[:lower:]')
include Makefile.targets
-include Makefile.depends
