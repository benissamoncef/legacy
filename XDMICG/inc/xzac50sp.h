/*E*/
/*  Fichier : $Id: xzac50sp.h,v 1.1 1994/11/03 14:53:02 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	21/09/94	: Creation
* C.T.	22/09/94	: Modif numero de commande 
*                         pour ACRP01 est negatif
* C.T.	23/09/94	: Modification du traitement de
*                         recherche du nom de la machine
*                         pilotant (V 1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC50_Commande_Fin_PMV
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Pour revenir l'affichage par defaut sur PMV
* 
* Fonction
* Verifier la non indisponibilite de l'equipement.
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP50 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMV
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/

 
#define XZAC50_Commande_Fin_PMV	"XZAC;50" 
