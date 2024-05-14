/*E*/
/* Fichier : $Id: xzas540sp.prc,v 1.1 2012/07/04 11:37:30 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:37:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas540sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module STRADA : Extraction des donnees radt a exporter
*
------------------------------------------------------
* HISTORIQUE :
*
* MG 15/05/2012 : Creation (DEM/1016)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAS540
* 
* Arguments en entree
* smallint	station
* tinyint	sens
* datetime  horodate
* 
* Arguments en sortie
*
* int		Q
* int		V
* int		TO
* tinyint	validite_Q
* tinyint	validite_TO
* tinyint	validite_V		
* 
* Service rendu
* retourne la liste ci-dessus pour l'horodate courante-6mn
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS540' and type = 'P')
	drop procedure XZAS540
go

create procedure XZAS540
	@va_station		smallint,
	@va_sens 		tinyint,
	@va_horodate	datetime,
	
	@va_Q_out			int output,
	@va_V_out			int output,
	@va_TO_out			int output,
	@va_validQ_out		tinyint output,
	@va_validV_out		tinyint output,
	@va_validTO_out		tinyint output
as

declare @vl_date_courante	datetime,
		@vl_date_min		datetime,
		@vl_Status int
	
if @va_station is null or @va_sens is null or @va_horodate = null
	return XDC_ARG_INV

	
			/* on calcule la date courante */
			select @vl_date_courante = @va_horodate
			select @vl_date_min = dateadd(minute,-6, @va_horodate)


#ifdef HIST

       /*A
        ** Recherche evenements sur HIS
        */
		
		select	@va_Q_out = Q,
				@va_validQ_out = validite_Q,
				@va_V_out = V,
				@va_validV_out = validite_V,
				@va_TO_out = T_O,
				@va_validTO_out = validite_TO
		from	HIS..MES_TRA
		where	station = @va_station and
				sens = @va_sens and
				voie = XDC_RADT_CHAUSSEE and
				horodate <= @vl_date_courante and
				horodate > @vl_date_min
		
#else
		
		/*A si la date donnee est anterieure d'un mois au moins */
	 	/*A rechercher l'evenement dans la base HIS sinon dans  */
		if @va_horodate < (select dateadd(month, -1, getdate ()))
			begin	
				exec @vl_Status = SQL_HIS.PRC..XZAS540
									   	@va_station,
										@va_sens,
										@va_horodate,
										@va_Q_out output,
										@va_V_out output,
										@va_TO_out output,
										@va_validQ_out output,
										@va_validV_out output,
										@va_validTO_out output

		        if @vl_Status = null
		                return XDC_PRC_INC
		        else return @vl_Status
			end
		else
			begin
		
		     	/*A
		        ** Recherche evenements sur EXP
		        */
				
				select	@va_Q_out = Q,
						@va_validQ_out = validite_Q,
						@va_V_out = V,
						@va_validV_out = validite_V,
						@va_TO_out = T_O,
						@va_validTO_out = validite_TO
				from	EXP..MES_TRA
				where	station = @va_station and
						sens = @va_sens and
						voie = XDC_RADT_CHAUSSEE and
						horodate <= @vl_date_courante and
						horodate > @vl_date_min
			end

#endif

	   return XDC_OK

go

