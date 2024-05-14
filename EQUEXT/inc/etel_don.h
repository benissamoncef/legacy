/*E*/
/* Fichier : @(#)etel_don.h	1.3      Release : 1.3        Date : 06/20/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusion des fichiers generalement necessaires 
* Declaration en extern des variables globales modules etel_xxx.c
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Fontaine.C    Version 1.2     04 Mai 1995     : Reprise entete
* Mismer.D      Version 1.3     14 Jun 1995     : Ajout verification inhibition MAA
* Diaz.L 	Version 1.4	09 Oct 1998     : Ajout PAB
* SFO   	06-10-2014 : modif pour compilation linux
------------------------------------------------------ */

#ifndef etel_don
#define etel_don

/* fichiers inclus */

#include "xdy.h"
#include "ex_mdon.h"
#include "etel_cfg.h"
#include "etel_cmd.h"
#include "xzec.h"
#include "xzamc.h"
#include "xdg.h"
#ifdef _HPUX_SOURCE
#include <cstadrv.h>
#endif




/* definitions des structures internes */
	
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
		   XDY_Entier		SocketMaitre;
		   void *		Suivant;
		}ETEL_DONNEES_MAA;
		
/* definition de macro exportees */

/* declarations de donnees globales */

extern ETEL_DONNEES_AUTOTEL		*pg_debutAUTOTEL ;
extern ETEL_DONNEES_MAA			*pg_debutMAA ;
extern int				vg_Mode_Fonct;
extern XDY_NomMachine			vg_NomMachine;
extern XDY_NomSite        		vg_NomSite;
#ifdef _HPUX_SOURCE
extern PbxLink				vg_Sock_Alcatel;
extern PbxLink				vg_Alcatel;
#endif
extern int				vg_Mac_PC2;
extern char				vg_Numero_PO[4];

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask ;
T_INT4			vg_SockRT;
EX_DECLA_INSERER   ( ETEL_DONNEES_AUTOTEL, ed_ins_liste_AUTOTEL )
EX_DECLA_SUPPRIMER ( ed_sup_liste_AUTOTEL )	
EX_DECLA_INSERER   ( ETEL_DONNEES_MAA, ed_ins_liste_MAA )
EX_DECLA_SUPPRIMER ( ed_sup_liste_MAA )	

#endif
