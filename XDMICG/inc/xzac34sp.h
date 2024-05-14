/*E*/
/*  Fichier : $Id: xzac34sp.h,v 1.1 1994/11/03 14:52:53 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac34sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif parametre en entree
*                         (HoroLancFin remplace par HoroFin
*                          et HoroSucces supprime)
* C.T.	20/09/94	: Modif taille message (non reteste)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC34_Lire_PMV_Mobile
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* char(48)		va_Message_out
* XY_Horodate		va_HoroDebut_out
* XY_Horodate		va_HoroFin_out
* XDY_Operateur		va_Operateur_out
* 
* Service rendu
* Lecture d'une actio sur un PMV sur un mobile
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee, ou insertion impossible
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Select dans les tables ACTION, PMV_MOBILE
----------------------------------------------*/


 
#define XZAC34_Lire_PMV_Mobile	"XZAC;34" 
