#ifndef SAY_H
#define SAY_H
#include <stdio.h>

void say_file_to();
void say_peer_ln();
void say_peer_str();
void say_rs();
void say_thru_rs();

void say_party_ln();
void say_party_str();
void say_2peer_str();

#define say_peer_str1(s1) \
say_peer_str((s1),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_peer_str2(s1,s2) \
say_peer_str((s1),(s2),NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_peer_str3(s1,s2,s3) \
say_peer_str((s1),(s2),(s3),NULL,NULL,NULL,NULL,NULL,NULL)
#define say_peer_str4(s1,s2,s3,s4) \
say_peer_str((s1),(s2),(s3),(s4),NULL,NULL,NULL,NULL,NULL)
#define say_peer_str5(s1,s2,s3,s4,s5) \
say_peer_str((s1),(s2),(s3),(s4),(s5),NULL,NULL,NULL,NULL)
#define say_peer_str6(s1,s2,s3,s4,s5,s6) \
say_peer_str((s1),(s2),(s3),(s4),(s5),(s6),NULL,NULL,NULL)
#define say_peer_str7(s1,s2,s3,s4,s5,s6,s7) \
say_peer_str((s1),(s2),(s3),(s4),(s5),(s6),(s7),NULL,NULL)
#define say_peer_str8(s1,s2,s3,s4,s5,s6,s7,s8) \
say_peer_str((s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),NULL)
#define say_peer_str9(s1,s2,s3,s4,s5,s6,s7,s8,s9) \
say_peer_str((s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9))

#define say_2peer_str1(p,s1) \
say_2peer_str((p),(s1),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str2(p,s1,s2) \
say_2peer_str((p),(s1),(s2),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str3(p,s1,s2,s3) \
say_2peer_str((p),(s1),(s2),(s3),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str4(p,s1,s2,s3,s4) \
say_2peer_str((p),(s1),(s2),(s3),(s4),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str5(p,s1,s2,s3,s4,s5) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str6(p,s1,s2,s3,s4,s5,s6) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str7(p,s1,s2,s3,s4,s5,s6,s7) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str8(p,s1,s2,s3,s4,s5,s6,s7,s8) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),NULL,NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str9(p,s1,s2,s3,s4,s5,s6,s7,s8,s9) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9),NULL,NULL,NULL,NULL,NULL)
#define say_2peer_str10(p,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9),(s10),NULL,NULL,NULL,NULL)
#define say_2peer_str11(p,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9),(s10), \
(s11),NULL,NULL,NULL)
#define say_2peer_str12(p,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9),(s10), \
(s11),(s12),NULL,NULL)
#define say_2peer_str13(p,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9),(s10), \
(s11),(s12),(s13),NULL)
#define say_2peer_str14(p,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14) \
say_2peer_str((p),(s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8),(s9),(s10), \
(s11),(s12),(s13),(s14))

#define say_rs1(s1) \
say_rs((s1),NULL,NULL,NULL,NULL,NULL,NULL,NULL)
#define say_rs2(s1,s2) \
say_rs((s1),(s2),NULL,NULL,NULL,NULL,NULL,NULL)
#define say_rs3(s1,s2,s3) \
say_rs((s1),(s2),(s3),NULL,NULL,NULL,NULL,NULL)
#define say_rs4(s1,s2,s3,s4) \
say_rs((s1),(s2),(s3),(s4),NULL,NULL,NULL,NULL)
#define say_rs5(s1,s2,s3,s4,s5) \
say_rs((s1),(s2),(s3),(s4),(s5),NULL,NULL,NULL)
#define say_rs6(s1,s2,s3,s4,s5,s6) \
say_rs((s1),(s2),(s3),(s4),(s5),(s6),NULL,NULL)
#define say_rs7(s1,s2,s3,s4,s5,s6,s7) \
say_rs((s1),(s2),(s3),(s4),(s5),(s6),(s7),NULL)
#define say_rs8(s1,s2,s3,s4,s5,s6,s7,s8) \
say_rs((s1),(s2),(s3),(s4),(s5),(s6),(s7),(s8))

#endif
