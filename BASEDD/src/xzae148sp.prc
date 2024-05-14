/*E*/
/*  Fichier : $Id: xzae148sp.prc,v 1.8 2010/09/08 12:19:26 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2010/09/08 12:19:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae148sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* recupere les droits d'enrichir des FMC du PC niveau 2
* qui etaient au CI
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14/01/98	: creation (dem/1535) 1.1
* JMG	18/06/1998	: correction pour ne pas avoir les FMC sur zone de recouvrement 1.2
* JPL	15/05/07	: Recherche PR tete en table meteo pour FMCs Chaussees (DEM 644,646) 1.3
* JPL	05/06/07	: Recherche PR tete en table travaux pour FMCs Basculement pour travaux (DEM 647) 1.4
* PNI   21/11/07	: Correction du cursor : ajout de la table FMC_HIS dans le select pour etre sur d'avoir des FMC validees 1.5 
* PNI   22/09/08	: exec de xzae;15 au CI (DEM 729) 1.6
* JPL	12/01/2009	: Degroupee; appel a XZAE15 renommee (lie a DEM 848) 1.7
* JPL	19/08/2010	: Recherche PR tete en table travaux pour FMC Zone de Stockage (DEM 942) 1.8
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* recupere les droits d'enrichir des FMC du PC niveau 2
* qui etaient au CI
* 
* Sequence d'appel
* SP	XZAE148_Recupere_Droits_FMC
* 
* Arguments en entree
* XDY_Octet		va_Site_in	: site 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* 
* Fonction
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE148' and type = 'P')
	drop procedure XZAE148
go


create procedure XZAE148

	@va_Site_in		char(2)		= null
as
	declare  @vl_res int,
		@vl_Site tinyint,
		@vl_SiteMaitre tinyint,
		@vl_fmc int,
		@vl_cle tinyint,
		@vl_Horodate datetime, @vl_NumAutoroute tinyint,
		@vl_Type	tinyint,
		@vl_Classe	tinyint,
		@vl_Autoroute	char(4),
		@vl_PR		T_PR,
		@vl_PRtete	T_PR,
		@vl_NumCause	int,
		@vl_CleCause	tinyint
		
	if @va_Site_in = null 
		return XDC_ARG_INV
		
	select @vl_Site=numero
	from CFG..RES_DIS
	where code = @va_Site_in

	if @vl_Site=null
	  return XDC_NOK

	select @vl_res = XDC_OK

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*A rechercher la zone du district */
	select
		autoroute,
		PR_debut,
		PR_fin
	into #ZONE
	from CFG..RES_POR
	where district = @vl_Site 

	
	/*B On recherche les fmc du site et qui sont au CI*/
	declare fmc cursor for
	select distinct
			FMC.numero,
			FMC.cle
	from EXP..FMC_GEN FMC,EXP..FMC_TRT TRT, EXP..FMC_HIS H
	where TRT.evenement = FMC.numero and
		    TRT.cle = FMC.cle and
		    H.numero=FMC.numero and
		    H.cle=FMC.cle and
		    TRT.sit = @vl_Site and 
		    FMC.sit = @vl_SiteMaitre and
	     	    FMC.type!=XZAEC_FMC_PosteOperateur 
			
	open fmc
	
	fetch fmc into	@vl_fmc,
			@vl_cle
        
	/*A Pour chaque fmc trouvee : */

	while (@@sqlstatus = 0)
	begin
	
		/*A rechercher la derniere date de validation de  */
		/*A la partie historique de l'evenement anterieur */
		/*A a la date donnee                              */
		select 
			@vl_Type		= type,
			@vl_Classe		= classe
		from  EXP..FMC_GEN G, CFG..TYP_FMC T
		where	G.numero = @vl_fmc and 
			G.cle = @vl_cle and
			G.type = T.numero 

		if @vl_Type!=XZAEC_FMC_PosteOperateur
		begin
			select 
				@vl_Horodate = max(horodate_validation) 
			from EXP..FMC_HIS
			where	EXP..FMC_HIS.numero = @vl_fmc and 
				EXP..FMC_HIS.cle = @vl_cle 
			if @@rowcount = 0
			begin
				return XDC_NOK
			end
	
			/*A lecture de la fiche main courante pour la derniere validation */
			select 
				@vl_NumAutoroute	= EXP..FMC_HIS.autoroute,
				@vl_PR			= EXP..FMC_HIS.PR,
				@vl_NumCause		= EXP..FMC_GEN.cause,
				@vl_CleCause		= EXP..FMC_GEN.cle_cause
	
			from  EXP..FMC_GEN,EXP..FMC_HIS
			where	EXP..FMC_GEN.numero = @vl_fmc and 
				EXP..FMC_GEN.cle = @vl_cle and 
				EXP..FMC_HIS.numero = @vl_fmc and 
				EXP..FMC_HIS.cle = @vl_cle and 
				EXP..FMC_HIS.horodate_validation = @vl_Horodate
	
			/* test si une fiche a ete trouvee sinon erreur */
			if @@rowcount = 0
			begin
				return XDC_NOK
			end
		
			/* recupere le nom de l'autoroute */
			select @vl_Autoroute=nom
			from CFG..RES_AUT
			where numero=@vl_NumAutoroute

			/*A Recherche la localisation de la tete de l'evenement */
			/*A s'il y a lieu                            */
			if @vl_Classe=XZAEC_ClasseBouchon
			begin
				select 
					@vl_PRtete		= PR
				from  EXP..FMC_HIS
				where	numero = @vl_NumCause and 
					cle = @vl_CleCause 
				group by numero,cle
				having 
					horodate_validation = max( horodate_validation )

			end
			else
			if (@vl_Classe=XZAEC_ClasseTravaux or
			    @vl_Classe=XZAEC_ClasseBasculTrav)
			begin
				select 
					@vl_PRtete		= localisation_tete
				from  EXP..FMC_TRH
				where	numero =  @vl_fmc and 
					cle = @vl_cle and 
					horodate = @vl_Horodate
			end
			else
			if (@vl_Classe=XZAEC_ClasseMeteo)
			begin
				select 
					@vl_PRtete		= PR
				from  EXP..FMC_MET
				where	numero =  @vl_fmc and 
					cle = @vl_cle and 
					horodate = @vl_Horodate
			end
			else
			begin
				select @vl_PRtete = @vl_PR
			end
			
			if exists ( select * from #ZONE where autoroute = @vl_NumAutoroute and ((@vl_PR < PR_fin and @vl_PR >= PR_debut) or 
											     (@vl_PRtete < PR_fin and @vl_PRtete >= PR_debut)) )
			begin
				exec PRC..XZAE15	@vl_fmc,
				@vl_cle,
				@vl_Site,
				null,
				@vl_res output,
				XDC_FAUX
			end
		end
		/*A	passe a la fmc suivante */
		fetch fmc into	@vl_fmc,
				@vl_cle
	
	end
	close fmc
		
	/* rafraichir la liste du site */
	exec XZAR;03 0, 0, null, " ", @vl_Site

	if @vl_Site <> @vl_SiteMaitre
	/* rafraichir la liste a traiter du CI */
		exec XZAR;03 0, 0, null, " ", @vl_SiteMaitre		

	return @vl_res
go
