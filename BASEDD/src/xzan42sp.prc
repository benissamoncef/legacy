/*E*/
/*Fichier : $Id: xzan42sp.prc,v 1.3 1996/12/16 14:20:10 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/12/16 14:20:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan42sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	15 nov 1996	: Creation (DEM/1306)
* guilhou	12 dec 1996	: retourner le numero des jours (DEM/1332) 1.2
* Guilhou	16 dec 1996	: XZAN42 devient XZAN;42 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan42sp.h" */

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

create procedure XZAN;42
   @va_Horodate_in		datetime = null,
   @va_Mode_in			tinyint = null
   as 

/* 
*
* SP	XZAN42SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
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
* NumCompetence inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_debut	datetime,
   @vl_compteur smallint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = XDC_DATE_NULLE 
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
	create table #RES (
		numero 	smallint null,
		numero_jour smallint null)

	/*demande de la liste des libelles des 7 prochains jours*/
	if @va_Mode_in = 1
	begin
		select @vl_compteur=0
		select @vl_debut=@va_Horodate_in

		while @vl_compteur<7
		begin
			insert #RES (numero,numero_jour)
			select datepart(weekday,@vl_debut), datepart(day,@vl_debut)

			select @vl_compteur=@vl_compteur+1
			select @vl_debut=dateadd(day,1,@vl_debut)
		end
	end
	/*demande de la liste des numeros des 31 prochains jours*/
	else if @va_Mode_in = 2
	begin
		select @vl_compteur=0
		select @vl_debut=@va_Horodate_in
		while @vl_compteur<31
		begin
			insert #RES (numero)
			select datepart(day,@vl_debut)

			select @vl_compteur=@vl_compteur+1
			select @vl_debut=dateadd(day,1,@vl_debut)
		end
	end
	/*demande de la liste des numeros des 52 prochaines semaines*/
	else if @va_Mode_in = 3
	begin
		select @vl_compteur=0
		select @vl_debut=@va_Horodate_in
		while @vl_compteur<52
		begin
			insert #RES (numero)
			select datepart(week,@vl_debut)

			select @vl_compteur=@vl_compteur+1
			select @vl_debut=dateadd(week,1,@vl_debut)
		end
	end
   end

   select *
   from #RES

   return ( XDC_OK )
         
go  
