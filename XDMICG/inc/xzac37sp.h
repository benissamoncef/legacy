/*E*/
/*  Fichier : $Id: xzac37sp.h,v 1.1 1994/11/03 14:52:58 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac37sp.prc
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
* Lecture d'une actio d'affichage dans une gare de peage
* 
* Sequence d'appel
* SP	XZAC37_Lire_Affichage_Gare
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Nom		va_NomGare_out
* XDY_Horodate		va_HoroAffDebut_out
* XDY_Horodate		va_HoroAffFin_out
* char(100)		va_Affichette_out
* XY_Horodate		va_HoroLancement_out
* XY_Horodate		va_HoroFin_out
* XDY_Operateur		va_Operateur_out
* XY_Horodate		va_HoroSucces_out
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
* Select dans les tables ACTION, AFFICHAGE_EN_GARE
-----------------------------------------------------------------------*/

 
#define XZAC37_Lire_Affichage_Gare	"XZAC;37" 
