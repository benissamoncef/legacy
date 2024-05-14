/* Fichier : $Id: agen_rdt.h,v 1.1 2008/11/17 11:29:39 pc2dpdy Exp $ Release : $Revision: 1.1 $   Date : $Date: 2008/11/17 11:29:39 $ *
B
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : agen_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evtic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans agen_evt.c
*************************************************
* HISTORIQUE :
*
* JMG	11/10/08 creation DEM/185
************************************************/

#ifndef agen_evt
#define agen_evt

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include 	<stdlib.h>

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzia.h"
#include "xzlg.h"

extern int agen_rdt_init();

#endif
