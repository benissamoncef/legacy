/*E*/
/*  Fichier : $Id: xzas53sp.h,v 1.1 1997/05/15 10:11:18 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/15 10:11:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas53sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Mise a jour et ecriture des donnees Meteo
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(dem/1232) (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees Meteo
* 
* Sequence d'appel
* SP	XZAS53_Ajout_Mesure_MET
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_dispo_in
* XDY_Mot		va_code_in
* XDY_Datetime		va_horo_debut_in
* XDY_Datetime		va_horo_fin_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 station inexistante
* 
* Conditions d'utilisation
* - va_Station_in, va_Sens_in, va_Voie_in, va_Horodate_in sont obligatoires
* - si va_horo_fin_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MES_MET
-------------------------------------------------------*/

 
#define XZAS53_Ajout_Mesure_MET	"XZAS53" 
