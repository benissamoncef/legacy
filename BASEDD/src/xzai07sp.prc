/*E*/
/*Fichier : $Id: xzai07sp.prc,v 1.5 2019/01/29 11:15:47 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2019/01/29 11:15:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* gaborit	28 dec 1994	: modif integration
* LCL		04/07/17	: modif PRA
* LCL		04/07/17	: correction PRA
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Recherche l'horodate du message a proposer et s'il est confidentiel ou non
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;07
   @va_NumEvt_in		int = null,
   @va_CleEvt_in		tinyint = null,
   @va_Horodate_out		datetime = null output,
   @va_Confidentiel_out		bit output
   as 

/* 
*
* SP	XZAI07SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Evt		va_NumEvt_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Horodate		va_Horodate_out
* XDY_Booleen 		va_Confidentiel_out
*
* VALEUR RENDUE :
*
* Recherche l'horodate du message a proposer et s'il est confidentiel ou non
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* SAD CI uniquement
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
   
   @vl_Evt	int,
   @vl_Horodate	datetime,
   @vl_Conf	bit,
   @vl_Site	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumEvt_in = null
   or @va_CleEvt_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      /*A
      ** Travail dans la base EXP
      */   
      
      select @vl_Evt = numero from EXP..FMC_TFM
        where numero 	= @va_NumEvt_in
          and cle 	= @va_CleEvt_in
   
      /*A
      ** L'evenement existe
      */
      
      if @vl_Evt != null
      begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = numero
			from CFG..RES_DIS
			where serveur = @@servername and type = XDC_TYPEM_SD

			/* Si le site est CI ou CA */
			if @@rowcount = 1
			begin
            
                select @va_Horodate_out=max(horodate)
            
                from EXP..FMC_TFM
            
                where numero 	= @va_NumEvt_in
                  and cle 	= @va_CleEvt_in
      
                select @va_Confidentiel_out = XDC_NON
                
                select @va_Confidentiel_out = confidentiel
                from EXP..FMC_TFM
            
                where numero 	= @va_NumEvt_in
                  and cle 	= @va_CleEvt_in
                  and confidentiel=XDC_OUI
                  and horodate=@va_Horodate_out
                
                  
            
         end
                  
         else
         
         /*
         ** Le site n'est pas le CI
         ** return ( XDC_NOK )
         */
         
            return ( XDC_NOK )
      end
              
      else
   
      /*A
      ** L'evenement n'existe pas
      ** return ( XDC_NOK )
      */
         select @va_Horodate_out=null, @va_Confidentiel_out=XDC_OUI
      
   return ( XDC_OK )
   
   end
         
go  
