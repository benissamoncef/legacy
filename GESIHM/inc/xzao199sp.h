/*E*/
/*Fichier : $Id: xzao199sp.h,v 1.1 1996/01/23 11:38:35 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/01/23 11:38:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao199sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* configuration du reseau
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao199sp.h" */

/* static char *version = "@(#)xzao199.c	1.1 22/01/96 : xzao199" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  supprimer le lien entre un interlocuteur et un objet graphique dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
* SP	XZAO199_Supprimer_Interlocuteur
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_TypeObjet_in
* XDY_Mot		va_NumeroObjet_in
* XDY_Nom		va_NomVille_in
* XDY_Mot		va_NumeroFicheAnnuaire_in
* 
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* supprimer l'interlocuteur pour un objet graphique en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* supprimer le lien entre un interlocuteur et un objet graphique
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* torregrossa	22 Jan 1996	: Creation
------------------------------------------------------*/
 
#define XZAO199_Supprimer_Interlocuteur	"XZAO;199" 
