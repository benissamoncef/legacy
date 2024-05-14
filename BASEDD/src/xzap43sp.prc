/*E*/
/*Fichier : $Id: xzap43sp.prc,v 1.1 2007/12/13 10:52:17 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/12/13 10:52:17 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	17/01/07	creation
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'envoi MAIL a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP43

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@Explication_In		char(250),
	@Libelle_In		char(25)
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Commentaire	Explication_In
* XDY_Nom		Libelle_In
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
* Ajoute dans la table PA_MAIL
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap43sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap07sp.prc	1.1 08/25/94 : xzap07sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
#ifdef HIST
declare @vl_Status int, 
		@vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP43
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@Explication_In		,
		@Libelle_In

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP43
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@Explication_In		,
		@Libelle_In

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
      
      return XDC_NOK
   else   
      begin
   
      /*A
      ** Ajout dans la table SAD_MAIL (PA_MAIL) d'une proposition d'envoi
      ** Fax a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Explication_In = null
         select @Explication_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '
      
      
      insert into EXP..SAD_MAIL

      values ( @NumPA_In, @Ordre_In, @District_In, @Remarque_In, @Explication_In, @Libelle_In )
   
      return XDC_OK
      
      end
#endif
go
