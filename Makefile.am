# =============================================================================
# Automake
# =============================================================================
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/third-party src/lib src/apps

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = dist/libschwa.pc

EXTRA_DIST = LICENCE doc src/ragel src/third-party/cityhash src/third-party/re2
dist-hook: clean-docs


# =============================================================================
# Custom targets.
# =============================================================================
.PHONY: clean-docs

clean-local: clean-docs

docs:
	doxygen doc/Doxyfile

clean-docs:
	rm -rf doc/html

wc:
	for d in src/lib src/apps; do \
		find $${d} \( -name "*.cc" -or -name "*.h" \) | egrep -v '_(gen|test)\.cc' | xargs wc -l; \
	done
