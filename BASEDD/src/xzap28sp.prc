/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* VR	08/06/12	Ajout destinations DTP PMA (DEM/1014 PMA)
* JMG	30/12/19	EQT_FIL DEM1347 1.3
* JMG	02/11/20 	cle double EQT_FIL 1.3
* GGY	26/09/23	Ajout district CA (DEM483)
* JPL	19/10/23 : Pas de FMC d'Essai ou Exercice en attente dans EQT_FIL (SAE_525)  1.4
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP28' and type = 'P')
	drop procedure XZAP28
go	


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'affichage PMVA a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP28

	@NumPA_In			smallint,
	@Ordre_In			smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In			smallint,
	@Explication_In		char(250),
	@Texte1PMV_In		char(15),
	@Texte2PMV_In		char(15),
	@Texte3PMV_In		char(15),
	@Texte4PMV_In		char(15),
	@Texte5PMV_In		char(15),
	@Alternat1PMV_In	char(15),
	@Alternat2PMV_In	char(15),
	@Alternat3PMV_In	char(15),
	@Alternat4PMV_In	char(15),
	@Alternat5PMV_In	char(15),
	@Flash_In			tinyint,
	@Dest_un_proche_In 	tinyint,
	@Dest_un_loin_In 	tinyint,
	@Dest_deux_proche_In tinyint,
	@Dest_deux_loin_In 	tinyint,
	@Libelle_In			char(25),
	@Realisable_In		bit

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 			NumPA_In
* XDY_Mot 			Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 			NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_LignePMV		Texte1PMV_In
* XDY_LignePMV		Texte2PMV_In
* XDY_LignePMV		Texte3PMV_In
* XDY_LignePMV		Texte4PMV_In
* XDY_LignePMV		Texte5PMV_In
* XDY_LignePMV		Texte6PMV_In
* XDY_Nom			Libelle_In
* XDY_TxtPicto		TextePicto_In
* XDY_NomPicto		NomPicto_In
* XDY_Mot			Dest_un_proche_In
* XDY_Mot			Dest_un_loin_In
* XDY_Mot			Dest_deux_proche_In
* XDY_Mot			Dest_deux_loin_In
* 					Libelle_In
* XDY_Booleen		Realisable_In
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
* FONCTION :
*
* Ajoute dans la table PA_PMVA et retourne le numero du PMVA
* cree
*
* MECANISMES :
*
------------------------------------------------------*/

as 
declare
   	@vl_Status int,
	@vl_num int,
	@vl_cle tinyint,
	@vl_Site tinyint

#ifdef HIST
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP28
			@NumPA_In			,
			@Ordre_In			,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In			,
			@Explication_In		,
			@Texte1PMV_In		,
			@Texte2PMV_In		,
			@Texte3PMV_In		,
			@Texte4PMV_In		,
			@Texte5PMV_In		,
			@Alternat1PMV_In		,
			@Alternat2PMV_In		,
			@Alternat3PMV_In		,
			@Alternat4PMV_In		,
			@Alternat5PMV_In		,
			@Flash_In			,
			@Dest_un_proche_In	,
			@Dest_un_loin_In	,
			@Dest_deux_proche_In	,
			@Dest_deux_loin_In		,
			@Libelle_In			,
			@Realisable_In

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP28
			@NumPA_In			,
			@Ordre_In			,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In			,
			@Explication_In		,
			@Texte1PMV_In		,
			@Texte2PMV_In		,
			@Texte3PMV_In		,
			@Texte4PMV_In		,
			@Texte5PMV_In		,
			@Alternat1PMV_In		,
			@Alternat2PMV_In		,
			@Alternat3PMV_In		,
			@Alternat4PMV_In		,
			@Alternat5PMV_In		,
			@Flash_In			,
			@Dest_un_proche_In	,
			@Dest_un_loin_In	,
			@Dest_deux_proche_In	,
			@Dest_deux_loin_In		,
			@Libelle_In			,
			@Realisable_In

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else

   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if      @NumPA_In = null 
   	or @Ordre_In = null 
   	or @District_In = null
	or @PrioriteEvt_In = null 
	or @NoEqt_In = null
	or @Realisable_In = null
      
      return XDC_NOK
   else   
      begin
     
      /*A
      ** Ajout dans la table SAD_PMVA (PA_PMVA) d'un nouvelle proposition d'affichage PMVA
      ** a un plan d'actions
      */
      
      if @Remarque_In = null
         select @Remarque_In = ' '
         
      if @Explication_In = null
         select @Explication_In = ' '
         
      if @Texte1PMV_In = null
         select @Texte1PMV_In = ' '   
         
      if @Texte2PMV_In = null
         select @Texte2PMV_In = ' '   
         
      if @Texte3PMV_In = null
         select @Texte3PMV_In = ' '   
         
      if @Texte4PMV_In = null
         select @Texte4PMV_In = ' '   
         
      if @Texte5PMV_In = null
         select @Texte5PMV_In = ' '   
         
      if @Alternat1PMV_In = null
         select @Alternat1PMV_In = ' '   
      if @Alternat2PMV_In = null
         select @Alternat2PMV_In = ' '   
               if @Alternat3PMV_In = null
         select @Alternat3PMV_In = ' '   
               if @Alternat4PMV_In = null
         select @Alternat4PMV_In = ' '   
               if @Alternat5PMV_In = null
         select @Alternat5PMV_In = ' '            
      if @Libelle_In = null
         select @Libelle_In = ' '   
         
      insert into EXP..SAD_PMA (numero, ordre, district, equipement, priorite, remarque, explication, 
				premiere_ligne, deuxieme_ligne, troisieme_ligne, quatrieme_ligne, cinquieme_ligne,
				premier_alternat, deuxieme_alternat, troisieme_alternat, quatrieme_alternat, cinquieme_alternat,
				flash, dest_un_proche, dest_un_lointaine, dest_deux_proche, dest_deux_lointaine,
				realisable, libelle)
      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, 
				@Texte1PMV_In, @Texte2PMV_In, @Texte3PMV_In, @Texte4PMV_In, @Texte5PMV_In, 
				@Alternat1PMV_In, @Alternat2PMV_In,@Alternat3PMV_In,@Alternat4PMV_In,@Alternat5PMV_In,
				@Flash_In, @Dest_un_proche_In, @Dest_un_loin_In, @Dest_deux_proche_In, @Dest_deux_loin_In, 
				@Realisable_In, @Libelle_In)
   

      if @Libelle_In != "EXTINCTION" and @Libelle_In != "SAPIENS"
      begin
         select @vl_num = evenement,
         @vl_cle = cle
         from EXP..SAD_PLN
         where numero = @NumPA_In

         select 1 from EXP..FMC_GEN
         where numero = @vl_num  and  cle = @vl_cle
         and origine_creation not like '%ESSAI%'
         and origine_creation not like '%EXERCICE%'
         and origine_creation not like '%SUPPRIMER%'

         if @@rowcount = 1
         begin
            delete EXP..EQT_FIL
            where evenement = @vl_num and cle = @vl_cle
            and type_equipement = XDC_EQT_PMVA
            and equipement = @NoEqt_In
            and sit = @District_In

            insert into EXP..EQT_FIL
            values (@vl_num, @vl_cle, XDC_EQT_PMVA, @NoEqt_In, @PrioriteEvt_In, 0, @District_In)
         end
      end

      return XDC_OK

   end
#endif
go
