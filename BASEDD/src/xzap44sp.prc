/*E*/
/*Fichier : $Id: xzap44sp.prc,v 1.2 2014/10/08 16:19:56 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2014/10/08 16:19:56 $
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
* JMG	17/10/07	: creation
* JPL	08/10/2014	: Nommage des colonnes a l'insertion d'une proposition (pour DEM 1100)  1.2
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute un destinataire pour un Mail
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP44

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@PR_In			T_PR,
	@NumeroAutoroute_In	tinyint,
	@TypeDAstreinte_In 	tinyint,
	@Explication_In 	char(250)
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_PR		PR_In
* XDY_Autoroute		NumeroAutoroute_In
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
* Ajoute dans la table DEST_PA_MAIL
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
		exec @vl_Status = SQL_CI.PRC..XZAP44
		@NumPA_In		,
		@Ordre_In		,
		@PR_In			,
		@NumeroAutoroute_In	,
		@TypeDAstreinte_In 	,
		@Explication_In

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP44
		@NumPA_In		,
		@Ordre_In		,
		@PR_In			,
		@NumeroAutoroute_In	,
		@TypeDAstreinte_In 	,
		@Explication_In

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
   	or @NumeroAutoroute_In	= null or @TypeDAstreinte_In  = null
      return XDC_NOK
   else   
      begin
      select @Explication_In=isnull (@Explication_In," ") 
      /*A
      ** Ajout dans la table SAD_DST_MAIL (DEST_PA_MAIL) d'un destiantaire pour un Mail
      */
   
      insert into EXP..SAD_DST_MAIL
        ( numero, ordre, autoroute, PR, type_d_astreinte, explication )

        values ( @NumPA_In, @Ordre_In, @NumeroAutoroute_In, @PR_In, @TypeDAstreinte_In, @Explication_In )
   
      return XDC_OK
      
      end
#endif
go
