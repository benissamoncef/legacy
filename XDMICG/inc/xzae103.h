/*E Fichier : $Id: xzae103.h,v 1.5 2010/12/09 00:05:05 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/12/09 00:05:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE103 * FICHIER XZAE103.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	26 Aou 1996	: Creation
*JMG		02/10/09 : modif NB PO DEM914 1.2
* JMG		12/12/10 : portage linux 1.3
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : commentaires imbriques  1.5
------------------------------------------------------ */

#ifndef XZAE103
#define XZAE103

/* fichiers inclus */
	/* Include systeme */
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <memory.h>
	
	#include <rtworks/common.h>
	#include <rtworks/ipc.h>
	
	/* Include Sybase */
#ifdef _HPUX_SOURCE
	#include <ctpublic.h>
	#include <cstypes.h>
#endif
	/* Include applicatif Migrazur */
	#include "xdy.h"
	#include "xdc.h"
	#include "xdg.h"
	#include "xdm.h"
	#include "xzea.h"
	#include "xzst.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

typedef struct
   {
   XDY_Booleen	Presence;
   XDY_Nom	NomSite;
   } XZAET_EPresence_Op_PCsimpl;

typedef struct 
{
  XDY_Nom	Machine;
  XDY_District	Site;
} XZAET_Machine_Presente;

#define XZAEC_NB_PO	20 	/* 5 * 4 = 20 */

/* definition de macro exportees */

/* declarations de donnees globales */



/* delaration de fonctions externes */
extern int XZAE103_Abt_Presence_Op_PCsimpl(XDY_Booleen va_abonnement, XDY_Fonction pa_fonction);

/*X*/
/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Fonction	pa_fonction		: Nom de la fonction utilisateur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etat de communication des Sites
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre le Datagroup correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre le Datagroup
********************************************************/



#endif
