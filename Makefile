SHELL = /bin/sh

default: dansarina connect ln+512 base64 plugger

clean: 
	rm -f *.o systype choose compile load *.a makelib dansarina \
	connect ln+512 socket.lib base64

compile: \
warn-auto.sh conf-cc
	( cat warn-auto.sh; \
	echo exec "`head -1 conf-cc`" '-c $${1+"$$@"}' \
	) > compile
	chmod 755 compile

choose: \
warn-auto.sh choose.sh conf-home
	cat warn-auto.sh choose.sh > choose
	chmod 755 choose

load: \
warn-auto.sh conf-ld
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo exec "`head -1 conf-ld`" \
	'-o "$$main" "$$main".o $${1+"$$@"}' \
	) > load
	chmod 755 load

makelib: \
warn-auto.sh systype
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo 'rm -f "$$main"'; \
	echo 'ar cr "$$main" $${1+"$$@"}'; \
	case "`cat systype`" in \
	sunos-5.*) ;; \
	unix_sv*) ;; \
	irix64-*) ;; \
	irix-*) ;; \
	dgux-*) ;; \
	hp-ux-*) ;; \
	sco*) ;; \
	*) echo 'ranlib "$$main"' ;; \
	esac \
	) > makelib
	chmod 755 makelib

systype: \
find-systype.sh conf-cc conf-ld trycpp.c
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\'; \
	cat find-systype.sh; \
	) | sh > systype

socket.lib: \
trylsock.c compile load
	( ( ./compile trylsock.c && \
        ./load trylsock -lsocket ) >/dev/null 2>&1 \
        && echo -lsocket || exit 0 ) > socket.lib
	rm -f trylsock.o trylsock

conf.a: \
makelib conf.h rw.h strerr.h open.h getln.h io.h conf_retr.o conf_trunc.o \
conf_append.o
	./makelib conf.a conf_retr.o conf_trunc.o conf_append.o

conf_retr.o: \
compile conf.h conf_retr.c
	./compile conf_retr.c

conf_trunc.o: \
compile conf.h conf_trunc.c
	./compile conf_trunc.c

conf_append.o: \
compile conf.h conf_append.c
	./compile conf_append.c

db.a: \
makelib db.h db_open.o db_write.o db_read.o db_creat.o db_unlock.o \
db_close.o db_perm.o
	./makelib db.a db_open.o db_write.o db_read.o db_creat.o db_unlock.o \
	db_close.o db_perm.o

db_open.o: \
compile db.h db_open.c
	./compile db_open.c

db_write.o: \
compile db.h db_write.c open.h
	./compile db_write.c

db_read.o: \
compile db.h db_read.c open.h
	./compile db_read.c

db_creat.o: \
compile db.h db_creat.c
	./compile db_creat.c

db_unlock.o: \
compile db.h db_unlock.c
	./compile db_unlock.c

db_close.o: \
compile db.h db_close.c
	./compile db_close.c

db_perm.o: \
compile db.h db_perm.c
	./compile db_perm.c

open.a: \
makelib open_read.o open_write.o open_trunc.o open_excl.o
	./makelib open.a open_read.o open_write.o open_trunc.o open_excl.o

open_trunc.o: \
compile open.h open_trunc.c
	./compile open_trunc.c

open_excl.o: \
compile open.h open_excl.c
	./compile open_excl.c

open_read.o: \
compile open.h open_read.c
	./compile open_read.c

open_write.o: \
compile open.h open_write.c
	./compile open_write.c

open_append.o: \
compile open.h open_append.c
	./compile open_append.c

io.a: \
makelib io_set.o io_out.o io_012.o io_in.o
	./makelib io.a io_set.o io_out.o io_012.o io_in.o

io_in.o: \
compile io.h byte.h 
	./compile io_in.c

io_012.o: \
compile io_012.c rw.h io.h
	./compile io_012.c

io_out.o: \
compile io_out.c byte.h io.h
	./compile io_out.c

io_set.o: \
compile io.h io_set.c
	./compile io_set.c

error.a: \
makelib strerr_die.o strerr_sys.o
	./makelib error.a strerr_die.o strerr_sys.o

byte.a: \
makelib byte_copy.o byte_zero.o byte_copy_r.o byte_ndx.o byte_cmp.o \
byte_cnt.o byte_2upper.o
	./makelib byte.a byte_copy.o byte_zero.o byte_copy_r.o byte_ndx.o \
	byte_cmp.o byte_cnt.o byte_2upper.o

byte_copy.o: \
compile byte.h byte_copy.c
	./compile byte_copy.c

byte_ndx.o: \
compile byte.h byte_ndx.c
	./compile byte_ndx.c

