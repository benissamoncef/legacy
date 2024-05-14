/* Fichier : amisol_tcpip.h   Release : 1.0   Date : 06/02    *
*************************************************
* Elcima Informatique
*************************************************
* MODULE  : BASEDD                              
* FICHIER : amisol_tcpip.h                       
*************************************************
* DESCRIPTION :
* fichier tache tami2 contient les principales fct
* tcpip, management de l'echange
*************************************************
* HISTORIQUE :
*
************************************************/

#ifndef TRANSFERT_TCPIP
#define TRANSFERT_TCPIP

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>

/* include librairie socket */
#include        <sys/socket.h>
#include	<sys/types.h>

/*include conditionnel pour lie aux systems HP-UX*/
#ifdef _XOPEN_SOURCE_EXTENDED
#include  <arpa/inet.h>
#else
#include  <netinet/in.h>
#endif

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzia.h"


/* definition des constantes */

/* definition des variables globales a l'application */

/* declaration de fonctions internes */

/* definition de fonctions externes */

extern void itoa(int n, char s[]);
extern void reverse(char s[]);
extern void fill_array_to_size(char s[], int length_end);
extern void amisol_tcpip_construct_entete();
extern void amisol_tcpip_format_mesure(char s_source[], char s_dest[], char s_symbol[]);
extern int amisol_tcpip_construct_table_mesure(char *va_nom_station,int va_val_QT,int va_val_TT, int va_val_VT);
extern int amisol_free_ressources();
extern char * amisol_tcpip_get_heure();
extern int amisol_tcpip_connexion_init();
extern int amisol_tcpip_send(char *vl_mess);
extern int amisol_tcpip_receive(char *vl_buf);
extern void amisol_tcpip_connexion_close();
extern int amisol_tcpip_manage_header_message();
extern int amisol_tcpip_send_body_message();
extern int amisol_tcpip_manage_end_message();

#endif

