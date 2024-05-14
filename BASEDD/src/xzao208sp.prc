/*E*/
/*Fichier : $Id: xzao208sp.prc,v 1.3 1995/01/28 19:34:58 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/01/28 19:34:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao208sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao208sp.h" */

/* static char *version = "@(#)xzao208.c	1.1 23/18/94 : xzao208" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Capteurs d'un tube
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;208
   @va_Basedd_in        	char(3),
   @va_NomTube_in		char(10),
   @va_TypeCapteur_in		tinyint
   as 

/* 
*
* SP	XZAO208SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_Basedd_in		
* XDY_NomEqt		va_NomTube_in
* XDY_TypeEqt		va_TypeCapteur_in	
*
* PARAMETRES EN SORTIE :
*
* XDY_Eqt		va_NumCapteur_out
* XDY_NomEqt		va_NomCapteur_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
*
* VALEUR RENDUE :
*
* Retourne la liste des Capteurs d'un tube
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
------------------------------------------------------
* HISTORIQUE :
*
* volcic	10 Oct 1994	: Creation
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_NomTube_in ) = null 		select @va_NomTube_in = null
   if @va_TypeCapteur_in = 0		 	select @va_TypeCapteur_in = null
   
   
   if @va_Basedd_in = null
   or @va_NomTube_in = null
   or @va_TypeCapteur_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls  
   */
      
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche la liste des Capteurs d'un Tunnel ou d'un Echangeur
         */
         
         select distinct GCPT.numero, 
         		GCPT.nom, 
         		GCPT.autoroute, 
         		GCPT.PR, 
         		GCPT.sens
         	
         from CFG..EQT_GEN GCPT, CFG..EQT_GEN GTUB, CFG..EQT_CPT CPT
         
           where GTUB.type   		= XDC_EQT_TUB
             and GTUB.nom 		= @va_NomTube_in
             and GCPT.type   		= @va_TypeCapteur_in
             and GCPT.maitre 		= GTUB.numero
             and GCPT.type_maitre 	= XDC_MAI_CPT
             and CPT.numero		= GCPT.numero
             and CPT.type   		= @va_TypeCapteur_in
             
         order by GCPT.numero    
         
      end
      
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche la liste des Capteurs d'un Tunnel ou d'un Echangeur
         */
         
         select distinct GCPT.numero, 
         		GCPT.nom, 
         		GCPT.autoroute, 
         		GCPT.PR, 
         		GCPT.sens
         	
         from CFT..EQT_GEN GCPT, CFT..EQT_GEN GTUB, CFT..EQT_CPT CPT
         
           where GTUB.type   		= XDC_EQT_TUB
             and GTUB.nom 		= @va_NomTube_in
             and GCPT.type   		= @va_TypeCapteur_in
             and GCPT.maitre 		= GTUB.numero
             and GCPT.type_maitre 	= XDC_MAI_CPT
             and CPT.numero		= GCPT.numero
             and CPT.type   		= @va_TypeCapteur_in
             
         order by GCPT.numero    
         
      end

#endif      
   end
      
   return XDC_OK
         
go  
