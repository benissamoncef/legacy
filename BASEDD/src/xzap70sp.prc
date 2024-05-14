/*E*/
/*Fichier : @(#)xzap02sp.prc	1.10      Release : 1.10        Date : 09/22/98
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
* JMG   20/03/17        creation DEM1191
* JMG	20/06/17	convergence 1.2
* AMA 	26/11/20	suppresion de clignotement  lors  du insert
* GGY	04/05/23	: Augmentation taille chalp TextePicto_In et NomPicto_In (DEM-473)
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/


use PRC
go

drop procedure XZAP70
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'affichage PMV a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP70

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In		smallint,
	@Explication_In		char(250),
	@Texte1PMV_In		char(18),
	@Texte2PMV_In		char(18),
	@Texte3PMV_In		char(18),
	@Texte4PMV_In		char(18),
	@Texte5PMV_In		char(18),
	@Libelle_In		char(25),
	@TextePicto_In		char(XDC_LGR_TXT_PICTO),
	@NomPicto_In		char(XDC_LGR_NOM_PICTO),
	@Barriere_In		tinyint,
	@Scenario_In		smallint,
	@Destination_In		smallint,
	@DestinationAlternat_In	smallint,
	@DestinationNoeud_In smallint ,
	@Realisable_In		bit,
	@Clignotement_In		int

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
* XDY_LignePMV		Texte1PMV_In
* XDY_LignePMV		Texte2PMV_In
* XDY_LignePMV		Texte3PMV_In
* XDY_LignePMV		Texte4PMV_In
* XDY_LignePMV		Texte5PMV_In
* XDY_LignePMV		Texte6PMV_In
* XDY_Nom		Libelle_In
* XDY_TxtPicto		TextePicto_In
* XDY_NomPicto		NomPicto_In
* XDY_Mot		@Destination_In
* XDY_Mot		@DestinationAlternat_In
* XDY_Mot		@DestinationNoeud_In
* XDY_Booleen		Realisable_Inmake_prc xzap70sp.prc CI

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
* Ajoute dans la table PA_PMV et retourne le numero du PMV
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
		exec @vl_Status = SQL_CI.PRC..XZAP70
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@PrioriteEvt_In		,
		@NoEqt_In		,
		@Explication_In		,
		@Texte1PMV_In		,
		@Texte2PMV_In		,
		@Texte3PMV_In		,
		@Texte4PMV_In		,
		@Texte5PMV_In		,
		@Libelle_In		,
		@TextePicto_In		,
		@NomPicto_In		,
		@Barriere_In		,
		@Scenario_In		,
		@Destination_In		,
		@DestinationAlternat_In	,
		@DestinationNoeud_In,
		@Realisable_In,
		@Clignotement_In

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP70
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@PrioriteEvt_In		,
		@NoEqt_In		,
		@Explication_In		,
		@Texte1PMV_In		,
		@Texte2PMV_In		,
		@Texte3PMV_In		,
		@Texte4PMV_In		,
		@Texte5PMV_In		,
		@Libelle_In		,
		@TextePicto_In		,
		@NomPicto_In		,
		@Barriere_In		,
		@Scenario_In		,
		@Destination_In		,
		@DestinationAlternat_In	,
		@DestinationNoeud_In,
		@Realisable_In,
		@Clignotement_In

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
      ** Ajout dans la table SAD_SAGA (PA_PMV) d'un nouvelle proposition d'affichage SAGA
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
         
      if @Libelle_In = null
         select @Libelle_In = ' '   
         
      if @TextePicto_In = null
         select @TextePicto_In = ' '   
         
      
      insert into EXP..SAD_SAGA

      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, @Texte1PMV_In, @Texte2PMV_In, @Texte3PMV_In, @Texte4PMV_In, @Texte5PMV_In, null,null,null,null,null, null, @NomPicto_In, null,@TextePicto_In, @Barriere_In, @Scenario_In, @Realisable_In, @Libelle_In /* @Destination_In, @DestinationAlternat_In, @DestinationNoeud_In*/)
   
      return XDC_OK
      
      end
#endif
go
