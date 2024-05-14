/*E*/
/* Fichier : $Id: etdp_pma.h,v 1.1 2013/01/25 10:11:23 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/01/25 10:11:23 $
-----------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-----------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-----------------------------------------------------
* MODULE TETDP * FICHIER etdp_cfg.h
-----------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de config de la tache TETDP
*
---------------------------------------------------------------
* HISTORIQUE :
*
* MG	04/06/12 : Creation (DEM/1014 PMA)
---------------------------------------------------------------*/

/* Declarations de donnees globales */
/* -------------------------------- */

/* Declaration de fonctions externes */
/* --------------------------------- */

extern int et_calcul_temps_de_parcours_PMVA ( XDY_Eqt, XDY_Eqt, XDY_TDP * , char *);
extern int et_Rafraichissment_PMVA ( void );
extern int et_Recherche_zone_PMVA ( XDY_Autoroute, XDY_PR, XDY_Sens, XDY_Eqt, XDY_Eqt );
