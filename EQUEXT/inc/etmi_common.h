/*E Fichier : $Id: etmi_common.h,v 1.3 1995/05/22 13:15:25 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/22 13:15:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_common.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusion des fichiers generalement necessaires par inclision de elexi_common.h
* Declaration en extern des varaibles globales modules etmi_xxx.c.
* Fichier inclus ds. tous les mods. etmi_xxx.c sauf etmi_dir.c
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     03 Mai 1995     : Reprise entete
* Mismer	Version 1.3	22 Mai 1995	: Modif gestion deconnexion ST
------------------------------------------------------ */

#ifndef etmi_common
#define etmi_common

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include        <rtworks/common.h>
#include        <rtworks/ipc.h>

#include "xdy.h"
#include "xdc.h"
#include "xdf.h"
#include "xdl.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzex.h"
#include "xzec.h"
#include "xzss.h"
#include "xzsm.h"
#include "xzamc.h"
#include "ex_mcom.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* type configuration tache TETMi */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK	*vg_SockList ;			/* liste globale des sockets */
extern XZEXT_ST_Config    vg_Config ;                     /* config tache TETxi */
extern XZEXT_ST_Sockets	vg_DesSocks;		/* struct. descs. sockets typiques tache TELxi */
extern XZEXT_Liste_Cmdes_LCR	*pg_ListeCmdesLCR;	/* liste cmdes. LCR empilees */
extern float            vg_val_timer ;                  /* time-out attente reponse */
extern XDY_Entier       vg_sock_TETEL ;			/* socket acces TETEL */
extern int		vg_EtatST;

/* declaration de fonctions externes */


/* declarations des structures internes */
		
typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_NomST		NumeroST;
   		   XDY_Rgs		Adr;
   		   XDY_Entier		MaxNumTel;
   		   XDY_Entier		TaillePrefixe;
   		   XDY_Prefixe		CodePrefixe;
   		   XDY_Renvoi		CodeRenvoi;
   		   XDY_Entier		TpsAttente;
   		   XDY_Entier		ModeEcho;
   		   XDY_Entier		CodeRetour;
   		   XDY_Entier		FormatCodeRetour;
   		   }ETEL_CONFIG_AUTOTEL;
		
typedef struct	{
		   ETEL_CONFIG_AUTOTEL	Config;
	           void *		Suivant;
		}ETEL_DONNEES_AUTOTEL;
		
typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_NomST		NumeroST;
   		   XDY_Operateur	NumeroPO;
   		   XDY_NoLS		NoLSMaitre;
   		   XDY_Rgs		AdrAUTOTEL;
   		   XDY_Tel		RepAssoc;
		}ETEL_CONFIG_MAA;
		
typedef struct	{
		   ETEL_CONFIG_MAA 	Config;
   		   XDY_Mot		EtatLiaison;
   		   XDY_Entier		NoCdeCourante;
   		   XDY_Tel		NoTelAppele;
   		   XDY_Entier		RenvoiSurRepondeur;
   		   XDY_Mot		CptRenduCde;
		   XDY_Entier		Etat_Service;
		   XDY_Entier		Etat_Activite;
		   XDY_Entier		SocketMaitre;
		   void *		Suivant;
		}ETEL_DONNEES_MAA;
		
#endif
