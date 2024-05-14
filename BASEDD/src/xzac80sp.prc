/*E*/
/*  Fichier : @(#)xzac80sp.prc	1.17      Release : 1.17        Date : 03/05/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac80.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  03/03/2004 :  Creation
* JMG	15/09/05 : action est en succes (au lieu de lancement) 1.1
* RGR 09/10/2020 : Ajout drop
* LCL 03/10/2023 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Insere une action DATEX si la FMC associee existe dans MIGRAZUR
* 
* Sequence d'appel
* SP	XZAC80_Ajouter_Action_DATEX
* 
* Arguments en entree
* char(10)      va_emetteur_in
* char(35)      va_reference_in
* char(35)      va_lnk_in
* XDY_Horodate	va_horodate_debut_in
* XDY_Horodate	va_horodate_fin_in
* char(250)     va_libelle_in
*
* Arguments en sortie
* XDY_Entier	va_numeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Verifier l'existence de la FMC associee
* 
* Inserer dans la table ACT_GEN et la table ACT_ADA.
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC80' and type = 'P')
  drop procedure XZAC80
go

create procedure XZAC80
	@va_emetteur_in         char(10)	= null,
	@va_reference_in        char(35)	= null,
	@va_lnk_in              char(35)	= null,
	@va_horodate_debut_in	datetime	= null,
	@va_horodate_fin_in	datetime	= null,
	@va_libelle_in          char(250)       = null,
	@va_numeroAction_out	int		= null	output

as
declare @vl_evt int,
	@vl_cle tinyint,
	@vl_SiteCI tinyint

	if @va_horodate_debut_in = XDC_DATE_NULLE select @va_horodate_debut_in = null
	if @va_horodate_fin_in = XDC_DATE_NULLE select @va_horodate_fin_in = null

	/*A controle des parametres obligatoires */
	if @va_emetteur_in = null or @va_reference_in = null or @va_lnk_in = null or 
	   @va_horodate_debut_in = null or @va_libelle_in = null
		return XDC_ARG_INV
	
	/* Recherche du site principal */
	select @vl_SiteCI=numero from CFG..RES_DIS where type=XDC_TYPEM_SD
	if @@rowcount != 1
		return XDC_NOK

        /* Recherche de la FMC associee */
	select @vl_evt=evenement,@vl_cle=cle from EXP..ADA_SIT
	where emetteur=@va_emetteur_in and reference=@va_lnk_in and evenement<>null and cle<>null
	if @vl_evt<>null and @vl_cle<>null
	begin
	   /* La FMC associee existe */
	   /* inserer ou mettre a jour l'action DATEX */
	   select @va_numeroAction_out=null
	   select @va_numeroAction_out = numero from EXP..ACT_ADA where emetteur=@va_emetteur_in and reference=@va_reference_in
	   if @va_numeroAction_out=null
	   begin

	      /* calcul du numero d'action a inserer */
	      select @va_numeroAction_out = isnull(max(numero), 0) + 1 from EXP..ACT_GEN
	      where sit = vl_SiteCI

	      begin tran
		   insert EXP..ACT_GEN ( numero, sit, evenement, cle, type, 
		   heure_lancement, heure_succes, heure_fin, prioritaire, distance_evt, operateur, sit_origine)
		   values (@va_numeroAction_out, vl_SiteCI, @vl_evt, @vl_cle,XDC_ACT_DATEX,
		   @va_horodate_debut_in, @va_horodate_debut_in, @va_horodate_fin_in, XDC_NON, null, 0, vl_SiteCI)

                   insert EXP..ACT_ADA (numero,libelle,emetteur,reference,sit)
		   values (@va_numeroAction_out,@va_libelle_in,@va_emetteur_in,@va_reference_in,vl_SiteCI)
	      commit tran
           end
	   else
	   begin
	      begin tran
		   update EXP..ACT_GEN set heure_lancement=@va_horodate_debut_in, heure_fin=@va_horodate_fin_in, 
		   heure_succes=@va_horodate_debut_in where numero=@va_numeroAction_out and sit=vl_SiteCI

		   update EXP..ACT_ADA set libelle = @va_libelle_in
		   where numero=@va_numeroAction_out and sit=vl_SiteCI
	      commit tran
           end
        end
	return XDC_OK
go
