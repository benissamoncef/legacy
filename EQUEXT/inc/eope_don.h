/*E*/
/* Fichier : $Id: eope_don.h,v 1.3 1995/12/13 15:05:43 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/12/13 15:05:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOPE * FICHIER eope_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables globales au module TEOPE
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Fontaine.C    Version 1.2     03 Mai 1995     : Reprise entete
* Fontaine.C    Version 1.3     04 Dec 1995     : Correction etat operator
------------------------------------------------------ */

#ifndef eope_don
#define eope_don

/* fichiers inclus */

#include "xdy.h"
#include "ex_mdon.h"
#include "eope_cfg.h"
#include "eope_cmd.h"
#include "xzec.h"
#include "xzamc.h"
#include "xdg.h"





/* definitions des structures internes */
		
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
		   XDY_Entier		SocketMaitre;
		   void *		Suivant;
		}EOPE_DONNEES_OPER;

/* definition de macro exportees */

/* declarations de donnees globales */

extern EOPE_DONNEES_OPER		*pg_debutOPER ;
extern int				vg_Mode_Fonct;
extern XDY_NomMachine			vg_NomMachine;
extern XDY_NomSite        		vg_NomSite;


/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask ;
T_INT4			vg_SockRT;
EX_DECLA_INSERER   ( EOPE_DONNEES_OPER, ed_ins_liste_OPER )
EX_DECLA_SUPPRIMER ( ed_sup_liste_OPER )	

#endif
