/*E*/
/*Fichier : $Id: xzan43sp.prc,v 1.3 1996/12/11 09:23:01 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/12/11 09:23:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan43sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	19 nov 1996	: Creation (DEM/1306)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan43sp.h" */

/* static char *version = "@(#)xzan32.c	1.1 23/18/94 : xzan32" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
* libelles de periodes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;43
   @va_Horodebut_in		datetime = null,
   @va_Horofin_in		datetime = null,
   @va_Delta_in			tinyint = null,
   @va_Debut_out		datetime = null output,
   @va_Fin_out			datetime = null output
   as 

/* 
*
* SP	XZAN43SP
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeFiche
*
* VALEUR RENDUE :
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
   /*A
   ** Declaration des variables locales
   */
   declare @vl_jour_semaine int,@vl_date datetime, @vl_date_inter datetime

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodebut_in = XDC_DATE_NULLE  OR @va_Horofin_in = XDC_DATE_NULLE
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
	if @va_Delta_in=1
	begin
		select @va_Debut_out=dateadd(day,@va_Delta_in,@va_Horodebut_in)
		select @va_Fin_out=dateadd(day,@va_Delta_in,@va_Horofin_in)
	end
	else if @va_Delta_in=4
	begin
		select @vl_date_inter = @va_Horodebut_in
		select @vl_jour_semaine = datepart(weekday,@va_Horodebut_in)
		while @vl_jour_semaine > 0
		begin
			select @vl_date = dateadd(day,-1,@vl_date_inter)
			select @vl_date_inter = @vl_date
			select @vl_jour_semaine = @vl_jour_semaine -1
		end
		select @va_Debut_out = @vl_date_inter
		select @va_Fin_out = @vl_date_inter
	end
	else
	begin
		select @va_Debut_out=dateadd(day,-1,@va_Horodebut_in)
		select @va_Fin_out=dateadd(day,-1,@va_Horofin_in)
	end
   end

   return ( XDC_OK )
         
go  
