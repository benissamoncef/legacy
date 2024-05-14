/*E*/
/*  Fichier : @(#)xzae555sp.prc	1.25      Release : 1.25        Date : 02/05/08
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae555sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/835
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE555_Bouchon_Conseq
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* 
* Arguments en sortie
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
---------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE555' and type = 'P')
	drop procedure XZAE555
go

create procedure XZAE555
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_nb_conseq_out	tinyint output,
	@va_num_out		int = null output,
	@va_cle_out		tinyint = null output
as
		declare  @vl_Status int,@vl_date datetime
		declare @vl_numero int, @vl_cle tinyint

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null

	/*A Controle le parametre obligatoire */
	if (@va_CleEvenement_in is null and @va_NumEvenement_in is null) 
		return XDC_ARG_INV


	/*! cas ou la procedure est creee au CI */

	/*A construire la liste des evenements et  des actions */
	if @va_NumEvenement_in is not null
	begin
		/*A liste des consequences de l'evenement */
		select @vl_date=getdate()
		exec @vl_Status = XZAE;68	@va_NumEvenement_in, @va_CleEvenement_in, 
						@vl_date, XZAEC_PasEffacDonnees

		if @vl_Status != XDC_OK
			return @vl_Status

		/*A  existe t il des bouchons consequence ? */
		select @va_nb_conseq_out=count(*) 
		from LISTE_CONSEQ 
		where spid = @@spid 
		and ( type = XZAEC_FMC_QueueBouchon or type = XZAEC_FMC_TeteBouchon)

		/*on prend le 1� bouchon*/
		if @va_nb_conseq_out>0
		begin
			set rowcount 1
			select @va_num_out=numero,
				@va_cle_out=cle
			from LISTE_CONSEQ
			where spid = @@spid
			and type = XZAEC_FMC_QueueBouchon

			if @@rowcount=0
			begin
				select @vl_numero=numero,
					@vl_cle=cle
				from LISTE_CONSEQ
				where spid = @@spid
				and type = XZAEC_FMC_TeteBouchon

				select @va_num_out=numero,
				@va_cle_out=cle
				from EXP..FMC_GEN
				where cause=@vl_numero and cle_cause=@vl_cle

			end
		end

		/*A effacer les donnees de la table LISTE_CONSEQ (utiliser par XZAE68) */
		delete LISTE_CONSEQ where spid = @@spid
	end

	return XDC_OK
go
