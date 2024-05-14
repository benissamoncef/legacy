/*E*/
/* Fichier : $Id: xzao187.h,v 1.4 2018/11/07 10:39:26 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/07 10:39:26 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao187.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	05 Nov 1996	: Creation
* JMG   31/10/07        : ajout site de gestion 
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.3
* JPL	07/11/18 : Procédure dégroupée  1.4
------------------------------------------------------ */

#ifndef xzao187
#define xzao187

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO187_RPC_NAME "XZAO187"

#define XZAOC_XZAO187_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	Nom;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	Type;
   	XDY_NomEqt	NomServeur;
   	XDY_Rgs		Rgs;
   	XDY_Octet	LiaisonMaitre;
   	XDY_Octet	LiaisonEsclave;
   	XDY_Octet	Code;
        XDY_NomSite     NomSite;        
	XDY_Booleen	Supprime;

} XZAOT_ConfEqtGenLCR;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO187_Ajouter_Eqt_generique_LCR (	XDY_Basedd,
						XZAOT_ConfEqtGenLCR,
						XDY_ResConf * );



#endif
