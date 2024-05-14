/*E*/
/*Fichier : $Id: xzao209sp.prc,v 1.3 2007/10/15 10:52:41 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/10/15 10:52:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao209sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao209sp.h" */

/* static char *version = "@(#)xzao209.c	1.1 23/18/94 : xzao209" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Sequences autorisees d'un tube
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO209' and type = 'P')
	drop procedure XZAO209
go


create procedure XZAO209
   @va_Basedd_in        	char(3),
   @va_NumTube_in		smallint
   as 

/* 
*
* SP	XZAO209SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_Basedd_in		
* XDY_Eqt		va_NumTube_in	
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_NumSequence_out
* XDY_Nom		va_NomSequence_out
* XDY_Octet		va_Bit_out
*
* VALEUR RENDUE :
*
* Retourne la liste des Sequences autorisees d'un tube
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
* JPL		15 Oct 2007	: Renommage sans ';' 1.3
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_NumTube_in = null
    
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
         ** Recherche la liste des Sequences d'un Tunnel ou d'un Tube
         */
         
         select distinct SQAT.sequence, 
         		SQT.nom, 
         		SQAT.bit_comm
         	
         from CFG..EQT_SQA_TUB SQAT, CFG..EQT_SEQ_TUB SQT
         
           where SQAT.tube = @va_NumTube_in
             and SQT.numero 	= SQAT.sequence
             
         order by SQAT.sequence    
         
      end
      
#ifdef CI      
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche la liste des Sequences d'un Tunnel ou d'un Tube
         */
         
         select distinct SQAT.sequence, 
         		SQT.nom, 
         		SQAT.bit_comm
         	
         from CFT..EQT_SQA_TUB SQAT, CFT..EQT_SEQ_TUB SQT
         
           where SQAT.tube = @va_NumTube_in
             and SQT.numero 	= SQAT.sequence
             
         order by SQAT.sequence    
         
      end
#endif      
      
   end
      
   return XDC_OK
         
go  
