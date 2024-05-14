/*E*/
/*  Fichier : $Id: xzaf14sp.prc,v 1.2 1995/06/01 18:52:45 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/06/01 18:52:45 $
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
* C.T.	30/05/95	: Ajout de la suppression de ACT_FRM (V 1.2)
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

use PRC
go


create procedure  XZAF;14
as
	delete EVT_FRM where spid = @@spid
	delete ACT_FRM where spid = @@spid

	return XDC_OK
go
