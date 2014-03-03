#include "configargs.h"

static char basever[] = "4.6.2";
static char datestamp[] = "20120613";
static char devphase[] = "FSL release r704";
static char revision[] = "[ARM/embedded-4_6-branch revision 188521]";

/* FIXME plugins: We should make the version information more precise.
   One way to do is to add a checksum. */

static struct plugin_gcc_version gcc_version = {basever, datestamp,
						devphase, revision,
						configuration_arguments};
