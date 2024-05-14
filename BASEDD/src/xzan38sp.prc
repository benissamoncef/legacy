/*E*/
/*Fichier : $Id: xzan38sp.prc,v 1.3 2020/11/09 14:15:04 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/09 14:15:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan38sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	14 nov 1996	: refonte complete de la gestion des astreintes (DEM/1306)
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan38sp.h" */

/* static char *version = "$Id: xzan38sp.prc,v 1.3 2020/11/09 14:15:04 devgtie Exp $ $Revision: 1.3 $ $Date: 2020/11/09 14:15:04 $: xzan38" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
* regarde si une competence existe deja en base 
* pour type, sous-type 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;38
   @va_NumAstreinte_in		tinyint = null,
   @va_SousType_in		char(25) = null,
   @va_NumComp_in		smallint = null
   as 

/* 
*
* SP	XZAN38SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumAstreinte_in
* XDY_Octet		va_SousTypeAstreinte
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* CODE RETOUR :
* 1	XDC_NOK 
* XDC_VRAI si une competence identique existe
* XDC_FAUX sinon
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare
	@vl_Site tinyint
	
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumAstreinte_in = null
   or @va_SousType_in = null
      return ( XDC_ARG_INV )
   else   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
      
         /*A
         ** Verification de l'existence du type et du sous-type
         */
         select * from CFG..ANN_AST_CMP
	 where type_d_astreinte = @va_NumAstreinte_in
	 and sous_type_d_astreinte = @va_SousType_in
	 and numero != @va_NumComp_in
	 and (inhibe != XDC_VRAI or inhibe = null)

	 /*si une competence identique existe deja*/
         if @@rowcount<>0 
		return (XDC_ACT_REFUSEE)
	 else
		return (XDC_OK)
      end
      else
      /*A
      ** Le serveur n'est pas le CI
      */
         return ( XDC_NOK )
   end 
go  
