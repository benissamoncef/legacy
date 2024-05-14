/* Fichier : acet_evt.h   Release : 1.1   Date : 05/15/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acet_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evtic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans acet_evt.c
*************************************************
* HISTORIQUE :
*
* JMG	13/02/16	creation
************************************************/

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include 	<stdlib.h>
#include 	<malloc.h>
#include 	<string.h>
 
#include        "cstypes.h"
#include        "ctpublic.h"
#include "asql_com.h"

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzaec.h"
#include "xzao316.h"
#include "xzit.h"
#include "xzai500.h"
#include "xzac80.h" 

#define CO_DELAI_FREQUENCE	10
#define CO_FIC_FREQUENCE	"frequence.txt"

#define CO_FIC_ATTSTR		"att_evt_saga.txt"
#define	CO_MAX_CARACTERE	250
#define TAILLE 			400
#define TAILLE_BD		250


#define CO_INCONNU_I		-1
#define CO_INCONNU_D		-1.0

#define NBR_PREMIER 		53
#define NBR_ELT_TABLE		800

#define XDC_DATEX_RIEN		0
#define XDC_DATEX_FMC		1
#define XDC_DATEX_TFM           2

	typedef struct {
	char name[5];		
  	char valeur[250];
	} liste_att;
	
extern tg_Context    *XZAGV_Context;


   	extern int acet_evt_init();

	extern void acet_evt_ftp();
	
	extern void acet_evt_decode();

	extern void acet_evt_freemem();

