/*E*/
/*  Fichier : $Id: xzae110sp.prc,v 1.6 2020/11/03 16:13:06 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/11/03 16:13:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae110sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	10/10/96	: Creation 	V 1.1
* C.T.     	02/12/96	: Ajout en entree date de fin de la fmc
*                                               V1.2
* P.N.		24/12/96	: Ajout de la mise a jour de FMC_HIS pour maj des synoptique v1.3
* JMG		07/01/98	: gestion multi-sites (dem/1536) 1.4
* JPL		12/01/2009	: Degroupee; appel a XZAE13 et XZAE14 renommees (lie a DEM 848) 1.5
* LCL   	22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Annuler une fmc a traiter
*
* Sequence d'appel
* SP    XZAE110_Annuler_FMC_A_traiter
*
* Arguments en entree
* XDY_Entier    va_NumEvt_in
* XDY_Octet     va_CleEvt_in
* XDY_Datetime  va_DateFin_in
*
* Arguments en sortie
*
* Code retour
* XDC_ARG_INV
* XDC_OK
* XZAEC_FMC_PAS_ENR : fmc pas modifiable par ce site
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE110' and type = 'P')
	drop procedure XZAE110
go


create procedure XZAE110
	@va_NumEvt_in   int = null,
	@va_CleEvt_in   tinyint = null,
	@va_DateFin_in  datetime = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Type smallint, @vl_PosteOperateur T_OPERATEUR,
		@vl_DebutPrevu datetime, @vl_Site T_SITE,
                @vl_Tete int, @vl_Resultat int, @vl_Status int

	/*A controler les paramatres d'entree */
	if @va_NumEvt_in is null or @va_CleEvt_in is null
		return XDC_ARG_INV

	/* recherche le site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_Site is null
		return XDC_NOK

	/*A Rechercher l'evenement */
	select
		@vl_DebutPrevu = isnull(debut_prevu,debut),
		@vl_Type = type,
		@vl_Tete = cause,
		@vl_PosteOperateur = poste_enrichisseur
	from EXP..FMC_GEN
	where  numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount = 0
		return XDC_NOK

	if @vl_Type = XZAEC_FMC_TeteBouchon
		return XDC_ARG_INV

	/*A positionner la date de fin */
	update EXP..FMC_GEN set debut = null, debut_prevu = @vl_DebutPrevu, fin = @va_DateFin_in, zz = zz + 1
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in and sit = @vl_Site

	if @@rowcount = 0
		return XZAEC_FMC_PAS_ENR

	/*A achever la FMC */
	if @vl_Type != XZAEC_FMC_QueueBouchon
		exec @vl_Status = XZAE14 @va_NumEvt_in, @va_CleEvt_in,
						@vl_PosteOperateur, @vl_Resultat output
	else
	begin
		/*A positionner la date de fin de la tete de bouchon */
		update EXP..FMC_GEN set debut = null, debut_prevu = @vl_DebutPrevu, fin = @va_DateFin_in, zz = zz + 1
		where numero = @vl_Tete and cle = @va_CleEvt_in and sit = @vl_Site

		exec @vl_Status = XZAE13 @va_NumEvt_in, @va_CleEvt_in,
					@vl_PosteOperateur, @vl_Resultat output
	end

	if @vl_Status != XDC_OK
		return @vl_Status
	if @vl_Resultat != XDC_OK
		return @vl_Resultat

	/* V1.3 */
	update EXP..FMC_HIS set numero=@va_NumEvt_in,cle=@va_CleEvt_in where numero=@va_NumEvt_in and cle=@va_CleEvt_in and  
			horodate_validation in (select max(horodate_validation)
                                        from EXP..FMC_HIS
                                        where numero = @va_NumEvt_in and cle = @va_CleEvt_in)

	
	
#ifdef CI
	/*A modifier la date de cloture pour quelle soit purgee le soir */
	update EXP..FMC_GEN set cloture = XDC_DATE_CLOTURE_PURGE, zz = zz +1
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in and cloture is not null
#endif

	return XDC_OK
go
