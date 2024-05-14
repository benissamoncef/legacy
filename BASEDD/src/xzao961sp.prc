/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao961sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Création de la procédure faisant un select dans la table des temps de parcours FCD: TDP_FCD
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO961SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate      Horodate_debut_In
* XDY_Horodate      Horodate_fin_In
* XDY_Autoroute     Autoroute_In
* XDY_PR            PR_debut_In
* XDY_PR            PR_fin_In
* XDY_Sens          Sens_In
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
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
* Select dans la table TDP_FCD
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* GR    23/07/2020 : Creation DEM-SAE152 1.0

------------------------------------------------------*/


use PRC
go

if exists (select * from sysobjects where name = 'XZAO961' and type = 'P')
drop procedure XZAO961
go


create procedure XZAO961
@va_Horodate_Debut_In  datetime,
@va_Horodate_Fin_In    datetime,
@va_Autoroute_In       tinyint,
@va_PR_Debut_In        T_PR,
@va_PR_Fin_In          T_PR,
@va_Sens_In            tinyint
as
   
declare @vl_PR int
declare @vl_Horodate datetime
   
/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_INV   
*/
      
if @va_Autoroute_In = null or @va_Horodate_Debut_In = null or @va_Horodate_Fin_In = null or @va_Sens_In = null
begin
return XDC_ARG_INV
end

else
begin

    /*B echange des PR pour que PR1<PR2 */
    if @va_PR_Debut_In>@va_PR_Fin_In
    begin
    select @vl_PR=@va_PR_Debut_In
    select @va_PR_Debut_In=@va_PR_Fin_In
    select @va_PR_Fin_In=@vl_PR
    end

    /*C echange des horodate pour que Horodate_debut<Horodate_fin*/
    if @va_Horodate_Debut_In>@va_Horodate_Fin_In
    begin
    select @vl_Horodate=@va_Horodate_Debut_In
    select @va_Horodate_Debut_In=@va_Horodate_Fin_In
    select @va_Horodate_Fin_In=@vl_Horodate
    end
      
    /* 
    ** retourne les TDP correspondants                   
    */
    begin
        select PR_debut,
               PR_fin,
               tdp,
               validite,
               horodate
        from EXP..TDP_FCD TDP_FCD
        where    @va_Horodate_Debut_In<=TDP_FCD.horodate
             and @va_Horodate_Fin_In>=TDP_FCD.horodate
             and @va_Sens_In=TDP_FCD.sens
             and @va_Autoroute_In=TDP_FCD.autoroute
             and @va_PR_Debut_In<=TDP_FCD.PR_debut
             and @va_PR_Fin_In>=TDP_FCD.PR_fin
    end
      
    return XDC_OK		
end
         
go				
