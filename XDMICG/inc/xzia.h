/*E Fichier : $Id: xzia.h,v 1.8 2010/12/07 12:02:01 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/12/07 12:02:01 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE XZIA * FICHIER XZIA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* GUILHOU	30 Sep 1994	: Creation		V1.1
* GUILHOU	13 oct 1994	: bouchon xzia08	V1.2
* NOEL		14 Oct 1994 	: Rien			V1.3
* Guilhou	21 dec 1994	: modif type XZIAT_Destinataire		V1.4
* Guilhou	21 dec 1994	: modif interface XZIA06 V1.5
* MERCIER	16 Jan 1995	: Replacement du endif a la fin du fichier V 1.5
* T.Milleville	11 Oct 1995	: Ajout d'appel a xzsm.h V1.6
* JMG		07/12/10	: portage linux 1.7
------------------------------------------------------ */

#ifndef XZIA
#define XZIA

/* fichiers inclus */

#include	"xdc.h"
#include	"xdy.h"
#include	"xdg.h"
#include	"xdm.h"

#include	"xzst.h"
#include	"xzsm.h"
#include	"xzic.h"


#include	<rtworks/common.h>
#include	<rtworks/ipc.h>
#ifdef _HPUX_SOURCE
#include	<rtworks/ipc_proto.h>
#endif

/* definitions de constantes exportees */


/* definitions de types exportes */
typedef union {
		XDY_NomMachine	machine;
		XDY_NomSite	site;
} XZIAT_destinataire;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* affiche un libelle dans la zone d'avertissement
* de l'ecran synoptique sur une machine ou un site
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZIA_06AffMsgSyst(XZIAT_destinataire va_destinataire, char *pa_libelle
	  );

/*
* ARGUMENTS EN ENTREE :
*
* va_destinataire       destinataire du libelle
* va_libelle            message a afficher
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR :
*
* XDC_NOK       Erreur d'ex≈cution de la fonction
* XDC_OK        Bonne ex≈cution de la fonction
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*permet d'afficher un message dans la zone d'avertissement
* de l'ecran synoptique sur une machine ou un site
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZIA08_AffMsgAdm(XZIAT_destinataire va_destinataire, char *va_texte
          );

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/


#endif
