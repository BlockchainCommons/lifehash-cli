
# Package-specific substitution variables
package = lifehash
version = 0.1.0
tarname = lifehash

# Prefix-specific substitution variables
prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin

# VPATH-specific substitution variables
srcdir = .


COMPILER = g++
LDLIBS += -lpng -lz -llifehash -lstdc++ -lm

toolname = lifehash

.PHONY: all
all: $(toolname)

OBJS = \
   lifehash.o

$(toolname): $(OBJS)

lifehash.o: png-writer.hpp

.PHONY: clean
clean:
	rm -f $(toolname) *.o
	rm -rf *.dSYM

.PHONY: install
install: $(toolname)
	install -d $(bindir)
	install $(toolname) $(bindir)

.PHONY: uninstall
uninstall:
	rm -f $(bindir)/$(toolname)
	-rmdir $(bindir) >/dev/null 2>&1

.PHONY: distclean
distclean: clean
	rm -f Makefile
	rm -f config.h
