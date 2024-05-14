/*E*/
/*Fichier : Fichier : 	@(#)xzap83sp.prc	1.1 Release : 1.1 Date : 03/26/07
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap83sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18	creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP83' and type = 'P')
	drop procedure XZAP83
go	


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'affichage CFE a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP83

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In		smallint,
	@Explication_In		char(250),
	@PlanDeFeux_In		tinyint,
	@Libelle_In		char(25),
	@Realisable_In		bit

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 		NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_Octet		PlanDeFeux_in
* XDY_Nom		Libelle_In
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
* Ajoute dans la table PA_CFE et retourne le numero du CFE
* cree
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap02sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap02sp.prc	1.1 08/25/94 : xzap02sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
declare
   @vl_Status int, @vl_Site tinyint
   
#ifdef HIST
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP83
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@PrioriteEvt_In		,
		@NoEqt_In		,
		@Explication_In		,
		@PlanDeFeux_in		,
		@Libelle_In			,
		@Realisable_In		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP83
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@PrioriteEvt_In		,
		@NoEqt_In		,
		@Explication_In		,
		@PlanDeFeux_in		,
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
         
      if @PlanDeFeux_In = null
         select @PlanDeFeux_In = 0   
         
      if @Libelle_In = null
         select @Libelle_In = ' '   
         
      insert into EXP..SAD_CFE (numero, ordre, district, equipement, priorite, remarque, explication, 
				plan_de_feux,
				realisable, libelle)
      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, 
				@PlanDeFeux_In,
				@Realisable_In, @Libelle_In)
   
      return XDC_OK
      
      end
#endif
go
