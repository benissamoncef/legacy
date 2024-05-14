/*E*/
/* Fichier : $Id: eimu_don.c,v 1.2 2021/05/04 13:25:03 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2021/05/04 13:25:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module decrivant les structures de donn≈es internes
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/011/20	: Creation DEM-SAE155 1.1
*
------------------------------------------------------*/

/* fichiers inclus */

#include "eimu_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eimu_don.c,v 1.2 2021/05/04 13:25:03 pc2dpdy Exp $ : eimu_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EIMU_DONNEES_IMU, ed_ins_liste_IMU, pg_debutIMU, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( EIMU_DONNEES_IMU, ed_sup_liste_IMU, pg_debutIMU, Config.Numero, Suivant)	

/* definition de fonctions externes */



