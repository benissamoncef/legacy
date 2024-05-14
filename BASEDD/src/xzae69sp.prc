/*E*/
/*  Fichier : $Id: xzae69sp.prc,v 1.6 1998/09/22 13:27:55 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/09/22 13:27:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae69sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/11/94	: Creation a partir de asrv_e69.prc
*                         (V 1.1)
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.2)
* B.G.	06/12/94	: Modif type (cause et non evt) (V 1.3)
* C.T.	05/06/95	: Blindage contre les bouclages de causes (V 1.4)
* C.T.	14/06/95	: Correction erreur compile (V 1.5)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.6)
* LCL	03/05/23	: Attente remontee replication pour COCKPIT
* LCL	15/05/23	: Adaptation attente remontee replication COCKPIT
* LCL	31/05/23	: Suppression attente COCKPIT suite a mise en place procedures chapeau
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des causes d'un evenement
* 
* Sequence d'appel
* SP	XZAE69_Liste_Causes
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante
* 
* Arguments en sortie
* XDY_Entier		va_NbCauses_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* parcoure la table EVENEMENT avec le lien cause jusqu'au pere final
-------------------------------------------------*/

use PRC
go

create procedure XZAE;69
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Horodate_in		datetime = null  -- par defaut heure courante
as
	declare @vl_NumEvt int, @vl_CleEvt tinyint, @vl_Cause int,  
		@vl_Status int, @vl_CleCause tinyint, @vl_replication_ok int, @vl_timeout int,
		@vl_zz int, @vl_zz_new int, @vl_origine varchar(30)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date courante a prendre si date donnee null */
	if @va_Horodate_in = null
		select @va_Horodate_in = getdate()

	select @vl_NumEvt =  @va_NumEvt_in, @vl_CleEvt = @va_CleEvt_in

	/*A creation d'une table temporaire pour stocker toutes les causes */
	create table #CAUSES (numero int not null, cle tinyint not null, type smallint  null)
	
#ifdef HIST
/*A recherche la premiere cause de l'evenement */
		select @vl_Cause = cause, @vl_CleCause = cle_cause
		from HIS..FMC_GEN
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_lien_cause <=  @va_Horodate_in
	
		/*A liste des causes d'un evenement */
		while ( @@rowcount = 1 and @vl_Cause != null and @vl_CleCause != null)
		begin
			/*B memorisation d'une cause */
			if not exists (select * from #CAUSES
					where numero = @vl_Cause and cle = @vl_CleCause)
			begin
				insert #CAUSES 
				values( @vl_Cause,  @vl_CleCause, 0)
		
				select @vl_NumEvt = @vl_Cause, @vl_CleEvt = @vl_CleCause
		
				/*B recherche cause precedente */
				select @vl_Cause = cause, @vl_CleCause = cle_cause 
				from HIS..FMC_GEN 
				where 	numero = @vl_NumEvt and cle = @vl_CleEvt and
					horodate_lien_cause <=  @va_Horodate_in
			end
			/* un bouclage : on arrete tout */
			else	select @vl_Cause = null
		end

		update #CAUSES
		set type = HIS..FMC_GEN.type
		from HIS..FMC_GEN
		where HIS..FMC_GEN.numero=#CAUSES.numero and
		      HIS..FMC_GEN.cle=#CAUSES.cle
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE;69
				@va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in
 		if @vl_Status = null
                       		return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		/* Adaptation PIT pour attente remontee replication */
