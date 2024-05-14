/*E*/
/*Fichier : $Id: xzap35sp.prc,v 1.1 2021/06/03 07:10:47 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2021/06/03 07:10:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap35sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	11/05/2021	Création DEM-SAE244 1.1
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC								
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition SONO a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP35' and type = 'P')
	drop procedure XZAP35
go


create procedure XZAP35

	@va_NumPA_in		smallint,
	@va_Ordre_in		smallint,
	@va_District_in		tinyint,
	@va_NoEqt_in		smallint,
	@va_PrioriteEvt_in	tinyint,
	@va_Remarque_in		char(50),
	@va_Explication_in	char(250),
	@va_Realisable_in	bit,
	@va_Message_in		char(31),
	@va_Autoroute_in	tinyint,
	@va_Sens_in		tinyint,
	@va_PR_Debut_in		int,
	@va_PR_Fin_in		int,
	@va_Libelle_in		char(25)
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_in
* XDY_Mot 		Ordre_in
* XDY_District		District_in
* XDY_Eqt 		NoEqt_in
* XDY_Octet		PrioriteEvt_in
* XDY_Commentaire	Remarque_in
* XDY_Text		Explication_in
* XDY_Booleen		Realisable_in
* XDY_Text		Message_in
* XDY_autoroute		Autoroute_in
* XDY_sens		Sens_in
* XDY_PR		PR_deb_in
* XDY_PR		PR_fin_in
* XDY_Nom		Libelle_in
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
* Ajoute dans la table SAD_SONO
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
		exec @vl_Status = SQL_CI.PRC..XZAP35
			@va_NumPA_in,
			@va_Ordre_in,
			@va_District_in,
			@va_NoEqt_in,
			@va_PrioriteEvt_in,
			@va_Remarque_in,
			@va_Explication_in,
			@va_Realisable_in,
			@va_Message_in,
			@va_Autoroute_in,
			@va_Sens_in,
			@va_PR_Debut_in,
			@va_PR_Fin_in,
			@va_Libelle_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP35
			@va_NumPA_in,
			@va_Ordre_in,
			@va_District_in,
			@va_NoEqt_in,
			@va_PrioriteEvt_in,
			@va_Remarque_in,
			@va_Explication_in,
			@va_Realisable_in,
			@va_Message_in,
			@va_Autoroute_in,
			@va_Sens_in,
			@va_PR_Debut_in,
			@va_PR_Fin_in,
			@va_Libelle_in

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if @va_NumPA_in = null 
   	or @va_Ordre_in = null 
   	or @va_District_in = null
	or @va_PrioriteEvt_in = null 
	or @va_NoEqt_in = null
	or @va_Realisable_in = null
	or @va_Autoroute_in = null
	or @va_Sens_in = null
	or @va_PR_Debut_in = null
	or @va_PR_Fin_in = null
      
      return XDC_NOK
   else   
      begin
   
      /*A
      ** Ajout dans la table SAD_SONO  d'une proposition de
      ** positionnement SONO a un plan d'actions
      */
   
      if @va_Remarque_in = null
         select @va_Remarque_in = ' '
      
      if @va_Explication_in = null
         select @va_Explication_in = ' '
      												
      if @va_Libelle_in = null
         select @va_Libelle_in = ' '
      
      
      
      insert into EXP..SAD_SONO

      values ( 	@va_NumPA_in,
		@va_Ordre_in,
		@va_District_in,
		@va_NoEqt_in,
		@va_PrioriteEvt_in,
		@va_Remarque_in,
		@va_Explication_in,
		@va_Realisable_in,
		@va_Message_in,
		@va_Autoroute_in,
		@va_Sens_in,
		@va_PR_Debut_in,
		@va_PR_Fin_in,
		@va_Libelle_in )
   
      return XDC_OK
      
      end
#endif
go
