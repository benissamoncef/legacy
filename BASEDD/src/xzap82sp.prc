/*E*/
/* Fichier : $Id: xzap82sp.prc,v 1.1 2012/07/04 11:37:09 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:37:09 $
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
* VR	22/12/11 	: Creation (DEM/1016)
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAP82' and type = 'P')
	drop procedure XZAP82
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'affichage PRV a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP82

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In		smallint,
	@Explication_In		char(250),
	@Bandeau_In		char(6),
	@Vitesse_In		char(6),
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
* XDY_LignePMV		Bandeau_In
* XDY_Mot		Vitesse_In
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
		exec @vl_Status = SQL_CI.PRC..XZAP82
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In		,
			@Explication_In		,
			@Bandeau_In		,
			@Vitesse_In,
			@Libelle_In		,
			@Realisable_In		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP82
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Remarque_In		,
			@PrioriteEvt_In		,
			@NoEqt_In		,
			@Explication_In		,
			@Bandeau_In		,
			@Vitesse_In,
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
      ** Ajout dans la table SAD_PRV (PA_PRV) d'un nouvelle proposition d'affichage PRV
      ** a un plan d'actions
      */
      
      if @Remarque_In = null
         select @Remarque_In = ' '
         
      if @Explication_In = null
         select @Explication_In = ' '
         
      if @Bandeau_In = null
         select @Bandeau_In = ' '   
            
     
      if @Vitesse_In = null
         select @Vitesse_In = ' '   

      if @Libelle_In = null
         select @Libelle_In = ' '   
         
    
         
      
      insert into EXP..SAD_PRV

      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, @Bandeau_In, @Vitesse_In, @Realisable_In, @Libelle_In)
   
      return XDC_OK
      
      end
#endif
go