/*		select @vl_replication_ok=0
		select @vl_timeout=0
		select @vl_zz = zz, @vl_origine=origine_creation from EXP..FMC_GEN
		where EXP..FMC_GEN.numero = @va_NumEvt_in and
			  EXP..FMC_GEN.cle = @va_CleEvt_in
		if @@rowcount = 0
		begin
			select @vl_zz=-1
			select @vl_origine = "PIT"
		end

		if @vl_origine like "PIT%"
		begin
			while ( @vl_replication_ok = 0 and @vl_timeout < 50 )
			begin
				select @vl_timeout=@vl_timeout+1
				select @vl_zz_new = zz from EXP..FMC_GEN
				where EXP..FMC_GEN.numero = @va_NumEvt_in and
					EXP..FMC_GEN.cle = @va_CleEvt_in

				if @@rowcount > 0
					if @vl_zz_new > @vl_zz
						select @vl_replication_ok = 1

				waitfor delay "00:00:00.100"
			end
		end
*/		
		/*A recherche la premiere cause de l'evenement */
		select @vl_Cause = cause, @vl_CleCause = cle_cause
		from EXP..FMC_GEN
		where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_lien_cause <=  @va_Horodate_in
	
		/*A liste des causes d'un evenement */
		while ( @@rowcount = 1 and @vl_Cause != null and @vl_CleCause != null)
		begin
			/*B memorisation d'une cause */
			if not exists (select * from #CAUSES
					where numero = @vl_Cause and cle = @vl_CleCause)
			begin
				insert #CAUSES 
				values( @vl_Cause,  @vl_CleCause, 0)
		
				select @vl_NumEvt = @vl_Cause, @vl_CleEvt = @vl_CleCause
		
				/*B recherche cause precedente */
				select @vl_Cause = cause, @vl_CleCause = cle_cause 
				from EXP..FMC_GEN 
				where 	numero = @vl_NumEvt and cle = @vl_CleEvt and
					horodate_lien_cause <=  @va_Horodate_in
			end
			/* un bouclage : on arrete tout */
			else	select @vl_Cause = null
		end

		update #CAUSES
		set type = EXP..FMC_GEN.type
		from EXP..FMC_GEN
		where EXP..FMC_GEN.numero=#CAUSES.numero and
		      EXP..FMC_GEN.cle=#CAUSES.cle
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/* Adaptation PIT pour attente remontee replication */
	select @vl_replication_ok=0
	select @vl_timeout=0
	select @vl_zz = zz, @vl_origine=origine_creation from EXP..FMC_GEN
	where EXP..FMC_GEN.numero = @va_NumEvt_in and
			EXP..FMC_GEN.cle = @va_CleEvt_in
	if @@rowcount = 0
	begin
		select @vl_zz=-1
		select @vl_origine = "PIT"
	end

	if @vl_origine like "PIT%"
	begin
		while ( @vl_replication_ok = 0 and @vl_timeout < 50 )
		begin
			select @vl_timeout=@vl_timeout+1
			select @vl_zz_new = zz from EXP..FMC_GEN
			where EXP..FMC_GEN.numero = @va_NumEvt_in and
				EXP..FMC_GEN.cle = @va_CleEvt_in

			if @@rowcount > 0
				if @vl_zz_new > @vl_zz
					select @vl_replication_ok = 1
	
			waitfor delay "00:00:00.100"
		end
	end
	
	/*A recherche la premiere cause de l'evenement */
	select @vl_Cause = cause, @vl_CleCause = cle_cause
	from EXP..FMC_GEN
	where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate_lien_cause <=  @va_Horodate_in

	/*A liste des causes d'un evenement */
	while ( @@rowcount = 1 and @vl_Cause != null and @vl_CleCause != null)
	begin
		/*B memorisation d'une cause */
		if not exists (select * from #CAUSES
				where numero = @vl_Cause and cle = @vl_CleCause)
		begin
			insert #CAUSES 
			values( @vl_Cause,  @vl_CleCause, 0)
	
			select @vl_NumEvt = @vl_Cause, @vl_CleEvt = @vl_CleCause
	
			/*B recherche cause precedente */
			select @vl_Cause = cause, @vl_CleCause = cle_cause 
			from EXP..FMC_GEN 
			where 	numero = @vl_NumEvt and cle = @vl_CleEvt and
				horodate_lien_cause <=  @va_Horodate_in
		end
		/* un bouclage : on arrete tout */
		else	select @vl_Cause = null
	end

	update #CAUSES
	set type = EXP..FMC_GEN.type
	from EXP..FMC_GEN
	where EXP..FMC_GEN.numero=#CAUSES.numero and
		EXP..FMC_GEN.cle=#CAUSES.cle
#endif
#endif

	/*A retourner la liste des causes */
	select * from #CAUSES
	where type<>XZAEC_FMC_TeteBouchon

	return XDC_OK
go
