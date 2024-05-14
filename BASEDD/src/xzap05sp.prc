/*E*/
/*Fichier : $Id: xzap05sp.prc,v 1.7 1998/09/22 13:37:15 gaborit Exp $      Release : $Revision: 1.7 $        Date : $Date: 1998/09/22 13:37:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Sep 1994	: Creation
* gaborit	21 Sep 1994	: Rien (v1.2)
* gaborit	21 Sep 1994	: Rien (v1.3)
* volcic	13 Oct 1994	: Modification (v1.4)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.5)
* volcic	25 Nov 1994	: Modification XDY_Scenario (v1.6)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.7
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition de signalisation echangeur a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;05

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In		smallint,
	@Explication_In		char(250),
	@Libelle_In		char(25),
	@Scenario_In		tinyint,
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
* XDY_Nom		Libelle_In
* XDY_Octet		Scenario_In
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
* Ajoute dans la table PA_ECHANGEUR
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap05sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap05sp.prc	1.1 08/25/94 : xzap05sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;05
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
		exec @vl_Status = SQL_CA.PRC..XZAP;05
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
      ** Ajout dans la table SAD_ECH (PA_ECHANGEUR) d'une proposition de
      ** positionnement NAV a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Explication_In = null
         select @Explication_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '
      
      
      insert into EXP..SAD_ECH

      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, @Scenario_In, @Realisable_In, @Libelle_In )
   
      return XDC_OK
      
      end
#endif
go
