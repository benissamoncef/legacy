/*E*/
/*Fichier : $Id: xzan16sp.prc,v 1.7 2020/11/09 14:26:11 devgtie Exp $      Release : $Revision: 1.7 $        Date : $Date: 2020/11/09 14:26:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* guilhou	25 nov 1996	: retourner le numero de zone en sortie (DEM/1306) 1.2
* PNI		15/09/2014	: patch pour ajout de colonne sens 1.3
* JPL/AD	07/10/2014	: Ecriture du sens (DEM 1100) 1.4
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan16sp.h" */

/* static char *version = "@(#)xzan16.c	1.1 23/18/94 : xzan16" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une zone geographique a une competence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;16
   @va_NumCompetence_in	smallint = null,
   @va_Autoroute_in	tinyint = null,
   @va_PRdebut_in	int = null,
   @va_PRfin_in		int = null,
   @va_sens_in		tinyint = null,
   @va_NumZone_out	smallint = null output
   as 

/* 
*
* SP	XZAN16SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PRdebut_in
* XDY_PR		va_PRfin_in
* XDY_Octet		va_sens_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot 		va_NumZoneGeo_out
*
* VALEUR RENDUE :
*
* Ajoute une zone geographique a une competence
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumFicheAnnuaire inconnu
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
* Renseigner Autoroute
* 
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Compt		smallint,
   @vl_Auto		tinyint,
   @vl_Numero		int,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumCompetence_in 	= null
   or @va_Autoroute_in 		= null	
   or @va_PRdebut_in		= null
   or @va_PRfin_in		= null
   or @va_sens_in		= null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
         
         select @vl_Compt = numero from CFG..ANN_AST_CMP
           where numero = @va_NumCompetence_in
           
         select @vl_Auto = numero from CFG..RES_AUT
           where numero = @va_Autoroute_in
           
           
         /*A
         ** La competence et l'autoroute existent
         ** Ajout d'une zone geographique
         */
         
         if @vl_Compt != null and @vl_Auto != null
         begin
            
            select @vl_Numero = max ( numero ) from CFG..ANN_GEO
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFG..ANN_GEO
               ( numero, competence, autoroute, PR_debut, PR_fin, sens )
            values ( @vl_Numero + 1, @va_NumCompetence_in, @va_Autoroute_in, @va_PRdebut_in, @va_PRfin_in , @va_sens_in)
         

		select @va_NumZone_out = @vl_Numero + 1
         end
         
         else
         
         begin
         
         /*A
         ** La competence ou l'autoroute n'existe pas
         ** return XDC_NOK
         */
         
            return ( XDC_NOK ) 
         
         end
      end
      
      else
      
      /*A
      ** Le serveur n'est pas le CI
      ** return XDC_NOK
      */
         
         return ( XDC_NOK )
      
   end 
     
   return ( XDC_OK )
         
go  
