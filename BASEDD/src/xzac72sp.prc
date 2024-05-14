/*E*/
/*  Fichier : $Id: xzac72sp.prc,v 1.3 2020/10/09 14:12:22 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/10/09 14:12:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac72sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Creation action "exporter intergestionnaire"
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	27/10/1998 : creation
* PNI	02/02/15 : Creation de l'action au sein d'une transaction  1.1.2
* RGR   09/10/20 : Ajout drop 
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC72_Exporter
*
* Arguments en entree
* XDY_Entier		va_NumeroEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Octet		va_Gravite_in
* XDY_Octet		va_Trafic_in
* XDY_Horodate		va_HeureLancement_in
* XDY_Operateur		va_Operateur_in
*
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Creation action "exporter intergestionnaire"
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC72' and type = 'P')
  drop procedure XZAC72
go

create procedure XZAC72
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_Gravite_in			tinyint		= null,
	@va_Trafic_in			tinyint		= null,
	@va_HeureLancement_in		datetime	= null,
	@va_Operateur_in		T_OPERATEUR	= null,
	@va_NumeroAction_out		int		= null	output
	
as

declare @vl_type	tinyint,
	@vl_numero	int,
	@vl_sit		tinyint,
	@vl_SiteCI  tinyint

	select @vl_SiteCI=numero from CFG..RES_DIS where type=XDC_TYPEM_SD
 
	/*A verifier l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK


begin tran
		/*A recherche du numero d'actio suivant */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 
		from EXP..ACT_GEN
		where sit = @vl_SiteCI

		/*A inserer une actio de type PMV sur fourgon */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		values( @va_NumeroAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_EXP, @va_HeureLancement_in,
			null, null, XDC_NON, null,
			null, @va_Operateur_in, @vl_SiteCI, null, @vl_SiteCI)
commit tran

		/*A inserer le complement de l'actio */
		if exists (select * from EXP..ACT_GEN
				where numero = @va_NumeroAction_out and sit = @vl_SiteCI)
			insert EXP..ACT_EXP_ADA (actio, sit , gravite,trafic)
			values (@va_NumeroAction_out, @vl_SiteCI, @va_Gravite_in , @va_Trafic_in )
		else	return XDC_NOK
		
		/* mise � fin des messages precedents */
		update EXP..ACT_GEN
		set heure_fin=@va_HeureLancement_in
		where evenement=@va_NumEvt_in
		  and cle=@va_CleEvt_in
		  and heure_fin=null
		  and numero<>@va_NumeroAction_out
		  and type=XDC_ACT_EXP

	
		select @vl_type=type 
		from EXP..FMC_GEN
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		/* Insertion de l'evt dans FMC_ADA por l'export */
		insert EXP..FMC_ADA values (@va_NumEvt_in,@va_CleEvt_in,@vl_type)

		/* Insertion de l'action TFM si elle existe dans TFM_ADA */
		select @vl_numero=numero,@vl_sit=sit from EXP..ACT_GEN
		where type=XDC_ACT_TFM and
		 evenement=@va_NumEvt_in and cle = @va_CleEvt_in and
		 heure_fin = null

		if @@rowcount=1 and not exists (select * from EXP..TFM_ADA where actio=@vl_numero and sit=@vl_sit)
			insert EXP..TFM_ADA values (@vl_numero,@vl_sit)

	return XDC_OK
go
