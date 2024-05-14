/*X  Fichier : $Id: xzae106sp.prc,v 1.1 1996/09/20 12:04:26 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/09/20 12:04:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae106sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/96	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Achever une fmc sur le district demande par forcage
*
* Sequence d'appel
* SP    XZAE106_Forcer_Achever_FMC_Sur_District
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* au CI uniquement
*
* Fonction
---------------------------------------------------*/

use PRC
go

#ifdef CI

create procedure XZAE;106
	@va_NumEvt_in	int = null,
	@va_CleEvt_in	tinyint = null,
	@va_Site_in	T_SITE = null
as
	declare @vl_Type smallint, @vl_Cause int

	/* Rechercher le type de la fmc */
	select @vl_Type = type, @vl_Cause = cause
	from EXP..FMC_GEN
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	/*A verifier que toutes les actions relatives a l'evenement */
	/*A sont terminees  sur le site local                       */
	if exists (	select * from EXP..ACT_GEN
			where	evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				heure_fin is null and
				sit=@va_Site_in)
	begin
		return XZAEC_FMC_ACT_NON_FIN
	end

	/*A dans le cas d'un bouchon supprimer l'autre extremite de la liste a traiter */
	if @vl_Type = XZAEC_FMC_QueueBouchon
		/* supprimer la tete de la liste a traiter */
		delete EXP..FMC_TRT
		where   evenement = @vl_Cause and
			cle = @va_CleEvt_in and
			sit = @va_Site_in
	else if @vl_Type = XZAEC_FMC_TeteBouchon
	begin
		/* rechercher la queue de bouchon */
		select @vl_Cause = numero
		from EXP..FMC_GEN
		where cause = @va_NumEvt_in and cle = @va_CleEvt_in

		/* supprimer de la liste a traiter */
		delete EXP..FMC_TRT
		where   evenement = @vl_Cause and
			cle = @va_CleEvt_in and
			sit = @va_Site_in
	end

	/*A Supprimer de la liste a traiter du district demande */
	delete EXP..FMC_TRT 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in and 
		sit = @va_Site_in


	return XDC_OK
go
#endif
