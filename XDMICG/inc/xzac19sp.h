/*E*/
/*  Fichier : $Id: xzac19sp.h,v 1.2 2009/11/04 09:10:59 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/11/04 09:10:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/09/94	: Creation
* JPL	03/11/09	: Procedure degroupee (renommee sans ";")
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer ou modifier un message TFM (au CI)
* 
* Sequence d'appel
* SP	XZAC19_Ecrire_Action_TFM
* 
* Arguments en entree
* XDY_Entier		va_NumeroEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Horodate		va_HeureLancement_in
* XDY_Horodate		va_HeureLancementFin_in
* XDY_Operateur		va_Operateur_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evnement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio est non value en entree alors 
* l'actio est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement
* Insert ou update (pour arreter la diffusion) dans la 
* table ACTION et dans la table MESSAGE_TFM
* Si un message etait actif pour le meme evenement, 
* positionne son horodate de fin.
* Appelle XZIT04 pour prevenir l'animateur radio d'un 
* changement dans sa liste de messages.
--------------------------------------------------*/

 
#define XZAC19_Ecrire_Action_TFM	"XZAC19" 