byte_cmp.o: \
compile byte.h byte_cmp.c
	./compile byte_cmp.c

byte_zero.o: \
compile byte.h byte_zero.c
	./compile byte_zero.c

byte_copy_r.o: \
compile byte.h byte_copy_r.c
	./compile byte_copy_r.c

byte_cnt.o: \
compile byte.h byte_cnt.c
	./compile byte_cnt.c

byte_2upper.o: \
compile byte.h byte_2upper.c
	./compile byte_2upper.c

str.a: \
makelib str.h str_alloc.o str_unalloc.o str_put.o str_len.o str_cat.o \
str_zero.o str_cmp.o
	./makelib str.a str_alloc.o str_unalloc.o str_put.o str_len.o \
	str_cat.o str_zero.o str_cmp.o

str_alloc.o: \
compile str.h str_alloc.c
	./compile str_alloc.c

str_unalloc.o: \
compile str.h str_unalloc.c
	./compile str_unalloc.c

str_put.o: \
compile str.h str_put.c
	./compile str_put.c

str_len.o: \
compile str.h str_len.c
	./compile str_len.c

str_cat.o: \
compile str.h str_cat.c
	./compile str_cat.c

str_zero.o: \
compile str.h str_zero.c
	./compile str_zero.c

str_pad.o: \
compile str.h str_pad.c
	./compile str_pad.c

str_cmp.o: \
compile str.h str_cmp.c
	./compile str_cmp.c

getln.a: \
makelib getln.o
	./makelib getln.a getln.o

ndelay.a: \
makelib ndelay_on.o ndelay_off.o
	./makelib ndelay.a ndelay_on.o ndelay_off.o

ndelay_off.o: \
compile ndelay_off.c ndelay.h
	./compile ndelay_off.c

ndelay_on.o: \
compile ndelay_on.c ndelay.h
	./compile ndelay_on.c

scan.a: \
makelib scan.h scan_ulong.o scan_word.o scan_uint32.o scan_pass.o scan_line.o
	./makelib scan.a scan_ulong.o scan_word.o scan_uint32.o scan_pass.o \
	scan_line.o

scan_ulong.o: \
compile scan.h scan_ulong.c
	./compile scan_ulong.c

scan_word.o: \
compile scan.h scan_word.c
	./compile scan_word.c

scan_uint32.o: \
compile scan.h scan_uint32.c
	./compile scan_uint32.c

scan_pass.o: \
compile scan.h scan_pass.c
	./compile scan_pass.c

scan_line.o: \
compile scan.h scan_line.c
	./compile scan_line.c

fmt.a: \
makelib fmt_ulong.o fmt_uint32.o
	./makelib fmt.a fmt_ulong.o fmt_uint32.o

fmt_ulong.o: \
compile fmt.h fmt_ulong.c
	./compile fmt_ulong.c

fmt_str.o: \
compile fmt.h fmt_str.c
	./compile fmt_str.c

socket.a: \
makelib socket_tcp.o socket_bind.o socket_accept.o pollwait.o \
socket_connect_host.o ip4_scan.o socket_connect_timeout.o ip4_fmt.o \
polltimeout.o socket_connect_ndelay.o
	./makelib socket.a socket_tcp.o socket_bind.o socket_accept.o \
	pollwait.o ip4_scan.o socket_connect_host.o socket_connect_timeout.o \
	ip4_fmt.o polltimeout.o socket_connect_ndelay.o

ip4_scan.o: \
compile ip4.h ip4_scan.c
	./compile ip4_scan.c

ip4_fmt.o: \
compile ip4.h ip4_fmt.c
	./compile ip4_fmt.c

socket_tcp.o: \
compile socket.h socket_tcp.c
	./compile socket_tcp.c

socket_bind.o: \
compile socket.h socket_bind.c
	./compile socket_bind.c

socket_accept.o: \
compile socket.h socket_accept.c
	./compile socket_accept.c

socket_connect_host.o: \
compile socket.h socket_connect_host.c
	./compile socket_connect_host.c

socket_connect_ulong.o: \
compile socket.h socket_connect_ulong.c
	./compile socket_connect_ulong.c

socket_connect_timeout.o: \
compile socket.h socket_connect_timeout.c
	./compile socket_connect_timeout.c

polltimeout.o: \
compile poll.h polltimeout.c
	./compile polltimeout.c

pollwait.o: \
compile poll.h pollwait.c
	./compile pollwait.c

socket_connect_ndelay.o: \
compile socket.h socket_connect_ndelay.c
	./compile socket_connect_ndelay.c

