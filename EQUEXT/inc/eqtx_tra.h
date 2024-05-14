/*E*/
/*  Fichier : $Id: eqtx_tra.h,v 1.2 1997/01/30 13:09:12 mismer Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/01/30 13:09:12 $
------------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------------------------------------------
* MODULE TERAU * FICHIER eqtx_tra.h
------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de traitement de la tache TEQTX
*
------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	07 Nov 1996	: Creation
* Mismer.D	version 1.2	13 Jan 1997	: Modification protocole (DEM/1393)
*
----------------------------------------------------------------------------------------*/

/* Declaration de fonctions externes */
/* --------------------------------- */

extern int em_traitement_trame_maitre( int, int );
extern int em_traitement_trame_esclave( int, int );
extern int em_creation_fichier_donnees_meteo_invalides( int );
extern int em_envoi_fichier_vers_Base_Donnees( char * );
