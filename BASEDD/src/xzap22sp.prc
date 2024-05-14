/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* volcic	15 Nov 1994	: Ajout gestion file d'attente (v1.5)
* gaborit	01 Dec 1994	: Correction (v1.6)
* gaborit	26 jan 1995	: ajout file prio 0 (v1.7)
* C.T.    	14 Mar 1995	: Modif longueur texte reveil (v1.8)
* C.T.    	28 Mar 1995	: Modif texte reveil (v1.9)
* C.T.    	16 Fev 1996	: Modif texte des propositions pmv et bra (v1.10)
* P.N.    	10 Mar 1997	: Modif appel XZAP;02 (v1.11)
* JMG		08 jan 1998	: gestion multi-sites (dem/1536) 1.12
* JMG		05 mar 1998	: ajout colonne sit dans EQT_FIL (dem/1595) 1.13
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.14
* JMG		16 mai 2000	: test etat evt avant reveil ou extinction 1.15
* JMG		25/10/05	: ajout PMVA BAF BAD 1.16
* JMG		02/10/09	: secto DEM887 1.17
* VR		14/12/11 	: Ajout PRV (DEM/1016)
* JMG		11/11/12	: regul vitesse PMVA 
* JMG		27/02/17 	: regio
* PNI		12/10/17 	: modif appel XZAP;02
* LCL		02/03/18	: ajout controleurs de feux CTRL_FEUX 1.22
* JMG		03/12/19 	: plus de EQT_FIL ici DEM1347 1.23
* GGY		26/09/23 	: Ajout district CA (DEM483)
* JPL		20/10/23	: Pas de FMC d'Essai ou Exercice en attente dans EQT_FIL (SAE_525)  1.24
* GGY		11/12/23	: Ajout PIC (DEM-483)
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Indique que le SAD a termine le calcul et l'envoi du plan d'actions.
*  (declencher maj file d'attente+gestion fin d'actio+reveil)
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAP;22

	@NumPA_In		smallint
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* Au niveau du C.I., pas de gestion de file d'attente donc seul PA pret est signale
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/

as
/*
#ifndef CI
*/

declare @vl_Evt			int,
		@vl_Cle			tinyint,
		@vl_PO			smallint,
		@vl_Date		datetime,
		@vl_Eqt			smallint,
		@vl_Type		tinyint,
		@vl_Evt2		int,
		@vl_Cle2		tinyint,
		@vl_texte		char(100),
		@vl_site		tinyint,
		@vl_ordre		smallint,
		@vl_PR			T_PR, 
		@vl_Sens 		T_SENS, 
		@vl_Autoroute 	T_AUTOROUTE, 
		@vl_Intitule 	varchar(50),
		@vl_Status 		int,
		@vl_horodate 	datetime

#ifdef HIST
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;22
			@NumPA_In	

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;22
			@NumPA_In	

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else

/*A Recherche de l'evenement associe au plan d'action */

select	@vl_Evt = evenement,
	@vl_Cle = cle,
	@vl_PO = machine_operateur,
	@vl_Date = horodate
from	EXP..SAD_PLN where numero = @NumPA_In

if @@rowcount<>1
begin
	delete EXP..EQT_FIL_TRV
	return XDC_NOK
end

/*A effacement de l'evenement de la file d'attente pour les equipements non desires */
/*
delete	EXP..EQT_FIL
from	EXP..EQT_FIL_TRV FAT
where	EXP..EQT_FIL.equipement = FAT.equipement
  and	EXP..EQT_FIL.type_equipement = FAT.type_equipement
  and	EXP..EQT_FIL.evenement = @vl_Evt
  and	EXP..EQT_FIL.cle = @vl_Cle
*/

/*A Recherche du site local */
select @vl_site = numero from CFG..RES_DIS
      where serveur = @@servername

/*
begin
	delete EXP..EQT_FIL_TRV
	return XDC_NOK
end
*/
/*A Recherche des equipements de la file d'attente de travail pour lesquels il existe une action */

declare Act cursor
for select equipement,type_equipement
from EXP..EQT_FIL_TRV

open Act

