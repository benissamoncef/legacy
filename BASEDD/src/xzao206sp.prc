/*E*/
/*Fichier : $Id: xzao206sp.prc,v 1.2 1995/01/09 13:18:18 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:18:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao206sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao206sp.h" */

/* static char *version = "@(#)xzao206.c	1.1 23/18/94 : xzao206" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Panneaux d'un tube ou d'un echangeur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;206
   @va_Basedd_in        	char(3),
   @va_NumTubeOuEchangeur_in	smallint,
   @va_TypeEqt_in		tinyint
   as 

/* 
*
* SP	XZAO206SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_Basedd_in		
* XDY_Eqt		va_NumTubeOuEchangeur_in		
* XDY_TypeEqt		va_TypeEqt_in		
*
* PARAMETRES EN SORTIE :
*
* XDY_Eqt		va_NumPanneau_out
* XDY_NomEqt		va_NomPanneau_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* XDY_Octet		va_Rang1_out
* XDY_Octet		va_Rang2_out
*
* VALEUR RENDUE :
*
* Retourne la liste des Panneaux d'un tube ou d'un echangeur
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
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_NumTubeOuEchangeur_in = null
   or @va_TypeEqt_in = null
    
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
         ** Recherche la liste des Panneaux d'un Tunnel ou d'un Echangeur
         */
         
         select distinct GPAN.numero, 
         		GPAN.nom, 
         		GPAN.autoroute, 
         		GPAN.PR, 
         		GPAN.sens,
         		PAN.rang_comm, 
         		PAN.rang_comm2 
         	
         from CFG..EQT_GEN GPAN, CFG..EQT_PAN PAN
         
           where GPAN.type   		= XDC_EQT_PAN
             and GPAN.numero 		= @va_NumTubeOuEchangeur_in
             and GPAN.maitre 		= GPAN.numero
             and GPAN.type_maitre 	= @va_TypeEqt_in
             and PAN.numero		= GPAN.numero
             
         order by GPAN.numero    
         
      end

#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche la liste des Panneaux d'un Tunnel ou d'un Echangeur
         */
         
         select distinct GPAN.numero, 
         		GPAN.nom, 
         		GPAN.autoroute, 
         		GPAN.PR, 
         		GPAN.sens,
         		PAN.rang_comm, 
         		PAN.rang_comm2 
         	
         from CFT..EQT_GEN GPAN, CFT..EQT_PAN PAN
         
           where GPAN.type   		= XDC_EQT_PAN
             and GPAN.numero 		= @va_NumTubeOuEchangeur_in
             and GPAN.maitre 		= GPAN.numero
             and GPAN.type_maitre 	= @va_TypeEqt_in
             and PAN.numero		= GPAN.numero
             
         order by GPAN.numero    
         
      end
#endif      
      
   end
      
   return XDC_OK
         
go  
