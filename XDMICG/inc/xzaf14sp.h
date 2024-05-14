/*E*/
/*  Fichier : $Id: xzaf14sp.h,v 1.1 1995/01/18 09:42:50 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 09:42:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/01/95	: Creation 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF14_Supprimer_Evts_Actions
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Service rendu
* Supprimer tous les actions et evenements en cours a
* la date precisee et clos actuellement pour la formation
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

 
#define XZAF14_Supprimer_Evts_Actions	"XZAF;14" 
