/* Fichier : $Id: xzsg.h,v 1.1 1994/09/30 15:16:45 milleville Exp $	Release : $Revision: 1.1 $        Date : $Date: 1994/09/30 15:16:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE * FICHIER xzsg.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les librairies de mise en place
* et d'arret des tests reseaux pour une machine donnee.
*
******************************************************
* HISTORIQUE :
*
* T.MILLEVILLE 30 Sep 1994  : Creation
*
******************************************************/


/* fichiers inclus */



/* declaration des constantes */
#define XZSGC_RES_HS 0
#define XZSGC_RES_OK 1



/* declaration de fonctions externes */
extern int XZSG_00InitServices() ;
extern int XZSG_01SurveillerReseau(T_STR pa_NomMachine) ;
extern int XZSG_02ArretSurveiller(T_STR pa_NomMachine) ;
