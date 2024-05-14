/*E*/
/*  Fichier : $Id: asql_c16_res_trc.trg,v 1.1 1995/02/15 19:49:05 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/02/15 19:49:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asq_c01_res_trc.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table RES_TRC
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  	15 fev 95        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Annuler l'update sur un troncon si aucune modification
* pour �viter de r�pliquer inutilement
*
* Code retour
* 
* n�ant
*
* Conditions d'utilisation
* base CFG
---------------------------------------------------*/

use CFG
go

drop trigger res_trc_u_trg
go

create trigger res_trc_u_trg
on RES_TRC
for update
as
	/*A tester s'il y a eu une reelle modification */
	select distinct * from inserted union select distinct * from deleted

	/*A si pas de modification : annuler l'update */
	if @@rowcount=1 rollback trigger
go
