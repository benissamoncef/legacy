/*E*/
/*  Fichier : $Id: xzas50sp.h,v 1.1 1996/10/09 17:08:09 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/09 17:08:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des DEbit horaire par classe
* de silhouette
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage
* 
* Sequence d'appel
* SP	XZAS50_Ajout_Mesure_KC
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_KC1_in
* XDY_Mot		va_KC2_in
* XDY_Mot		va_KC3_in
* XDY_Mot		va_KC4_in
* XDY_Mot		va_KC5_in
* XDY_Mot		va_KC6_in
* XDY_Mot		va_KC7_in
* XDY_Mot		va_KC8_in
* XDY_Mot		va_KC9_in
* XDY_Mot		va_KC10_in
* XDY_Mot		va_KC11_in
* XDY_Mot		va_KC12_in
* XDY_Mot		va_KC13_in
* XDY_Mot		va_KC14_in
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
* - si va_KCx_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table DEBIT HORAIRE PAR CLASSE SIHL
-------------------------------------------------------*/

 
#define XZAS50_Ajout_Mesure_KC	"XZAS50" 
