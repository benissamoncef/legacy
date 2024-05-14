/*E*/
/* Fichier : $Id: etim_ins.h,v 1.2 1995/05/23 12:49:56 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 12:49:56 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_ins.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier gerant les demandes
*   de reveil, de la tache TETIM
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	01 Sep 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef etim_ins
#define etim_ins

/* fichiers inclus */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "etim_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Insere dans une liste de demande de reveil, une de-
* mande de reveil. les demandes sont classees dans leurs
* ordre d'echeance. Le timer peut etre eventuellement 
* reprogrammee
*------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void			ei_ins_liste ( ETIM_ENRG_TIMER *	pa_Enr );


/*
* ARGUMENTS EN ENTREE :
*   pa_Enr 		: Pointeur sur enregistrement a inserer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*
******************************************************/
#endif
