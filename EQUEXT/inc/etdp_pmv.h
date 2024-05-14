/*E*/
/*  Fichier : $Id: etdp_pmv.h,v 1.4 2012/02/14 14:30:49 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2012/02/14 14:30:49 $
------------------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_cfg.h
------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de config de la tache TETDP
*
------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.3	05 Jun 1997	: Ajout alerte sur panne zone (DEM/1447)
* JMG		version 1.4	14/02/12	: tdp etendu dem/1014
----------------------------------------------------------------------------------------*/

/* Declarations de donnees globales */
/* -------------------------------- */


/* Declaration de fonctions externes */
/* --------------------------------- */

extern int et_calcul_temps_de_parcours ( XDY_Eqt, 
				  	 XDY_Eqt,
				  	 XDY_TDP *, char * );
extern int et_Rafraichissment_PMV ( void );
extern int et_Recherche_zone_PMV ( 	XDY_Autoroute,
					XDY_PR,
					XDY_Sens,
					XDY_Eqt,
					XDY_Eqt );
