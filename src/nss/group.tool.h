//#pragma once
#ifndef nss_exo_group_tool_h
#define nss_exo_group_tool_h

#include <nss.h>
#include <grp.h>

enum nss_status nss_exo_tool_group_get (const char */* getter_type*/, const char */* getter_content*/, struct group */* result*/, char */* buffer*/, size_t/* buffer_size*/, int */* error*/);

#endif
