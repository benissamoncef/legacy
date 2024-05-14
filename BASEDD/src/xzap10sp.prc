/*E*/
/*Fichier : $Id: xzap10sp.prc,v 1.6 1998/09/22 13:37:21 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/09/22 13:37:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap10sp.prc
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
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.6
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute un morceau de texte (250 car.) d'un FAX a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;10

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@Texte_In		char(250)
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_Texte		Texte_In
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
* Ajoute dans la table TEXTE_PA_FAX
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap10sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap10sp.prc	1.1 08/25/94 : xzap10sp" ; */

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
		exec @vl_Status = SQL_CI.PRC..XZAP;10
		@NumPA_In		,
		@Ordre_In		,
		@Texte_In		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;10
		@NumPA_In		,
		@Ordre_In		,
		@Texte_In		


	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A
   ** Declaration des variables locales a la procedure
   */
   
   declare @Morceau smallint
   
   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if @NumPA_In = null or @Ordre_In = null
      
      return XDC_NOK
   else   
   begin
   
      /*A
      ** Ajout dans la table SAD_TXT_FAX (TEXTE_PA_FAX) d'une proposition d'envoi
      ** Fax a un plan d'actions
      */

      
      if @Texte_In = null
         select @Texte_In = ' '
      
      
      select @Morceau = max(morceau) from EXP..SAD_TXT_FAX 
         where numero = @NumPA_In 
           and ordre = @Ordre_In
      
      if ( @Morceau = NULL)
         select @Morceau = 0
      
      insert into EXP..SAD_TXT_FAX

      values ( @NumPA_In, @Ordre_In, @Morceau + 1, @Texte_In )
      
      return XDC_OK
      
   end
#endif
go