peer.a: \
makelib peer_init.o peer_getln.o peer_attach.o peer_detach.o peer_goodbye.o \
peer_status.o peer_auth.o
	./makelib peer.a peer_init.o peer_getln.o peer_attach.o \
	peer_detach.o peer_goodbye.o peer_status.o peer_auth.o

peer_auth.o: \
compile peer.h peer_auth.c
	./compile peer_auth.c

peer_status.o: \
compile peer.h io.h peer_status.c
	./compile peer_status.c

peer_goodbye.o: \
compile peer.h io.h rw.h peer_goodbye.c
	./compile peer_goodbye.c

peer_init.o: \
compile peer.h peer_init.c
	./compile peer_init.c

peer_getln.o: \
compile peer.h peer_getln.c
	./compile peer_getln.c

peer_detach.o: \
compile peer.h peer_detach.c
	./compile peer_detach.c

peer_attach.o: \
compile peer.h peer_attach.c
	./compile peer_attach.c

sig.a: \
makelib sig.h sig_catch.o sig_pipe.o
	./makelib sig.a sig_catch.o sig_pipe.o

sig_catch.o: \
compile sig.h sig_catch.c
	./compile sig_catch.c

sig_pipe.o: \
compile sig.h sig_pipe.c
	./compile sig_pipe.c

getln.o: \
compile getln.h getln.c
	./compile getln.c

fmt_uint32.o: \
compile fmt_uint32.c fmt.h
	./compile fmt_uint32.c

fmt_int64.o: \
compile fmt_int64.c fmt.h
	./compile fmt_int64.c

strerr_die.o: \
compile strerr_die.c io.h exit.h strerr.h
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c io.h exit.h strerr.h
	./compile strerr_sys.c

dansarina: \
load dansarina.o party.o events.o handlers.o breakup.o say.o user.o \
db.a socket.a io.a getln.a byte.a str.a ndelay.a peer.a error.a scan.a \
fmt.a getln.a open.a sig.a fmt.a conf.a socket.lib key.o b64.o log.o
	./load dansarina events.o handlers.o breakup.o say.o \
	user.o party.o key.o b64.o log.o \
	db.a socket.a getln.a io.a byte.a str.a ndelay.a peer.a error.a \
	scan.a fmt.a getln.a open.a sig.a fmt.a conf.a `cat socket.lib`
	chmod 0700 usr/

dansarina.o: \
compile lim.h io.h peer.h strerr.h socket.h poll.h exit.h \
byte.h ndelay.h events.h say.h sig.h handlers.h db.h breakup.h user.h \
open.h fmt.h scan.h dansarina.c conf.h log.h
	./compile dansarina.c

events.o: \
compile events.c events.h io.h byte.h str.h handlers.h breakup.h
	./compile events.c

handlers.o: \
compile  breakup.h io.h peer.h exit.h handlers.h handlers.c
	./compile handlers.c

breakup.o: \
compile breakup.h breakup.c 
	./compile breakup.c

say.o: \
compile say.h say.c
	./compile say.c

user.o: \
compile user.h breakup.h db.h fmt.h str.h user.c
	./compile user.c

party.o: \
compile party.h party.c
	./compile party.c

key.o: \
compile key.h key.c
	./compile key.c

b64.o: \
compile b64.h b64.c
	./compile b64.c

# plugin.o: \
# compile plugin.h plugin.c
# 	./compile plugin.c
#
# plugin_exist.o: \
# compile plugin.h plugin_exist.c
# 	./compile plugin_exist.c
#
# plugin_argv.o: \
# compile plugin.h plugin_argv.c
# 	./compile plugin_argv.c
#
# plugin_path.o: \
# compile plugin.h plugin_path.c
# 	./compile plugin_path.c

plugger.o: \
compile plugger.c
	./compile plugger.c

plugger: \
load plugger.o getln.a io.a sig.a byte.a socket.a str.a scan.a error.a
	./load plugger getln.a error.a io.a sig.a byte.a socket.a str.a \
	scan.a

# test programs below

connect.o: \
compile connect.c 
	./compile connect.c

connect: \
load connect.o io.a str.a socket.a byte.a error.a ndelay.a socket.lib
	./load connect socket.a error.a ndelay.a io.a byte.a str.a \
	`cat socket.lib`

ln+512.o: \
compile ln+512.c 
	./compile ln+512.c

ln+512: \
load ln+512.o io.a str.a socket.a byte.a error.a ndelay.a socket.lib
	./load ln+512 socket.a error.a ndelay.a io.a byte.a str.a \
	`cat socket.lib`

base64.o: \
compile b64.h byte.h io.h base64.c
	./compile base64.c

base64: \
load b64.o io.a str.a byte.a base64.o
	./load base64 b64.o io.a str.a byte.a

log.o: \
log.c log.h strerr.h
	./compile log.c