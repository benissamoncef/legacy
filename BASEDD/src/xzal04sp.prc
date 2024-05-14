/*E*/
/*Fichier : $Id: xzal04sp.prc,v 1.5 2020/11/09 13:38:42 devgtie Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/09 13:38:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAL * FICHIER xzal04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Appel >
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	16 jan 1995	: creation
* niepceron	11 Oct 1996	: lecture des libelles dans lib_pre (dem/1235) v1.2
* niepceron     14 Sep 1998	: separation EXP/HIS dem/1696 1.3
* LCL		04/07/17	: modif PRA
* RGR		09/11/2020	: Correction Pb compilation
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Archive le motif d'un appel usager
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAL;04
   @va_Horodate_in	datetime = null
   as 

/* 
*
* SP	XZAL04SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
*
* PARAMETRES EN SORTIE :
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
* Poste Operateur CI uniquement
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Horodate datetime,
   @vl_Site tinyint
         
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      /*A
      ** Le serveur est l'HIStorique
      */
      select  @vl_Site = numero
      from CFG..RES_DIS
      where serveur = @@servername

      /* PRA
      if @@servername = XDC_SQLHIS
      */
      if @vl_Site = XDC_HIS
      begin
      
         
            /*A
            ** Recuperation du jour plein de la date en entree
            */
            
            
            select @vl_Horodate = convert(datetime,convert(char(8),@va_Horodate_in,3))
            select @va_Horodate_in = dateadd(day,1,@vl_Horodate)
        
            
            select horodate,libelle,nombre
            from HIS..APL_HOR, CFG..LIB_PRE
            where CFG..LIB_PRE.notyp=XDC_LIB_MOTIF_APPEL 
              and HIS..APL_HOR.numero=CFG..LIB_PRE.code
              and horodate>=@vl_Horodate
              and horodate<@va_Horodate_in
            order by horodate,CFG..LIB_PRE.code
               
         
      end
      
      else
      
      /*A
      ** Le site n'est pas celui du CI
      ** return XDC_NOK
      */
      
         return XDC_NOK
      
   end
      
   return ( XDC_OK )
         
go  
