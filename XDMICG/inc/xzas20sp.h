/*E*/
/*  Fichier : $Id: xzas20sp.h,v 1.1 1994/12/27 10:51:55 fournel Exp $    Release : $Revision: 1.1 $      Date : $Date: 1994/12/27 10:51:55 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas20sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Calcul de retard sur itineraire
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.	29/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare les donnees qui serviront a tracer les courbes de retard
* sur itineraire
*
* Sequence d'appel
* SP    XZAS20_Calcul_Retard_Itineraire
*
* Arguments en entree
* XDY_Mot       va_NumEchangDeb_in      : numero du premier echangeur
* XDY_Mot       va_NumEchangFin_in      : numero du deuxieme echangeur
* XDY_Datetime  va_Horodate_in          : date du calcul (Heure H)
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Nom       nom             : nom de l'echangeur
* XDY_PR        PR              : PR de l'echangeur
* XDY_Mot       retard_H        : retard (Heure H)
* XDY_Mot       retard_H_M1     : retard (Heure H - 1)
* XDY_Mot       retard_H_P1     : retard (Heure H + 1)
* XDY_Entier    cumul_H         : cumul du temps (Heure H)
* XDY_Entier    cumul_H_M1      : cumul du temps (Heure H - 1)
* XDY_Entier    cumul_H_P1      : cumul du temps (Heure H + 1)
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
*
* Fonction
*
-------------------------------------------------------*/

 
#define XZAS20_Calcul_Retard_Itineraire	"XZAS;20" 
