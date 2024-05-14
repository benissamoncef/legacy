/*E*/
/*  Fichier : $Id: xzao405.h,v 1.2 1997/06/06 13:46:56 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/06/06 13:46:56 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao405.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       10 Fev 1997     : Creation (DEM/1395)
* Mismer       06 Jun 1997     : Modif taille message TFM v1.2 (DEM/1440)
------------------------------------------------------ */

#ifndef xzao405
#define xzao405

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO405_RPC_NAME "XZAO;405"

#define XZAOC_XZAO405_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumDest;
	XDY_NomAuto	Autoroute;
        XDY_PR          PR;
	XDY_Sens	Sens;
	XDY_Ligne_PMV   Libelle_PMV;
        char            Libelle_TFM[60];
        XDY_Booleen     Type_Localisation;
        XDY_Booleen	Inhibition;

} XZAOT_ConfDestTDP;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO405_Ajouter_Destination_TDP (	XDY_Basedd,
					XZAOT_ConfDestTDP,
					XDY_ResConf * );



#endif
