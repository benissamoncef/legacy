/*E*/
/*Fichier : @(#)xzap11sp.prc	1.14      Release : 1.14        Date : 10/22/09
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* volcic	28 Oct 1994	: Modification realisabe => XDC_VRAI (v1.2)
*				  Remplacement XZAPC par des XDC_ACT                
* gaborit	02 dec 1994	: ajout clause where
* C.T.		28 fev 1995	: Modif libelle retourne (v1.4)
* C.T.		28 fev 1995	: Correction erreur (v1.5)
* C.T.		28 mar 1995	: Modif libelle retourne (v1.6)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.7
* Niepceron	03 Dec 1998	: Ajout de SAD_EXP (1724) V1.9
* JMG		27/04/04	: ajout de SAD_PAL 1.10
* JMG		16/02/05	: ajout de la colonne explication en sortie 1.11
* JMG		25/10/05	: ajout PMVA BAF BAD 1.12
* PNI		27/10/05	: Filtre le plan d'action sur priorite < 255 DEM524 v1.13*
* JMG		02/07/09	: ajout domaine sur SAD_TUB DEM895
* VR		14/12/11 	: Ajout PRV (DEM/1016)
* JMG		10/06/16	: ajout eqt SAGA DEM1191
* PNI		05/01/18	: Degroupage et modif ordre tri v1.17
* LCL		02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.18 DEM1284
* JMG		15/08/18	: ajout SMS LOT25 DEM1310 1.19 1.20
* ABE		17/05/21	: Ajout PAU SONO DEM-SAE244 1.21	
* ABE		17/06/21	: Ajout IMU DEM-SAE155 1.22
* ABE		04/10/21	: Fix IMU DEM-SAE155 1.23
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		11/12/23	: Ajout PIC (DEM-483)
* PNI		22/02/24	: Mettre les BAF en première lignes des proposition SAE-588 1.24
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit en base un plan d'actions (Liste de Propositions)
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
drop proc XZAP11
go
create procedure XZAP11

	@va_NumPA_in		smallint
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
*
* ARGUMENTS EN SORTIE :
*
* XZAPT_PropPA 		va_PropPA_out
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Lit en base un plan d'actions (Liste de Propositions)
*
* MECANISMES :
*
------------------------------------------------------*/


as 
  
declare 
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Type tinyint,
   @vl_Status int, 
   @vl_Site tinyint

  
