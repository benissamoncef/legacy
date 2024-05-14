/* Fichier : $Id: xzap821sp.prc
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap821sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	04/12/23	: Creation (DEM483)
------------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAP821' and type = 'P')
	drop procedure XZAP821
go

/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'affichage PIC a un plan d'actions
*  
* ------------------------------------------------------
* SEQUENCE D'APPEL :
*/
create procedure XZAP821

	@NumPA_In			smallint,
	@Ordre_In			smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In			smallint,
	@Explication_In		char(250),
	@Bandeau_In 		char(XDC_LGR_Bandeau_PIC),
	@Flash_In			bit,
	@Picto_In			char(XDC_LGR_Picto_PIC),
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
* XDY_Bandeau_PIC	Bandeau_In
* XDY_Booleen		Flash_In
* XDY_Picto_PIC		Picto_In
* XDY_Nom			Libelle_In
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
* Ajoute dans la table PA_PMV et retourne le numero du PMV
* cree
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */
/* definitions de constantes */
/* definitions de types locaux */
/* definition de macro locales */
/* declaration de variables locales */
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
		exec @vl_Status = SQL_CI.PRC..XZAP821
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In		,
			@Explication_In		,
			@Bandeau_In,
			@Flash_In,
			@Picto_In,
			@Libelle_In		,
			@Realisable_In		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP821
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In		,
			@Explication_In		,
			@Bandeau_In,
			@Flash_In,
			@Picto_In,
			@Libelle_In		,
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
      ** Ajout dans la table SAD_PIC (PA_PIC) d'un nouvelle proposition d'affichage PIC
      ** a un plan d'actions
      */
      
      if @Remarque_In = null
         select @Remarque_In = ' '
         
      if @Explication_In = null
         select @Explication_In = ' '
         
      if @Bandeau_In = null
         select @Bandeau_In = ' '   
            
     
      if @Picto_In = null
         select @Picto_In = ' '   

      if @Libelle_In = null
         select @Libelle_In = ' '   
         
      
      insert into EXP..SAD_PIC
	  	values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, @Picto_In, @Bandeau_In, @Flash_In, @Realisable_In, @Libelle_In)

      return XDC_OK
      
      end
#endif
go
