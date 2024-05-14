/*E*/
/*X  Fichier : $Id: xzap33sp.prc,v 1.2 2014/10/08 12:42:31 gesconf Exp $    Release : $Revision: 1.2 $       Date : $Date: 2014/10/08 12:42:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap33sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	30/08/05	: Creation
* JPL	08/10/14	: Suppression procedure avant compilation  1.2
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition BAD a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP33' and type = 'P')
	drop procedure XZAP33
go


create procedure XZAP33

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(250),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In		smallint,
	@Explication_In		char(250),
	@Libelle_In		char(25),
	@Scenario_In		char(10),
	@Realisable_In		bit
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Commentaire	Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 		NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_Nom		Libelle_In
* XDY_Scenario		Scenario_In
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
* Ajoute dans la table PA_NAV 
* cree
*
* MECANISMES :
*
------------------------------------------------------*/


as 

#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP33
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In		,
			@Explication_In		,
			@Libelle_In		,
			@Scenario_In		,
			@Realisable_In		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP33
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In		,
			@Explication_In		,
			@Libelle_In		,
			@Scenario_In		,
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
      ** Ajout dans la table SAD_NAV (PA_NAV) d'une proposition de
      ** positionnement NAV a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Explication_In = null
         select @Explication_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '
      
      if @Scenario_In = null
         select @Scenario_In = ' '
      
      
      insert into EXP..SAD_BAD

      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, @Scenario_In, @Realisable_In, @Libelle_In )
   
      return XDC_OK
      
      end
#endif
go
