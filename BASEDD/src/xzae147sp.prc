/*E*/
/*  Fichier : $Id: xzae147sp.prc,v 1.4 2014/11/12 10:11:19 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2014/11/12 10:11:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae147sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* transmet les droits d'enrichir d'un site a un autre
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/01/98	: creation (dem/1535) 1.1
* JMG	14/01/98	: 2 sites en argument en entree (dem/1535) 1.2
* JPL	12/01/09	: Degroupee; appel a XZAE15 renommee (lie a DEM 848) 1.3
* JPL	10/10/14	: Appel a XZAE15 avec demande "sans reveil" (DEM 1101)  1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* transmet les droits d'enrichir d'un site a un autre
* 
* Sequence d'appel
* SP	XZAE147_Transmet_Droits_FMC
* 
* Arguments en entree
* XDY_Octet		va_Site_Origine_in	: site  origine
* XDY_Octet		va_Site_Dest_in		: site dest 
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

if exists (select * from sysobjects where name = 'XZAE147' and type = 'P')
	drop procedure XZAE147
go


create procedure XZAE147

	@va_Site_Origine_in		char(2)		= null,
	@va_Site_Dest_in		char(2)         = null
as
	declare  @vl_res int,
		@vl_Site tinyint,
		@vl_Site_Dest tinyint,
		@vl_fmc int,
		@vl_cle tinyint
		
	if @va_Site_Origine_in = null or @va_Site_Dest_in = null
		return XDC_ARG_INV
		
	select @vl_Site=numero
	from CFG..RES_DIS
	where code = @va_Site_Origine_in

	if @vl_Site=null
	  return XDC_NOK

	select @vl_Site_Dest=numero
	from CFG..RES_DIS
	where code=@va_Site_Dest_in
	select @vl_res = XDC_OK

	if @vl_Site_Dest=null
	  return XDC_NOK

	/*B On recherche les fmc du site qui ne sont pas operateur*/
	declare fmc cursor for
	select distinct
			FMC.numero,
			FMC.cle
		from EXP..FMC_GEN FMC,EXP..FMC_TRT TRT
		where TRT.evenement = FMC.numero and
		    TRT.cle = FMC.cle and
		    FMC.sit = @vl_Site and
	     	    FMC.type!=XZAEC_FMC_PosteOperateur 
			
	open fmc
	
	fetch fmc into	@vl_fmc,
			@vl_cle
        
	/*A Pour chaque fmc trouvee : */
	while (@@sqlstatus = 0)
	begin
		
		exec XZAE15	@vl_fmc,
				@vl_cle,
				@vl_Site_Dest,
				null,
				@vl_res output,
				XDC_FAUX,
				XDC_VRAI            /* sans reveil */

		/*A	passe a la fmc suivante */
		fetch fmc into	@vl_fmc,
				@vl_cle
	
	end
	close fmc
		
	/* rafraichir la liste du site destination */
	exec XZAR;03 0, 0, null, " ", @vl_Site_Dest

	return @vl_res
go
