/*E*/
/*Fichier : $Id: xzap180sp.prc,v 1.3 2020/08/25 12:31:18 akkaakka Exp $      $Revision: 1.3 $        $Date: 2020/08/25 12:31:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	20/03/17	creation DEM1191
* JMG	17/05/17	correction bug 1.2
* PNI	25/08/20	correction pour avoir l'astreinte PEAGE 1.3 SAE-161
* JPL	02/07/21	Extension du champ "remarque" a 250 caracteres (SAE-287)  1.4
* GGY	26/09/23	Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAP180' and type = 'P')
	drop procedure XZAP180
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  pour evt majeur recupere la derniere proposition d appel
*  pour fermeture entrees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP180

	@va_NumPA_in		smallint,
	@va_Remarque_out	char(250) output
	
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
*
* ARGUMENTS EN SORTIE :
*
* XZAOPT_PropTel 	va_PropTel
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
* Lit une proposition d'appel telephone ou bip d'un plan d'actions
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
		exec @vl_Status = SQL_CI.PRC..XZAP180
			@va_NumPA_in		,
			@va_Remarque_out	 output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP180
			@va_NumPA_in		,
			@va_Remarque_out	 output

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
    declare @vl_Num smallint, @vl_num int, @vl_cle tinyint

   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   
   if @va_NumPA_in = null
  
      return XDC_ARG_INV
  
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition APL d'un plan d'action
   */
   
   begin
      
      select @va_Remarque_out=""
      select @vl_num = evenement, @vl_cle = cle
      from EXP..SAD_PLN
      where numero = @va_NumPA_in

      if @@rowcount = 0
       return (XDC_ARG_INV)

      select  @vl_Num  = max(numero) from
	EXP..SAD_PLN
	where evenement = @vl_num 
	and cle = @vl_cle
	and numero < @va_NumPA_in
      
      if @@rowcount = 0
          return (XDC_OK)	

      select 	@va_Remarque_out=rtrim(remarque)
   	from EXP..SAD_APL
        where numero = @vl_Num
          and rtrim(explication) <> "" and type_d_astreinte=XDC_AST_PEAGE

   end   
   select @va_Remarque_out
   return (XDC_OK)      
#endif
go
