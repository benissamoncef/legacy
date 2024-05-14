/*  Fichier : $Id: asql_t02_fmc_his.trg,v 1.15 2019/06/04 14:41:53 gesconf Exp $        $Revision: 1.15 $        $Date: 2019/06/04 14:41:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t02_fmc_his.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FMC_HIS
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  17/10/94        : Creation
* B.G.  12/01/95        : Ajout de l'appel a xzah20 pour dai et radt (1.2)
* C.T.  20/02/95        : Fonctionalite transferee dans xzah;30 et remplacee
*                         insertion dans FMC_SYN (V 1.3)
* B.G.  03/04/1995      : insertion dans FMC_SYN pour tous types sauf poste operateur (1.4)
* P.N   01/06/1997      : ajout appel XZAD09
* P.N   26/09/1997      : appel XZAD09 si site enrichisseur 1.6
* P.N   27/04/1998      : filtre sur evt null 1.7
* JMG	16/01/2001	: on remplit la table FMC_HIS_DER (WEB) 1.8
* F.L   05/05/2004      : mise a 0 de FMC_HIS_DER.diffusion_datex (SAGA) 1.10
* P.N   22/06/2007      : Ajout site DP pour migration Peyruis en niv2 1.11 DEM641
* JMG           02/10/09        : secto DEM887 1.12
* JMG	28/11/16 : evt majeur lot 23 1.13
* JMC 	09/03/2017	: REGIONALISQATION - DEM1220
* JPL	28/03/19	: Ajout 'ecoulement' � FMC_HIS et FMC_HIS_DER (DEM 1326)  1.15
* LCL	26/09/2023	: CNA DEM-483
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM sur insert ou update
*
* Code retour
* 
* nEant
*
* Conditions d'utilisation
* base EXP
---------------------------------------------------*/

use EXP
go

drop trigger fmc_his_iu_trg
go

create trigger fmc_his_iu_trg
on FMC_HIS
for insert,update
as

/*A DEclaration des variables locales */

        declare @vl_NumEvt      int,
                @vl_CleEvt      tinyint,
                @vl_Autoroute   tinyint,
                @vl_PR          int,
                @vl_Sens        tinyint,
                @vl_Type        smallint,
                @vl_Site        T_SITE,
                @vl_SiteEnrich  T_SITE,
		@vl_horodate_validation datetime,
		@vl_confirme_ou_signale bit,
		@vl_type_anterieur smallint,
		@vl_type_FMC_valide smallint,
		@vl_pt_car tinyint,
		@vl_num_pt_car tinyint,
		@vl_position tinyint,
		@vl_VR tinyint,
		@vl_VM2 tinyint,
		@vl_VM1 tinyint,
		@vl_VL tinyint,
		@vl_BAU tinyint,
		@vl_VR_I tinyint,
		@vl_VM2_I tinyint,
		@vl_VM1_I tinyint,
		@vl_VL_I tinyint,
		@vl_BAU_I tinyint,
		@vl_BAU_etroite tinyint,
		@vl_BAU_I_etroite tinyint,
		@vl_sit T_SITE,
		@vl_longueur int,
		@vl_diffusion_datex tinyint,              /* Ajout SAGA */
               	@vl_majeur tinyint,
               	@vl_ecoulement tinyint
                
/*A rEcupEration des valeurs insErEes */
        select  @vl_NumEvt=numero,
                @vl_CleEvt=cle,
                @vl_Autoroute=autoroute,
                @vl_PR=PR,
                @vl_Sens=sens,
		@vl_horodate_validation=horodate_validation,
		@vl_confirme_ou_signale=confirme_ou_signale,
		@vl_type_anterieur=type_anterieur,
		@vl_type_FMC_valide=type_FMC_valide,
		@vl_pt_car=point_caracteristique,
		@vl_num_pt_car=numero_point_caracteristique,
		@vl_position=position_sur_aire_ou_echangeur,
		@vl_VR=VR,
		@vl_VM2=VM2,
		@vl_VM1=VM1,
		@vl_VL=VL,
		@vl_BAU=BAU,
		@vl_VR_I=VR_I,
		@vl_VM2_I=VM2_I,
		@vl_VM1_I=VM1_I,
		@vl_VL_I=VL_I,
		@vl_BAU_I=BAU_I,
		@vl_BAU_etroite=BAU_etroite,
		@vl_BAU_I_etroite=BAU_I_etroite,
		@vl_sit=sit,
		@vl_longueur=longueur,
		@vl_majeur = majeur,
		@vl_ecoulement = ecoulement
        from inserted

