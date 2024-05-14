/*E*/
/*  Fichier : $Id: xzac27sp.h,v 1.2 1995/01/03 16:28:36 guilhou Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/03 16:28:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac27sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	15/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne le numero de la version actuelle de la liste des actions realisees
* 
* Sequence d'appel
* SP	XZAC27_Controler_Modif
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Compteur_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Appele pour savoir s'il est utile de relire les actions avec XZAC21
* 
* Fonction
* Select compteur from EVENEMENT where numero, cle = NumEvt
---------------------------------------------*/

 
#define XZAC27_Controler_Modif	"XZAC;27" 
