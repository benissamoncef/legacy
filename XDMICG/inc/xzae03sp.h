/*E*/
/*  Fichier : $Id: xzae03sp.h,v 1.2 2007/04/16 18:54:55 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/04/16 18:54:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/09/94	: Creation	(V 1.1)
* JPL	16/04/07	: Changement de nom de la procedure (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des types d'evenement connus
* 
* Sequence d'appel
* SP	XZAE03_Liste_Types_Evt
* 
* Arguments en entree
* 
* Arguments en sortie
* XDY_Octet	va_Type_out
* XDY_Octet	va_Classe_out
* XDY_Nom	va_Nom_out
* char(10)	va_Abreviation_out
* XDY_Booleen	va_Operation_out
* XDY_Booleen	va_Supprime_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* select * from TYPE_EVENEMENT
-------------------------------------------------------*/

 
#define XZAE03_Liste_Types_Evt	"XZAE03"
