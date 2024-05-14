/*E*/
/*  Fichier : $Id: xzac32sp.h,v 1.1 1994/11/03 14:52:51 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac32sp.prc
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
* Lecture d'une intervention de la patrouille Escota
* 
* Sequence d'appel
* SP	XZAC32_Lire_Intervention_SIS
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Nom		va_NomPatrouille_out
* XDY_Booleen		va_PoseBalisage_out
* XY_Horodate		va_DebutBalisage_out
* XY_Horodate		va_FinBalisage_out
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
* Select dans les tables ACTION, INTERVENTION_PATROUILLE
-----------------------------------------------------------------------*/

 
#define XZAC32_Lire_Intervention_SIS	"XZAC;32" 
