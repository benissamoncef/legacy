/*E*/
/*  Fichier : $Id: xzac09sp.h,v 1.2 2019/02/06 11:02:26 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/02/06 11:02:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation
* C.T.	14/09/94	: Modif appel XZAC11 et ajout 
*                         du type d'astreinte en entree
* JMG	10/10/18	: pas de ; suppression saga DEM1306 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer une actio d'envoi de fax a un destinataire (uniquement pour les fax 
* "actio", c'est a dire envoyes pour l'exploitation de l'autoroute, et lies a 
* une FMC).
* 
* Sequence d'appel
* SP	XZAC09_Ajouter_Fax
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Texte		Texte
* XDY_Entier		va_Interlocuteur_in
* XDY_Octet		va_TypeAstreinte_in
* XDY_Tel		va_NumeroTalFax_in
* XDY_Horodate		va_HoroLancement_in
* XDY_Operateur		va_Operateur_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Utiliser pour le premier destinataire d'un fax. Les autres 
* destinataires sont ajoutes avec XZAC10.
* Utiliser XZAC54 pour ajouter du texte au fax dans le cas 
* ou il y a plus de 250 caracteres 
* 
* Fonction
* Appeler XZAC11 pour ajouter le destinataire
* Inserer dans la table TEXTE_FAX
-------------------------------------------------------*/

 
#define XZAC09_Ajouter_Fax	"XZAC09" 