#ifdef HIST

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP11
			@va_NumPA_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP11
			@va_NumPA_in

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   create table #TMP_INTITULE (libelle	varchar(25),
			  suite_libelle	varchar(26) null,
			  ordre		smallint,
			  type		tinyint,
			  district	tinyint,
			  realisable	bit,
			  explication   varchar(250) null,
			  domaine	tinyint null,
			  remarque	varchar(250) null,
			  tri smallint,
				PR int,
				sens tinyint
			  )
  
   /*A
   ** Propositions PMV
   */
   
   select @vl_Type = XDC_ACT_PMV
   
   insert #TMP_INTITULE
   select distinct libelle,
		' ' + XDC_LIB_PMV + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,
		null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   
   from EXP..SAD_PMV , CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_PMV.priorite < 255 and 
	EXP..SAD_PMV.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_PMV.equipement and
	CFG..EQT_GEN.type = XDC_EQT_PMV and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
   order by CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   	
   /*A
   ** Propositions NAV
   */
   
   select @vl_Type = XDC_ACT_NAV
   
   insert #TMP_INTITULE
   select distinct libelle,
	' ' + XDC_LIB_NAV + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,
		null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_NAV, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_NAV.priorite < 255 and 
	EXP..SAD_NAV.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_NAV.equipement and
	CFG..EQT_GEN.type = XDC_EQT_NAV and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
   
   /*A
   ** Propositions PAL
   */

   select @vl_Type = XDC_ACT_PAL
  
   insert #TMP_INTITULE
   select distinct libelle,
                ' ' + XDC_LIB_PAL + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + 
		convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
                ordre,
                type = @vl_Type,
                district,
                realisable,
		explication,
		null,
		null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens

   from EXP..SAD_PAL, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_PAL.priorite < 255 and 
	EXP..SAD_PAL.numero = @va_NumPA_in and
        CFG..EQT_GEN.numero = EXP..SAD_PAL.equipement and
        CFG..EQT_GEN.type = XDC_EQT_PAL and
        CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute

	
   /*A
   ** Propositions TUB
   */
   
   select @vl_Type = XDC_ACT_TUNNEL
   
   insert #TMP_INTITULE
   select distinct libelle,
		' ' + XDC_LIB_TUBE + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		domaine,
		null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_TUB , CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_TUB.priorite < 255 and 
	EXP..SAD_TUB.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_TUB.equipement and
	CFG..EQT_GEN.type = XDC_EQT_TUBE and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
   	
   /*A
   ** Propositions ECH
   */
   
   select @vl_Type = XDC_ACT_ECHANGEUR
   
   insert #TMP_INTITULE
   select distinct libelle,
		' ' + XDC_LIB_ECH + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null, null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_ECH, CFG..EQT_GEN , CFG..RES_AUT
   where  EXP..SAD_ECH.priorite < 255 and 
	EXP..SAD_ECH.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_ECH.equipement and
	CFG..EQT_GEN.type = XDC_EQT_ECH and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
   	
   /*A
   ** Propositions PMVA
   */
   
   select @vl_Type = XDC_ACT_PMVA
   
   insert #TMP_INTITULE
   select distinct libelle,
	' ' + XDC_LIB_PMVA + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   
   from EXP..SAD_PMA , CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_PMA.priorite < 255 and 
	EXP..SAD_PMA.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_PMA.equipement and
	CFG..EQT_GEN.type = XDC_EQT_PMVA and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
   
   /* propositions SAGA*/
    select @vl_Type = XDC_ACT_SAGA
    insert #TMP_INTITULE
    select distinct libelle,
	' ' + "PMV SAGA "  + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7),
	 PR/1000.0),
		ordre,
		type = @vl_Type,
		district,
		realisable,
		explication,
		null, null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
    from EXP..SAD_SAGA , CFG..EQT_GEN, CFG..RES_AUT, CFG..EQT_SAGA
    where  EXP..SAD_SAGA.priorite < 255 and
	EXP..SAD_SAGA.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_SAGA.equipement and
	CFG..EQT_GEN.type = XDC_EQT_SAGA and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
	CFG..EQT_SAGA.numero  = CFG..EQT_GEN.numero and
	CFG..EQT_SAGA.type_saga <= XDC_SAGA_PMV_3_15_P

    insert #TMP_INTITULE
    select distinct libelle,
	' ' + "PMVA SAGA "  + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7),
	 PR/1000.0),
		ordre,
		type = @vl_Type,
		district,
		realisable,
		explication,
		null, null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
    from EXP..SAD_SAGA , CFG..EQT_GEN, CFG..RES_AUT, CFG..EQT_SAGA
    where  EXP..SAD_SAGA.priorite < 255 and
	EXP..SAD_SAGA.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_SAGA.equipement and
	CFG..EQT_GEN.type = XDC_EQT_SAGA and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
	CFG..EQT_SAGA.numero  = CFG..EQT_GEN.numero and
	CFG..EQT_SAGA.type_saga = XDC_SAGA_PMVA

    insert #TMP_INTITULE
    select distinct libelle,
	' ' + XDC_LIB_SAGA  + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7),
	 PR/1000.0),
		ordre,
		type = @vl_Type,
		district,
		realisable,
		explication,
		null, null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
    from EXP..SAD_SAGA , CFG..EQT_GEN, CFG..RES_AUT, CFG..EQT_SAGA
    where  EXP..SAD_SAGA.priorite < 255 and
	EXP..SAD_SAGA.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_SAGA.equipement and
	CFG..EQT_GEN.type = XDC_EQT_SAGA and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
	CFG..EQT_SAGA.numero  = CFG..EQT_GEN.numero and
	CFG..EQT_SAGA.type_saga > XDC_SAGA_PMV_3_15_P and
	CFG..EQT_SAGA.type_saga <> XDC_SAGA_PMVA and
	CFG..EQT_SAGA.type_saga <> XDC_SAGA_BAF and
	CFG..EQT_SAGA.type_saga <> XDC_SAGA_BRA


    insert #TMP_INTITULE
    select distinct libelle,
	' ' + "BAF SAGA "  + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7),
	 PR/1000.0),
		ordre,
		type = @vl_Type,
		district,
		realisable,
		explication,
		null, null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
    from EXP..SAD_SAGA , CFG..EQT_GEN, CFG..RES_AUT, CFG..EQT_SAGA
    where  EXP..SAD_SAGA.priorite < 255 and
	EXP..SAD_SAGA.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_SAGA.equipement and
	CFG..EQT_GEN.type = XDC_EQT_SAGA and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
	CFG..EQT_SAGA.numero  = CFG..EQT_GEN.numero and
	CFG..EQT_SAGA.type_saga = XDC_SAGA_BAF
   
    insert #TMP_INTITULE
    select distinct libelle,
	' ' + "NAV SAGA "  + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7),
	 PR/1000.0),
		ordre,
		type = @vl_Type,
		district,
		realisable,
		explication,
		null, null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
    from EXP..SAD_SAGA , CFG..EQT_GEN, CFG..RES_AUT, CFG..EQT_SAGA
    where  EXP..SAD_SAGA.priorite < 255 and
	EXP..SAD_SAGA.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_SAGA.equipement and
	CFG..EQT_GEN.type = XDC_EQT_SAGA and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute and
	CFG..EQT_SAGA.numero  = CFG..EQT_GEN.numero and
	CFG..EQT_SAGA.type_saga = XDC_SAGA_BRA
   /*A
   ** Propositions PRV
   */
   
   select @vl_Type = XDC_ACT_PRV
   
   insert #TMP_INTITULE
   select distinct libelle,
	' ' + XDC_LIB_PRV + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_PRV, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_PRV.priorite < 255 and 
	EXP..SAD_PRV.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_PRV.equipement and
	CFG..EQT_GEN.type = XDC_EQT_PRV and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute

	/* Propositions PIC */
	select @vl_Type = XDC_ACT_PIC
	
	insert #TMP_INTITULE
	select distinct libelle,
	' ' + XDC_LIB_PIC + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
			ordre,
			type = @vl_Type,
			district,
			realisable,
		explication,
		null,null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
			
	from EXP..SAD_PIC, CFG..EQT_GEN, CFG..RES_AUT
	where  EXP..SAD_PIC.priorite < 255 and 
	EXP..SAD_PIC.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_PIC.equipement and
	CFG..EQT_GEN.type = XDC_EQT_PIC and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute

   /*A
   ** Propositions CFE
   */
   
   select @vl_Type = XDC_ACT_CFE
   
   insert #TMP_INTITULE
   select distinct libelle,
	' ' + XDC_LIB_CFE + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_CFE, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_CFE.priorite < 255 and 
	EXP..SAD_CFE.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_CFE.equipement and
	CFG..EQT_GEN.type = XDC_EQT_CFE and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
	
   /*A
   ** Propositions BAF
   */
   
   select @vl_Type = XDC_ACT_BAF
   
   insert #TMP_INTITULE
   select distinct libelle,
	' ' + XDC_LIB_BAF + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,null,255+0+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_BAF, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_BAF.priorite < 255 and 
	EXP..SAD_BAF.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_BAF.equipement and
	CFG..EQT_GEN.type = XDC_EQT_BAF and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
   
   /*A
   ** Propositions BAD
   */
   
   select @vl_Type = XDC_ACT_BAD
   
   insert #TMP_INTITULE
   select distinct libelle,
		' ' + XDC_LIB_BAD + ' ' + convert(varchar(4), CFG..RES_AUT.nom) + '-' + convert(char(1), sens) + '-' + convert(char(7), PR/1000.0),
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable,
		explication,
		null,null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   		
   from EXP..SAD_BAD, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_BAD.priorite < 255 and 
	EXP..SAD_BAD.numero = @va_NumPA_in and
	CFG..EQT_GEN.numero = EXP..SAD_BAD.equipement and
	CFG..EQT_GEN.type = XDC_EQT_BAD and
	CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute
	
   /*A
   ** Propositions PAU SONO
   */

   select @vl_Type = XDC_ACT_PAU
  
   insert #TMP_INTITULE
   select distinct libelle,
                ' ' + XDC_LIB_PAU + ' ' + 'TUNNEL ' +convert(varchar(4), CFG..RES_AUT.nom),
                ordre,
                type = @vl_Type,
                district,
                realisable,
		explication,
		null,
		null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens

   from EXP..SAD_SONO, CFG..EQT_GEN, CFG..RES_AUT
   where  EXP..SAD_SONO.priorite < 255 and 
	EXP..SAD_SONO.numero = @va_NumPA_in and
        CFG..EQT_GEN.numero = EXP..SAD_SONO.equipement and
        CFG..EQT_GEN.type = XDC_EQT_PAU and
        CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute


   /*A
   ** Propositions IMU
   */
   select @vl_Type = XDC_ACT_IMU
   insert #TMP_INTITULE
   select distinct libelle,
                ' ' + XDC_LIB_IMU + ' ' + 'TUNNEL ' +convert(varchar(4), CFG..RES_AUT.nom),
                ordre,
                type = @vl_Type,
                district,
                realisable,
  		explication,
  		null,
  		null,255+@vl_Type+CFG..EQT_GEN.autoroute,CFG..EQT_GEN.PR,CFG..EQT_GEN.sens
   
   from EXP..SAD_IMU, CFG..EQT_GEN, CFG..RES_AUT
   where EXP..SAD_IMU.priorite < 255 and
 	EXP..SAD_IMU.numero = @va_NumPA_in and
        CFG..EQT_GEN.numero = EXP..SAD_IMU.equipement and
        CFG..EQT_GEN.type = XDC_EQT_IMU and
        CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute


   /*A
   ** Propositions TFM
   */
   
   select @vl_Type = XDC_ACT_TFM
   
   insert #TMP_INTITULE
   select distinct libelle,
		null,
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable = XDC_VRAI,
		null,
		null,null,ordre,0,0
   		
   from EXP..SAD_TFM 
   where  numero = @va_NumPA_in
   	
   /*A
   ** Propositions FAX
   */
   
   select @vl_Type = XDC_ACT_FAX
   
   if exists (select * from EXP..SAD_DST_FAX
	where numero = @va_NumPA_in and sms <> 1)
	begin
   insert #TMP_INTITULE
   select distinct libelle,
		null,
   		ordre,
   		type = @vl_Type,
   		district,
   		XDC_VRAI,
		explication,
		null,null,ordre,0,0
   
   from EXP..SAD_FAX 
   where  numero = @va_NumPA_in 
   	end

   	
   /*A								
   ** Propositions SMS
   */
   
   select @vl_Type = XDC_ACT_SMS
   
   if exists (select * from EXP..SAD_DST_FAX
	where numero = @va_NumPA_in and sms = 1)
	begin
   insert #TMP_INTITULE
   select distinct libelle,
		null,
   		ordre,
   		type = @vl_Type,
   		district,
   		XDC_VRAI,
		explication,
		null,null,ordre,0,0
   
   from EXP..SAD_FAX 
   where  numero = @va_NumPA_in
   end

   /*A
   ** Propositions APPEL
   */
   
   select @vl_Type = XDC_ACT_APPEL
   
   insert #TMP_INTITULE
   select distinct libelle,
		' ' + nom,
   		ordre,
   		type = @vl_Type,
   		district,
   		realisable = XDC_VRAI,
		explication,
		null, remarque,ordre,0,0
   		
   from EXP..SAD_APL, CFG..TYP_AST 
   where  EXP..SAD_APL.numero = @va_NumPA_in and
	CFG..TYP_AST.numero = EXP..SAD_APL.type_d_astreinte

   /*A
   ** Propositions EXPORT
   */
#ifdef CI  
   select @vl_Type = XDC_ACT_EXP
   
   insert #TMP_INTITULE
   select distinct XDC_ACT_LIB_EXP,
   		null,
		ordre,
   		type = @vl_Type,
   		district,
   		realisable = XDC_VRAI,
		null,
		null, null,ordre,0,0
   		
   from EXP..SAD_EXP
   where numero = @va_NumPA_in 
#endif 



/*A
   ** Retourne la liste des Propositions d'un PA
   ** ordonnee par district et par ordre et autoroute PR sens
   */
   

    select 
	libelle + suite_libelle, 
	explication,
	ordre, 
	type, 
	district, 
	realisable ,
	domaine,
	remarque
   from #TMP_INTITULE
   order by district desc, tri,PR,sens
  
   
 
   
  return XDC_OK
#endif
        
go
