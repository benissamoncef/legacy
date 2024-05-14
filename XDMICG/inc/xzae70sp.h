/*E*/
/*  Fichier : $Id: xzae70sp.h,v 1.1 1994/11/03 14:53:57 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae70sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	01/09/94	: Creation
* C.T.	13/09/94	: Modif sur test de tete de bouchon
* C.T.  04/10/94        : Ajout de la suppression de bouclage 
*			  et la colonne spid dans LISTE_CONSEQ
*                         (V 1.5)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des consequences d'un evenement
* 
* Sequence d'appel
* SP	XZAE70_ListeConsequence
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in 
* XDY_Booleen		va_Memorisation_in 
* 
* Arguments en sortie
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* XDY_Octet		va_Type_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Attention, saute la queue du bouchon
* les arguments de sortie sont retournes dans une liste
* 
* Fonction
* SI Type(NumEvt)=TetedeBouchon ALORS NumEvt=NumQueue
* Select numero, cle, type from EVENEMENT where cause, cle_cause=NumEvt
---------------------------------------------------*/

 
#define XZAE70_ListeConsequence	"XZAE;70" 
