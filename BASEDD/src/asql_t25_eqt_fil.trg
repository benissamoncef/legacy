/*E*/
/*  Fichier : $Id: asql_t25_eqt_fil.trg,v 1.1 2007/03/26 16:23:27 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 16:23:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t25_eqt_fil.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table EQT_FIL
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	22/05/00	: creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* 
*
* Code retour
* 
* nEant
*
* Conditions d'utilisation
* base EXP
---------------------------------------------------*/

use EXP
go

drop trigger eqt_fil_iu_trg
go

create trigger eqt_fil_iu_trg
on EQT_FIL
for insert
as

/*A DEclaration des variables locales */

        declare @vl_NumEvt      int,
                @vl_CleEvt      tinyint,
		@vl_type	smallint
                
/*A rEcupEration des valeurs insErEes */
        select  @vl_NumEvt=evenement,
                @vl_CleEvt=cle
        from inserted

	/*A Rechercher l'evenement */
        select  @vl_type=type
        from FMC_GEN
        where   @vl_NumEvt=numero and
                @vl_CleEvt=cle

	/*si l'evt n'est pas dans FMC_GEN*/
	if @@rowcount = 0
		/*annuler l'insertion*/
		rollback trigger
go
