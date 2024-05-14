/*E*/
/*  Fichier : $Id: xzac35sp.h,v 1.1 1994/11/03 14:52:55 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac35sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	08/09/94	: Creation
* C.T.	13/09/94	: Modif parametre en entree
*                         (HoroLancFin remplace par HoroFin)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture d'une intervention d'un depanneur
* 
* Sequence d'appel
* SP	XZAC35_Lire_Intervention_Depanneur
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
*
* Arguments en sortie
* XDY_Nom		va_NomDepanneur_out
* XDY_Nom		va_Resultat_out
* XY_Horodate		va_HoroLancement_out
* XY_Horodate		va_HoroFin_out
* XDY_Operateur		va_Operateur_out
* XY_Horodate		va_HoroSucces_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* 
* Conditions d'utilisation
* 
* Fonction
* Select dans les tables ACTION, INTERVENTION_DEPANNEUR
------------------------------------------------*/

 
#define XZAC35_Lire_Intervention_Depanneur	"XZAC;35" 
