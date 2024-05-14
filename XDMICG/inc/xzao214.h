/*E*/
/*Fichier : $Id: xzao214.h,v 1.4 1994/11/24 18:32:10 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/24 18:32:10 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao214.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	24 Nov 1994	: Modif type NomTunnel (v1.3)
* volcic	24 Nov 1994	: Modif entete (v1.4)
------------------------------------------------------ */

#ifndef xzao214
#define xzao214

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO214_RPC_NAME "XZAO;214"

#define XZAOC_XZAO214_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Mot		NumeroSegment;
	XDY_Mot		NumeroPortion;
	XDY_Octet	NumeroAutoroute;
	XDY_Entier	Coeftrans_debut;
	XDY_Entier	Coeftrans_fin;
	XDY_Sens	Sens;
	XDY_Octet	NombreVoies;
	XDY_Octet	LargeurBAU;
	XDY_Booleen	Tunnel;
	XDY_Nom		NomTunnel;

} XZAOT_Segment;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO214_Liste_Segments_Portion (	XDY_Basedd,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
