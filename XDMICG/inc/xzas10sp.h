/*E*/
/*  Fichier : $Id: xzas10sp.h,v 1.1 1994/10/26 19:29:21 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/26 19:29:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/10/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage
* 
* Sequence d'appel
* SP	XZAS10_Ajout_Mesure_6_mn
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_Q_in
* XDY_Booleen		va_ValiditeQ_in
* XDY_Octet		va_V_in
* XDY_Booleen		va_ValiditeV_in
* XDY_Octet		va_TO_in
* XDY_Booleen		va_ValiditeTO_in
* XDY_Octet		va_PL_in
* XDY_Booleen		va_ValiditePL_in
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
* - va_Station_in, va_Sens_in, va_Voie_in, va_Horodate_in sont obligatoires
* - si va_Q_in est non value, utiliser XDC_RADT_Q_INCONNU
* de meme pour va_V_in (XDC_RADT_Q_INCONNU), va_TO_in (XDC_RADT_TO_INCONNU), 
* va_PL_in (XDC_RADT_PL_INCONNU)
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MESURE_TRAFIC_6_MINUTES
-------------------------------------------------------*/

 
#define XZAS10_Ajout_Mesure_6_mn	"XZAS;10" 
