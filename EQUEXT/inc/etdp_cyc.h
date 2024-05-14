/*E*/
/*  Fichier : $Id: etdp_cyc.h,v 1.3 2011/06/23 09:57:47 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2011/06/23 09:57:47 $
-------------------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER eqtx_cyc.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache TETDP
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* LCL		version 1.3	22/06/11	: TDP A8
-----------------------------------------------------------------------------------------*/


/* Declaration de fonctions externes */
/* --------------------------------- */

extern int ey_traitement_cyclique ( int );
extern int ey_envoi_demande_donnees_horaire ( XDY_Eqt );

extern XZEZT_Liste_Zdp *                pg_ListeZone;