fetch Act into @vl_Eqt, @vl_Type
while @@sqlstatus = 0
begin
	update EXP..ACT_GEN
	set priorite=0
	where evenement = @vl_Evt
	  and cle	= @vl_Cle
	  and equipement = @vl_Eqt
	  and type	= @vl_Type
	  and sit	= @vl_site
	  and heure_fin = null
	if @@rowcount<>0
	/* il existe une action non finie */
	begin
		/* on remet l'equipement en file d'attente avec priorite 0 */
		select 1 from EXP..FMC_GEN
		where numero = @vl_Evt  and  cle = @vl_Cle
		and origine_creation not like '%ESSAI%'
		and origine_creation not like '%EXERCICE%'
		and origine_creation not like '%SUPPRIMER%'

		if @@rowcount = 1
		begin
			insert EXP..EQT_FIL (
				     evenement,
				     cle,
				     type_equipement,
				     equipement,
				     distance,
				     priorite,
				     sit
				     )
			values (
				@vl_Evt,
				@vl_Cle,
				@vl_Type,
				@vl_Eqt,
				0,
				0,
				@vl_site
				)
		end

	/*A on recherche si un autre evt souhaite utiliser l'eqt */
		select @vl_Evt2=null,@vl_Cle2=null
		
		set rowcount 1 -- on ne prend que le plus prioritaire
		
		select @vl_Evt2=evenement,@vl_Cle2=cle
		from EXP..EQT_FIL
		where equipement = @vl_Eqt
		  and type_equipement = @vl_Type
		  and priorite<>0
		order by priorite desc,distance asc
		
		set rowcount 0

		/*verifier si l'evt est en cours*/
		select @vl_horodate = null
		select @vl_horodate = fin
		from EXP..FMC_GEN
		where numero = @vl_Evt2 and cle = @vl_Cle2

		/*si pas d'evenement ou evt termine*/
		if @@rowcount = 0 or @vl_horodate <> null
			/*proposer extinction*/
			select @vl_Evt2=null
		
		if @vl_Evt2<>null
		begin
		/*A on reveille l'evt */
		
			select 	@vl_Autoroute=autoroute,
				@vl_PR=PR,
				@vl_Sens=sens
			from CFG..EQT_GEN
			where numero=@vl_Eqt and type=@vl_Type

			/* nommage de l'equipement */
			exec XZAC;60 @vl_Type, @vl_Autoroute, @vl_PR, @vl_Sens, @vl_Intitule output

			select @vl_texte=XDC_REV_LIB_EQT + @vl_Intitule + XDC_REV_LIB_EQT_SUITE
			
			exec PRC..XZAR;03 @vl_Evt2,@vl_Cle2,null,@vl_texte,@vl_site
		end
		else
		begin
		/*A on propose d'eteindre l'equipement */
			
			select @vl_ordre=isnull(@vl_ordre,256)-1
		
			if @vl_Type = XDC_EQT_PMV
			exec PRC..XZAP;02	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
						XDC_CHAINE_VIDE, --ligne 1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --ligne3
						XDC_CHAINE_VIDE, --alter1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --alter3
						"EXTINCTION",
						XDC_CHAINE_VIDE, --texte picto
						XDC_PICTO_PASPICTO,
						null,
						null,
						XDC_VRAI,
						0
			else
			if @vl_Type = XDC_EQT_PMVA
			exec PRC..XZAP28	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
						XDC_CHAINE_VIDE, --ligne 1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --ligne3
						XDC_CHAINE_VIDE, --ligne4
						XDC_CHAINE_VIDE, --ligne5
						XDC_CHAINE_VIDE, --alter1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --alter3
						XDC_CHAINE_VIDE, --alter4
						XDC_CHAINE_VIDE, --alter5
						0,	-- pas flash
						0,
						0,
						0,
						0,
						"EXTINCTION",
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_PRV
			exec PRC..XZAP82	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE PRV N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE PRV N'EST PLUS UTILE POUR L'EVENEMENT",
						XDC_CHAINE_VIDE, --ligne 1
						"L130",
						"EXTINCTION",
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_PIC
			exec PRC..XZAP82	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE PIC N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE PIC N'EST PLUS UTILE POUR L'EVENEMENT",
						XDC_CHAINE_VIDE, --ligne 1
						"",
						"",
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_CFE
			exec PRC..XZAP83	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE CFE N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE CFE N'EST PLUS UTILE POUR L'EVENEMENT",
						0, -- Mode horaire nominal
						"MODE NOMINAL",
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_NAV
			exec PRC..XZAP;03	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
						"LIBERATION",
						XDC_NAV_RENTREE,
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_BAF
			exec PRC..XZAP29	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE BAF N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE BAF N'EST PLUS UTILE POUR L'EVENEMENT",
						"LIBERATION",
						0, -- module 1
						0,
						0,
						0,
						0,
						0, -- module 6
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_BAD
			exec PRC..XZAP33	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE BAD N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE BAD N'EST PLUS UTILE POUR L'EVENEMENT",
						"LIBERATION",
						"L_GLOVR1",
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_TUB
			exec PRC..XZAP;04	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CE TUNNEL N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CE TUNNEL N'EST PLUS UTILE POUR L'EVENEMENT",
						"EXTINCTION TUNNEL",
						XDC_TUB_ETEINT,
						XDC_VRAI
			else
			if @vl_Type = XDC_EQT_ECH
			exec PRC..XZAP;05	@NumPA_In,
						@vl_ordre,
						@vl_site,
						"CET ECHANGEUR N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_Eqt,
						"CET ECHANGEUR N'EST PLUS UTILE POUR L'EVENEMENT",
						"EXTINCTION ECHANGEUR",
						XDC_ECH_ETEINT,
						XDC_VRAI
						
		end
	end
	fetch Act into @vl_Eqt, @vl_Type
end


delete EXP..EQT_FIL_TRV
/*
#endif
*/
return XDC_OK
#endif
go
