/*E*/
/*  Fichier : $Id: xzac33sp.h,v 1.1 1994/11/03 14:52:52 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac33sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une intervention de la patrouille Escota avec PMV sur fourgon
* 
* Sequence d'appel
* SP	XZAC33_Lire_PMV_Sur_Fourgon
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XY_Horodate		va_HoroDebut_in
* XY_Horodate		va_HoroFin_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* char(20)		va_Message_in
* XDY_Operateur		va_Operateur_in
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Select dans les tables ACTION et PMV_SUR_FOURGON
-----------------------------------------------------*/

 
#define XZAC33_Lire_PMV_Sur_Fourgon	"XZAC;33" 
