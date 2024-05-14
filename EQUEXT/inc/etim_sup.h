/*E*/
/* Fichier : $Id: etim_sup.h,v 1.2 1995/05/23 12:49:58 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 12:49:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_sup.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier gerant les annulations
*  de reveil, pour la tache TETIM
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef etim_sup
#define etim_sup

/* fichiers inclus */
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "etim_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Dans une liste de demande de reveil, detruit un reveil
* donne selon la cle et le numero de la socket de donnee.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void		es_sup_liste ( 	char 	*pa_CleProg,
				int      va_NumSock  );
				
/*
* ARGUMENTS EN ENTREE :
*   pa_CleProg 		: Cle du reveil
*   va_NumSock		: Numero de la socket de la tache traitement
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
*   Recherche dans la liste chainee de l'enregistrement timer a supprimer
*   Suppression de l'enregistrement
*
------------------------------------------------------*/
#endif
