#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <linux/limits.h>
// libxml
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
// ZeroMQ
#include <zmq.h>

//#define DEBUG 1

//#ifndef NSS_SQLITE_H
//#define NSS_SQLITE_H

#define _GNU_SOURCE


#ifdef HAVE_CONFIG_H
	#include <config.h>
//#else
//	#error You would need to use autotools for building this.
#endif

#include <nss.h>
#include <syslog.h>
//#include <stdio.h>
#include <errno.h>
//#include "tool.h"
#include <malloc.h>
#include <string.h>

// Some syslog shortcuts
#ifdef DEBUG
	#define NSS_DEBUG(msg, ...) syslog(LOG_DEBUG, (msg), ## __VA_ARGS__)
#else
	//#define NSS_DEBUG(msg, ...)
	#define NSS_DEBUG(msg, ...) syslog(LOG_DEBUG, (msg), ## __VA_ARGS__)
#endif

#define NSS_ERROR(msg, ...) syslog(LOG_ERR, (msg), ## __VA_ARGS__)

//#define FALSE 0
//#define TRUE 1

#define executable "/etc/nss/exo/handler"

//#endif
