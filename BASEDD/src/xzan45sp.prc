/*E*/
/*Fichier : $Id: xzan45sp.prc,v 1.2 2011/04/20 09:36:24 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/04/20 09:36:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan45sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	22/05/00	: Creation
* JMG		20/04/11	: ajout drop procedure
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan45sp.h" */

/* static char *version = "$Id: xzan45sp.prc,v 1.2 2011/04/20 09:36:24 gesconf Exp $ $Revision: 1.2 $ $Date: 2011/04/20 09:36:24 $"; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
* retourne une liste de periodes discontinues
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAN45
go

create procedure XZAN45
   @va_debut_in		datetime = null,
   @va_fin_in		datetime = null
   as 

/* 
*
* SP	XZAN45SP
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
   declare @vl_inter datetime,
	   @vl_hdebut int,
	   @vl_mindebut int,
	   @vl_hfin int,
	   @vl_minfin int,
	   @vl_debut datetime,
	   @vl_fin datetime
   
   select @vl_inter = @va_debut_in,
	  @vl_hdebut = datepart(hour,@va_debut_in),
	  @vl_mindebut = datepart(minute,@va_debut_in),
	  @vl_hfin = datepart(hour,@va_fin_in),
	  @vl_minfin = datepart(minute,@va_fin_in)

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_debut_in = XDC_DATE_NULLE  OR @va_fin_in = XDC_DATE_NULLE
      return ( XDC_ARG_INV )
   
   create table #HORO(debut datetime, fin datetime)

   while @vl_inter <= @va_fin_in
   begin
	select @vl_debut = dateadd(hour, - @vl_hdebut,@vl_inter)
        select @vl_debut = dateadd(minute, - @vl_mindebut, @vl_debut)

	select @vl_fin = dateadd(hour, @vl_hfin, @vl_debut)
	select @vl_fin = dateadd(minute, @vl_minfin, @vl_fin)

	select @vl_debut = @vl_inter

	if @vl_fin < @vl_debut
		select @vl_fin = dateadd(day,1,@vl_fin)
	insert into #HORO values(@vl_debut, @vl_fin)

	select @vl_debut = dateadd(day,1,@vl_debut)
	select @vl_inter = dateadd(day,1,@vl_inter)
   end

   select * from #HORO

   return ( XDC_OK )
         
go  
