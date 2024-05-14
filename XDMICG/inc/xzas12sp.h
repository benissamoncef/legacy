/*E*/
/*  Fichier : $Id: xzas12sp.h,v 1.1 1994/10/26 19:29:37 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/26 19:29:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/12/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les niveaux de service des autoroutes
* 
* Sequence d'appel
* SP	XZAS12_Ajout_Niveau_Service
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Datetime		va_Horodate_in
* XDY_Octet		va_Niveau_in
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
* - si va_Niveau_in est non value, utiliser XDC_RADT_NIV_INCONNU
* - si va_Niveau_in est non value, aucune action en base
* (aucune erreur n'est retournee par la procedure)
* 
* Fonction
* Inserer ou Modifier dans la table NIVEAU_SERVICE
-------------------------------------------------------*/

 
#define XZAS12_Ajout_Niveau_Service	"XZAS;12" 
