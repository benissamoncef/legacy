/*E*/
/*  Fichier : $Id: xzac44sp.h,v 1.1 1994/11/03 14:53:02 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac44sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
* C.T.	20/09/94	: Modif retour du message tfm 
*                         suite a modif de table ACT_TXT_TFM
*                         modif interface (ajout site)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio Message TFM
* 
* Sequence d'appel
* SP	XZAC44_Lire_ActionTFM
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_Horodate	va_HeureLancement_out		envoi a la radio
* XDY_Horodate	va_HeureLancementFin_out	envoi de la fin de message
* XDY_Horodate	va_HeureSucces_out		premiere diffusion
* XDY_Horodate	va_HeureFin_out			suppression du message
* XDY_Entier	va_NombreDiffusions_out
* char*		va_Message_out			liste de morceau de texte
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* le message de trafic FM est retourne dans une liste de 
* de morceau de texte ordonnee
* 
* Fonction
* Select des tables ACTION et MESSAGE_TFM
------------------------------------------------------*/

 
#define XZAC44_Lire_ActionTFM	"XZAC;44" 
