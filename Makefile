# Package-specific substitution variables
package = lifehash
version = 0.1.0

# Prefix-specific substitution variables
prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin

# VPATH-specific substitution variables
srcdir = .


.PHONY: all clean check lint
all clean check:
	cd src && $(MAKE) $@

.PHONY: lifehash
lifehash:
	cd src && $(MAKE) $@

#.PHONY: install uninstall
#install uninstall:
#	cd src && $(MAKE) $@

#config.status: configure
#	./config.status --recheck

#.PHONY: distclean
#distclean:
#	cd src && $(MAKE) $@
#	rm -f Makefile
#	rm -rf autom4te.cache
#	rm -f autoscan.log
#	rm -f configure.scan configure.status config.log
#	rm -f config.log config.status config.h
#	rm -rf deps/*
#	rm -rf sysroot
