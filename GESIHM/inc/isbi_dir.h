/*E Fichier : $Id: isbi_dir.h,v 1.3 2021/01/29 09:09:12 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2021/01/29 09:09:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSMA * FICHIER isyn_sma.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module  directeur de la tache TISYN
*   
*
------------------------------------------------------
* HISTORIQUE :
* Niepceron	24 Nov 1994	: Creation
* ABE 		26/01/20 : 	Ajout gestion fichier de config TOPS, pour IP:DISPLAY.PORT pour l'IHM DEM-SAE228 V1.2
------------------------------------------------------ */

#ifndef MSMA
#define MSMA

/* fichiers inclus */
#include "xzst.h"
#include "xzsc.h"
#include "xdy.h"

//#include "isyn_sra_tri.h"
//#include "isyn_svu_applix.h"
//#include "isyn_sco.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XZSCT_NomMachine vm_NomMachine;
XZSCT_NomSite    vm_NomSite;

XDY_Booleen vm_cx;

int vm_sockfd, vm_ElfFd; /* Sockets pour RPC venant d'applix */

/* Fichier de configuration TOPS */
								
#define CO_FIC_TOPS_GEN_PATH "/produits/migrazur/appliSD/fichiers/adm/TOPS_"
#define CO_FIC_TOPS_DISPLAY1 "display1"
#define CO_FIC_TOPS_DISPLAY2 "display2"

/* delaration de fonctions externes */

#endif
