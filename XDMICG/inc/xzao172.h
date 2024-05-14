/*E*/
/*  Fichier : $Id: xzao172.h,v 1.1 1994/10/05 19:12:50 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:50 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao172.h
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

#ifndef xzao172
#define xzao172

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO172_RPC_NAME "XZAO;172"

#define XZAOC_XZAO172_NB_PARM_RETURN 1

/* definitions de types exportes */
		
typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_NomEqt	NomEch;
	XDY_Mot		Adresse;
	XDY_Octet	BitComm;
	XDY_Octet	BitDiscordance;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqAutEch;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO172_Ajouter_SeqAut_Echangeur (	XDY_Basedd,
						XZAOT_ConfSeqAutEch,
						XDY_ResConf *);


#endif
