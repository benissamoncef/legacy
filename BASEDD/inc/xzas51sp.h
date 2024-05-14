/*E*/
/*  Fichier : $Id: xzas51sp.h,v 1.1 1996/10/09 17:08:11 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/09 17:08:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas51sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des Nombres d'essieux par classe
* de poids
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
* SP	XZAS51_Ajout_Mesure_EC
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_EC1_in
* XDY_Mot		va_EC2_in
* XDY_Mot		va_EC3_in
* XDY_Mot		va_EC4_in
* XDY_Mot		va_EC5_in
* XDY_Mot		va_EC6_in
* XDY_Mot		va_EC7_in
* XDY_Mot		va_EC8_in
* XDY_Mot		va_EC9_in
* XDY_Mot		va_EC10_in
* XDY_Mot		va_EC11_in
* XDY_Mot		va_EC12_in
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
* - si va_ECx_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table DEBIT HORAIRE PAR CLASSE SIHL
-------------------------------------------------------*/

 
#define XZAS51_Ajout_Mesure_EC	"XZAS51" 