/*A Rechercher le site courant */
        if @@servername = XDC_SQLCI
                select @vl_Site = XDC_CI
        else if @@servername = XDC_SQLCA
                select @vl_Site = XDC_CA
        else if @@servername = XDC_SQLVC
                select @vl_Site = XDC_VC
        else if @@servername = XDC_SQLDP
                select @vl_Site = XDC_DP

/*A Rechercher le type de l'evenement */
        select  @vl_Type=type,
                @vl_SiteEnrich=sit
        from FMC_GEN
        where   @vl_NumEvt=numero and
                @vl_CleEvt=cle

/*A Mettre a jour de la table des franchissement de seuil 
    et Inserer l'evenement valide dans FMC_SYN */

        if @vl_Type != XZAEC_FMC_PosteOperateur  AND
	   @vl_NumEvt != NULL AND  @vl_CleEvt != NULL
        begin
                if @vl_Site=@vl_SiteEnrich
                begin
                        exec PRC..XZAD09 @vl_NumEvt, @vl_CleEvt, @vl_Type, @vl_Autoroute, @vl_PR, @vl_Sens
                end
                insert EXP..FMC_SYN ( evenement , cle)
                values( @vl_NumEvt, @vl_CleEvt)
        end
		/* Ajout SAGA */
		/* diffusion_datex est mis a 0 sauf si la validation vient de l'exterieur            */
		/* c'est a dire la date de validation de ADA_SIT et la date de validation de FMC_HIS */
		/* sont identiques                                                                   */
                select @vl_diffusion_datex = 0

#ifdef CI
/*		if exists (select * from EXP..ADA_SIT where evenement=@vl_NumEvt and cle=@vl_CleEvt and horodate=@vl_horodate_validation)
		begin
		   select @vl_diffusion_datex = 1
                end
*/
#endif

		if exists (select * from FMC_HIS_DER where numero=@vl_NumEvt and cle = @vl_CleEvt)
		begin
			update FMC_HIS_DER
			set horodate_validation=@vl_horodate_validation,
			    confirme_ou_signale=@vl_confirme_ou_signale,
			    type_anterieur=@vl_type_anterieur,
			    type_FMC_valide=@vl_type_FMC_valide,
			    autoroute=@vl_Autoroute,
			    PR=@vl_PR,
			    sens=@vl_Sens,
			    point_caracteristique=@vl_pt_car,
			    numero_point_caracteristique=@vl_num_pt_car,
			    position_sur_aire_ou_echangeur=@vl_position,
			    VR=@vl_VR,
			    VM2=@vl_VM2,
			    VM1=@vl_VM1,
			    VL=@vl_VL,
			    BAU=@vl_BAU,
			    VR_I=@vl_VR_I,
			    VM2_I=@vl_VM2_I,
			    VM1_I=@vl_VM1_I,
			    VL_I=@vl_VL_I,
			    BAU_I=@vl_BAU_I,
			    BAU_etroite=@vl_BAU_etroite,
			    BAU_I_etroite=@vl_BAU_I_etroite,
			    sit=@vl_sit,
			    majeur = @vl_majeur,
			    longueur=@vl_longueur,
			    ecoulement=@vl_ecoulement,
			    diffusion_datex=@vl_diffusion_datex    /* ajout SAGA */
			where numero=@vl_NumEvt and cle = @vl_CleEvt
		end
		else if @vl_NumEvt  != NULL
		begin
			insert FMC_HIS_DER (numero, cle, horodate_validation,confirme_ou_signale,type_anterieur,type_FMC_valide,autoroute,PR,sens,point_caracteristique,numero_point_caracteristique,position_sur_aire_ou_echangeur,VR,VM2,VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite,BAU_I_etroite,sit,longueur,diffusion_datex, majeur, ecoulement)
			values (@vl_NumEvt,@vl_CleEvt,@vl_horodate_validation,@vl_confirme_ou_signale,@vl_type_anterieur,@vl_type_FMC_valide,@vl_Autoroute,@vl_PR,@vl_Sens,@vl_pt_car,@vl_num_pt_car,@vl_position,@vl_VR,@vl_VM2,@vl_VM1,@vl_VL,@vl_BAU,@vl_VR_I,@vl_VM2_I,@vl_VM1_I,@vl_VL_I,@vl_BAU_I,@vl_BAU_etroite,@vl_BAU_I_etroite,@vl_sit,@vl_longueur,@vl_diffusion_datex, @vl_majeur, @vl_ecoulement)
		end
go
