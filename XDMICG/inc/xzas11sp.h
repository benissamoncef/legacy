/*E*/
/*  Fichier : $Id: xzas11sp.h,v 1.1 1994/10/26 19:29:32 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/26 19:29:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage poids
* 
* Sequence d'appel
* SP	XZAS11_Ajout_Mesure_Poids
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Datetime		va_Horodate_in
* XDY_Entier		va_Cumul_in
* XDY_Entier		va_NbPl_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres d'entree null, ou valeur non autorisee ou
*                 station inexistante
* 
* Conditions d'utilisation
* - va_Station_in, va_Sens_in, va_Horodate_in sont obligatoires
* - si va_Cumul_in est non value, utiliser XDC_RADT_CUMUL_INCONNU
* de meme pour va_NbPl_in (XDC_RADT_NBPL_INCONNU)
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MESURE_POIDS
-------------------------------------------------------*/

 
#define XZAS11_Ajout_Mesure_Poids	"XZAS;11" 
