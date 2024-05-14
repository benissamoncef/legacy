/*E*/
/*  Fichier : $Id: xzag50sp.h,v 1.1 1996/10/10 09:52:56 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/10 09:52:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	09/10/96	: Creation	(dem/1227) (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transfere les donnees Pesage du mois dans la base de sauvegarde
* /restauration
* 
* Sequence d'appel
* SP	XZAG04_Transfert_Mesures_Pesage
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: 
* XDC_ARG_INV	: parametre d'entree null ou date demandee trop proche
* 
* Conditions d'utilisation
* - Il faut que la date courante soit au moins superieure a
* au mois specifie plus 7 jours sinon erreur de parametre
* - Le mois vaut de 1 a 12
* - L'annee vaut au moins 1900
* - La periode de sauvegarde est du debut de mois - 7 jours a la
* la fin du mois + 7 jours
* 
* Fonction
* Copier de la base historique dans la base sauvegarde/restauration
-------------------------------------------------------*/

 
#define XZAG04_Transfert_Mesures_Pesage	"XZAG;50" 
