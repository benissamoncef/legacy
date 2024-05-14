/*E*/
/*Fichier : $Id: xzal02sp.prc,v 1.6 2020/11/09 13:36:22 devgtie Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/11/09 13:36:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAL * FICHIER xzal02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Appel >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	03 Nov 1994	: Creation
* gaborit	16 jan 1995	: modif bug existance motif (1.2)
* gaborit	16 jan 1995	: lecture motif dans lib_pre (dem/1235) (1.3)
* niepceron	02 sep 1998	: sep. EXP/HIS (dem/1696) 1.4
* LCL		04/07/17	: Modif PRA
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

create procedure XZAL;02
   @va_Horodate_in	datetime = null,
   @va_Motif_in		tinyint = null,
   @va_Nombre_in	smallint = null
   as 

/* 
*
* SP	XZAL02SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_Octet		va_Motif_in
* XDY_Mot		va_Nombre_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Archive le motif d'un appel usager
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
* Incremente le compteur de la ligne correspondant a l'heure et au motif
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Motif	tinyint,
   @vl_Horodate datetime,
   @vl_Min	int,
   @vl_Sec	int,
   @vl_Site	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
   or @va_Motif_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   if @va_Nombre_in < 1
   
      return ( XDC_OK )
      
   else
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      /*A
      ** Le site est celui du HIS
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
         ** Si le Motif existe en base
         */
         
         if exists (select * from CFG..LIB_PRE where code = @va_Motif_in and notyp=XDC_LIB_MOTIF_APPEL)
         begin
         
            /*A
            ** Recuperation de l'heure pleine de la date en entree
            */
            
            
            select @vl_Horodate = convert(datetime,convert(char(8),@va_Horodate_in,3)+' '+convert(char(2),@va_Horodate_in,8)+':00')
        
        
            /*A
            ** Archivage du motif d'un appel usager
            */
      
                
            /*A
            ** Si l'enregistrement n'existe pas dans APL_HOR
            ** Creation de celui-ci
            */
            
            if not exists (select * from HIS..APL_HOR where numero = @va_Motif_in and horodate = @vl_Horodate )
            
               insert into HIS..APL_HOR
               values ( @va_Motif_in, @vl_Horodate, @va_Nombre_in )
               
            else
            
            /*A
            ** Si l'enregistrement existe dans APL_HOR
            ** Update de celui-ci
            */
               
               update HIS..APL_HOR
               set nombre = nombre + @va_Nombre_in
               where numero 	= @va_Motif_in
                 and horodate 	= @vl_Horodate
              
         end
            
         else
         
         /*A
         ** Le Motif n'existe pas en base
         ** return XDC_NOK
         */
         
            return XDC_NOK 
         
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
