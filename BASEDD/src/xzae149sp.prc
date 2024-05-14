/*E*/
/*  Fichier : @(#)xzae149sp.prc	1.1      Release : 1.1        Date : 02/26/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae149sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos d'une situation strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* F.Lizot       01 Avr 2004 : traitement du cas ou l'enregistrement exsite deja (SAGA)
*                             + gestion du droit de propriete V1.2
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE149_Stocke_Synthese_Strada
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* insertion dans la table ADA_TXT_SYN
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE149' and type = 'P')
	drop procedure XZAE149
go

create procedure XZAE149
	@va_emetteur_in		char(10)=null,
	@va_reference_in 	char(35)=null,
	@va_morceau_in		tinyint=null,
	@va_texte_in		char(250)=null
	
as
declare	@vl_morceau smallint,
	@vl_date	datetime,
	@vl_evt		int,
	@vl_cle		tinyint,
	@vl_propriete_fmc bit,  /* Ajout SAGA */
	@vl_propriete_queue bit, /* Ajout SAGA */
	@vl_SiteMaitre tinyint
 
	if @va_emetteur_in = null or 
		@va_reference_in = null or 
		@va_morceau_in = null
	   return XDC_ARG_INV

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD
	
        /* Remplacement de insert EXP..ADA_TXT_SYN par insert ou update si l'enregistrement existe d�j� */
        /* Modif SAGA                                                                                   */
        if exists (select * from EXP..ADA_TXT_SYN where emetteur=@va_emetteur_in and reference=@va_reference_in and morceau=@va_morceau_in)
        begin
	   update EXP..ADA_TXT_SYN set texte=@va_texte_in 
	   where emetteur=@va_emetteur_in and reference=@va_reference_in and morceau=@va_morceau_in
        end
	else
	begin
	   insert EXP..ADA_TXT_SYN (emetteur,reference,morceau,texte)
	   values  (@va_emetteur_in,@va_reference_in,@va_morceau_in,@va_texte_in)
	end
	
	select @vl_date=horodate,
	 	@vl_evt=evenement,
		@vl_cle=cle
	from  EXP..ADA_SIT 
	where	emetteur=@va_emetteur_in and reference= @va_reference_in
	
	if ( @vl_evt != null and @vl_cle!=null)
	begin
	      /* Ajout SAGA : Gestion du droit de propriete dans le cas DDE83 */
	      select @vl_propriete_fmc   = XDC_NON
	      select @vl_propriete_queue = XDC_NON
	      if @va_emetteur_in = "frd83"
	         exec XZAE551 @vl_evt,@vl_cle,@vl_propriete_fmc output,@vl_propriete_queue output

	      if @vl_propriete_fmc=XDC_NON or @vl_propriete_queue=XDC_NON 
	      begin
		select @vl_morceau=isnull(max(numero),0) from EXP..FMC_COM where evenement=@vl_evt and cle=@vl_cle
		insert EXP..FMC_COM
			(evenement,cle,numero,horodate_validation,texte,sit)
 		values (@vl_evt,@vl_cle,@vl_morceau+1,@vl_date,@va_texte_in,@vl_SiteMaitre)
	      end
	end
	return XDC_OK
go

