/*E*/
/*  Fichier : $Id: xzao170.h,v 1.1 1994/10/05 19:12:48 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:48 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao170.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao170
#define xzao170

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO170_RPC_NAME "XZAO;170"

#define XZAOC_XZAO170_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_NomEqt	NomTube;
	XDY_Mot		Adresse;
	XDY_Octet	BitComm;
	XDY_Octet	BitDiscordance;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqAutTun;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO170_Ajouter_SeqAut_Tunnel (	XDY_Basedd,
						XZAOT_ConfSeqAutTun,
						XDY_ResConf *);



#endif
