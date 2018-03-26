#ifndef HANDLERS_H
#define HANDLERS_H

extern void sirc_ping();
extern void sirc_join();
extern void sirc_quit();
extern void sirc_part();
extern void sirc_error();

extern void priv_help();
extern void priv_hello();
extern void priv_pass();

extern void ctcp_chat();
extern void ctcp_version();
extern void ctcp_source();
extern void ctcp_ping();

#endif
