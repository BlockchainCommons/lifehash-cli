#
# Regular cron jobs for the lifehash-cli package.
#
0 4	* * *	root	[ -x /usr/bin/lifehash-cli_maintenance ] && /usr/bin/lifehash-cli_maintenance
