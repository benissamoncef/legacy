/*E*/
/*Fichier : $Id: xzap09sp.prc,v 1.14 2018/10/19 17:41:29 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2018/10/19 17:41:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap09sp.prc
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
* gaborit	05 dec 1994	: ajout explication (v1.7)
* gaborit       02 mai 1995	: explication=" " si nulle (1.8)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.9
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.6
* PNI		04 Jul 2013 	: verif l'existance d'une competence geographique: evite les proposition sans interlocuteur ou destinataire v1.10 FE114
* JPL		08/10/2014	: Nommage des colonnes a l'insertion d'une proposition (pour DEM 1100)  1.11
* JPL		09/10/2014	: Degroupee; prise en compte du sens des competences (DEM 1100)  1.12
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.13
* JMG		02/10/18	: ajout SMS LOT25 DEM1301 1.14
------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAP09' and type = 'P')
	drop procedure XZAP09
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute un destinataire pour un Fax
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP09

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@PR_In			T_PR,
	@NumeroAutoroute_In	tinyint,
	@Sens_In		tinyint,
	@TypeDAstreinte_In 	tinyint,
	@Explication_In 	char(250),
	@Sms_In	tinyint=null
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_PR		PR_In
* XDY_Autoroute		NumeroAutoroute_In
* XDY_Sens		Sens_In
* XDY_Octet		TypeDAstreinte_in
* XDY_Commentaire       Explication_in
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
* Ajoute dans la table DEST_PA_FAX
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
		exec @vl_Status = SQL_CI.PRC..XZAP09
			@NumPA_In		,
			@Ordre_In		,
			@PR_In			,
			@NumeroAutoroute_In	,
			@Sens_In		,
			@TypeDAstreinte_In 	,
			@Explication_In 	,
			@Sms_In

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP09
			@NumPA_In		,
			@Ordre_In		,
			@PR_In			,
			@NumeroAutoroute_In	,
			@Sens_In		,
			@TypeDAstreinte_In 	,
			@Explication_In 	,
			@Sms_In

	if @vl_Status = null
		return XDC_PRC_INC
	else 
		return @vl_Status

#else

   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if      @NumPA_In = null or @Ordre_In = null or @PR_In = null
   	or @NumeroAutoroute_In	= null or @Sens_In = null or @TypeDAstreinte_In  = null
      return XDC_NOK
   else   
      begin
        declare @vl_Sens tinyint

        select @vl_Sens = @Sens_In
        if ( @Sens_In = XDC_SENS_ENTREE_NORD )  or  (@Sens_In = XDC_SENS_SORTIE_NORD )
          select @vl_Sens = XDC_SENS_NORD
        if ( @Sens_In = XDC_SENS_ENTREE_SUD )  or  (@Sens_In = XDC_SENS_SORTIE_SUD )
          select @vl_Sens = XDC_SENS_SUD

      select @Explication_In=isnull (@Explication_In," ") 

      /*A
      ** Ajout dans la table SAD_DST_FAX (DEST_PA_FAX) d'un destiantaire pour un Fax
      */
         
      /* FE 114, n'ins�re la proposition que s'il existe une comp�tence geographique: evite les proposition sans interlocuteur ou destinataire */
	select C.numero
	from CFG..ANN_GEO C, CFG..ANN_AST_CMP A 
	where type_d_astreinte=@TypeDAstreinte_In and
	      A.numero=competence and
	      autoroute=@NumeroAutoroute_In and
	      (case when PR_debut <= PR_fin then PR_debut else PR_fin end) <= @PR_In and
	      (case when PR_debut <= PR_fin then PR_fin else PR_debut end) >= @PR_In and
	      ( sens = XDC_LES_2_SENS  or  sens = @vl_Sens )

      if @@rowcount > 0
      begin

      insert into EXP..SAD_DST_FAX
        ( numero, ordre, autoroute, PR, sens, type_d_astreinte, explication , sms)

        values ( @NumPA_In, @Ordre_In, @NumeroAutoroute_In, @PR_In, @vl_Sens, @TypeDAstreinte_In, @Explication_In , @Sms_In)
      end

      return XDC_OK
      
      end
#endif
go
