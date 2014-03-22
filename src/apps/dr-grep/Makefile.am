# =============================================================================
# Ragel
# =============================================================================
language_gen.cc: $(srcdir)/../../ragel/dr-grep/language.rl
	ragel -C -G2 -o $@ $<


# =============================================================================
# Automake
# =============================================================================
bin_PROGRAMS = dr-grep
dr_grep_CPPFLAGS = -I$(srcdir)/../../lib -I$(srcdir)/..
dr_grep_CXXFLAGS = -W -Wall -Wextra -Werror -pedantic
dr_grep_LDADD = ../../lib/libschwa.la
dr_grep_SOURCES = main.cc main.h language.cc language_gen.cc language.h
