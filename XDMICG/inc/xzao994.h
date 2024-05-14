/*E*/
/*Fichier :  $Id: xzao994.h,v 1.2 2009/10/22 15:56:07 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/10/22 15:56:07 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao994.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ   	15 Nov 2007	: Creation DEM 662
* JMG		02/10/09	: ajout nature DEM895 
------------------------------------------------------ */

#ifndef xzao994
#define xzao994

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO994_RPC_NAME "XZAO;994"

#define XZAOC_XZAO994_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_Booleen	Creation;
	XDY_Octet	Nature;
	
} XZAOT_ConfSeqSIG;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO994_Ajouter_Seq_SIG ( XDY_Basedd,
						             XZAOT_ConfSeqSIG,
						             XDY_ResConf *);



#endif
