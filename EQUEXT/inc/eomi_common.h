/*E Fichier : $Id: eomi_common.h,v 1.2 1995/05/22 13:15:22 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 13:15:22 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOMi * FICHIER eomi_common.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusion des fichiers generalement necessaires par inclision de elexi_common.h
* Declaration en extern des varaibles globales modules eomi_xxx.c.
* Fichier inclus ds. tous les mods. eomi_xxx.c sauf eomi_dir.c
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Mismer	Version 1.2	22 Mai 1995	: Modif gestion deconnexion ST
------------------------------------------------------ */

#ifndef eomi_common
#define eomi_common

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include <xdy.h>
#include <xdc.h>
#include <xdf.h>
#include <xdl.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzex.h>
#include <xzec.h>
#include <xzss.h>
#include <xzsm.h>
#include "ex_mcom.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* type configuration tache TEOMi */


/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK	*vg_SockList ;			/* liste globale des sockets */
extern XZEXT_ST_Config  vg_Config ;                     /* config tache TEOxi */
extern XZEXT_ST_Sockets	vg_DesSocks;			/* struct. descs. sockets typiques tache TEOxi */
extern XZEXT_Liste_Cmdes_LCR	*pg_ListeCmdesLCR;	/* liste cmdes. LCR empilees */
extern float            vg_val_timer ;                  /* time-out attente reponse */
extern XDY_Entier       vg_sock_TEOPE ;			/* socket acces TEOPE */
extern int		vg_EtatST;

/* declaration de fonctions externes */


/* declarations des structures internes */
		
typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_NomST		NumeroST;
   		   XDY_NoLS		NoLSMaitre;
		}EOPE_CONFIG_OPER;
		
typedef struct	{
		   EOPE_CONFIG_OPER 	Config;
   		   XDY_Mot		EtatLiaison;
   		   XDY_Entier		NoCdeCourante;
   		   XDY_Mot		CptRenduCde;
   		   XDY_Num_Oper		NumOperateur;
		   XDY_Msg_ES80		MessageEnvoye;
		   XDY_Entier		Etat_Service;
		   XDY_Entier		Etat_Activite;
		   XDY_Entier		SocketMaitre;
		   void *		Suivant;
		}EOPE_DONNEES_OPER;
		

/* FIN INTEGRATION */

#endif
