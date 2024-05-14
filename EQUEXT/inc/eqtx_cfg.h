/*E*/
/*  Fichier : $Id: eqtx_cfg.h,v 1.1 1996/12/05 18:59:46 orengo Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/12/05 18:59:46 $
------------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------------------------------------------
* MODULE TERAU * FICHIER eqtx_cfg.h
------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de config de la tache TEQTX
*
------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	07 Nov 1996	: Creation
* Orengo.A	Version 1.2	05 Nov 1996	: Changement du nom des equipements
*
----------------------------------------------------------------------------------------*/

/* Declarations de donnees globales */
/* -------------------------------- */

extern EQTX_DONNEES_SYSTQTX vg_donnees_systeme_qtx;

/* Declaration de fonctions externes */
/* --------------------------------- */

void ec_controle( int, XDY_NomMachine );
int ec_init( XDY_NomMachine );
extern void ec_sortir( void);
