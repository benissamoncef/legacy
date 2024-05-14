/*E*/
/*  Fichier : $Id: xzae81sp.h,v 1.1 1994/11/03 14:54:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae81sp.h
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth�se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	14/10/94	: Creation	V 1.1 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajouter le complement d'une fiche courante dans 
* la synthese dans le cas d'evenement historisant
* son complement
* 
* Sequence d'appel
* SP	XZAE81_Intitule_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDC_Octet	va_CleEvt_in
* XDC_Octet	va_Classe_in
* XDC_Mot	va_Type_in
* XDY_Horodate	va_HoroValidation
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	
* XDC_NOK	evenement pas trouve
* < 0          : erreur sybase
* 
* Conditions d'utilisation
* Cette procedure est utilis�e par la procedure XZAE;02
* Elle est interne a la base de donn�e.
* 
* Fonction
* Si l'evenement est de classe Meteo, Vehicule Lent,
* Bouchon, Travaux alors 
* 	rechercher le complement pour l'ajouter a la 
*       synthese de l'evenement
-------------------------------------------------------*/

 
#define XZAE81_Intitule_FMC	"XZAE;81" 
