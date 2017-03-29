//#pragma once
#ifndef nss_exo_passwd_tool_h
#define nss_exo_passwd_tool_h

#include <nss.h>
#include <pwd.h>


enum nss_status nss_exo_user_get (const char * getter_type, const char */* getter_content*/, struct passwd */* result*/, char */* buffer*/, size_t/* buffer_size*/, int */* error*/);

#endif
