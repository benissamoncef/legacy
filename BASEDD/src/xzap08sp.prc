/*E*/
/*Fichier : $Id: xzap08sp.prc,v 1.11 2016/09/21 10:39:09 devgfi Exp $      Release : $Revision: 1.11 $        Date : $Date: 2016/09/21 10:39:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap08sp.prc
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
* volcic	24 Oct 1994	: Ajout _In @TypeDAstreinte (v1.5)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.6)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.7
* PNI		04 Jul 2013	: verif l'existance d'une competence geographique: evite les proposition sans interlocuteur ou destinataire v1.8 FE114
* JPL		08/10/2014	: Nommage des colonnes a l'insertion d'une proposition (pour DEM 1100)  1.9
* JPL		09/10/2014	: Degroupee; prise en compte du sens des competences (DEM 1100)  1.10
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.11
* JPL		03/08/2021	: Selection competence si autoroute et PRs concordent pour evt. dans les 2 sens (SAE-314)  1.12
* JPL		01/07/2021	: Extension du champ "Remarque" a 250 caracteres (SAE-287)  1.13
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAP08' and type = 'P')
	drop procedure XZAP08
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'appel telephonique ou bip a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP08

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(250),
	@Explication_In		char(250),
	@Libelle_In		char(25),
	@PR_In			T_PR,
	@NumeroAutoroute_In	tinyint,
	@Sens_In		tinyint,
	@TypeDAstreinte_In 	tinyint
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Commentaire	Explication_In
* XDY_Nom		Libelle_In
* XDY_PR		PR_In
* XDY_Autoroute		NumeroAutoroute_In
* XDY_Sens		Sens_In
* XDY_Octet		TypeDAstreinte_in
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
* Ajoute dans la table PA_APPEL
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap08sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap08sp.prc	1.1 08/25/94 : xzap08sp" ; */

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
		exec @vl_Status = SQL_CI.PRC..XZAP08
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@Explication_In		,
		@Libelle_In		,
		@PR_In			,
		@NumeroAutoroute_In	,
		@Sens_In		,
		@TypeDAstreinte_In 	

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP08
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@Explication_In		,
		@Libelle_In		,
		@PR_In			,
		@NumeroAutoroute_In	,
		@Sens_In		,
		@TypeDAstreinte_In 	

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
	or @PR_In = null 
	or @NumeroAutoroute_In	= null 
	or @Sens_In = null
	or @TypeDAstreinte_In  = null
      return XDC_NOK
   else   
      begin
        declare @vl_Sens tinyint

        select @vl_Sens = @Sens_In
        if ( @Sens_In = XDC_SENS_ENTREE_NORD )  or  (@Sens_In = XDC_SENS_SORTIE_NORD )
          select @vl_Sens = XDC_SENS_NORD
        if ( @Sens_In = XDC_SENS_ENTREE_SUD )  or  (@Sens_In = XDC_SENS_SORTIE_SUD )
          select @vl_Sens = XDC_SENS_SUD

      /*A
      ** Ajout dans la table SAD_APL (PA_APPEL) d'une proposition d'envoi
      ** Fax a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Explication_In = null
         select @Explication_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '
      
      /* FE 114, n'ins�re la proposition que s'il existe une comp�tence geographique: evite les proposition sans interlocuteur ou destinataire */
	select C.numero
	from CFG..ANN_GEO C, CFG..ANN_AST_CMP A 
	where type_d_astreinte=@TypeDAstreinte_In and
	      A.numero=competence and
	      autoroute=@NumeroAutoroute_In and
	      (case when PR_debut <= PR_fin then PR_debut else PR_fin end) <= @PR_In and
	      (case when PR_debut <= PR_fin then PR_fin else PR_debut end) >= @PR_In and
	      ( sens = XDC_LES_2_SENS  or  @vl_Sens = XDC_LES_2_SENS  or  sens = @vl_Sens )

      if @@rowcount > 0
      begin
      	insert into EXP..SAD_APL
          ( numero, ordre, district, remarque, explication, autoroute, PR, sens, type_d_astreinte, libelle )

          values ( @NumPA_In,
                   @Ordre_In,
                   @District_In,
                   @Remarque_In,
                   @Explication_In,
                   @NumeroAutoroute_In,
                   @PR_In,
                   @vl_Sens,
                   @TypeDAstreinte_In,
                   @Libelle_In )
      end
      return XDC_OK
      
      end
#endif
go
