/*E*/
/*Fichier : $Id: xzao210sp.prc,v 1.3 2007/10/15 10:52:42 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/10/15 10:52:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao210sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao210sp.h" */

/* static char *version = "@(#)xzao210.c	1.1 23/18/94 : xzao210" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Sequences autorisees d'un echangeur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO210' and type = 'P')
	drop procedure XZAO210
go


create procedure XZAO210
   @va_Basedd_in        	char(3),
   @va_NumEchangeur_in		smallint
   as 

/* 
*
* SP	XZAO210SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_Basedd_in		
* XDY_Eqt		va_NumEchangeur_in	
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_NumSequence_out
* XDY_Nom		va_NomSequence_out
* XDY_Octet		va_Bit_out
*
* VALEUR RENDUE :
*
* Retourne la liste des Sequences autorisees d'un echangeur
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
* JPL		15 Oct 2007	: Renommage sans ';' 1.3
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_NumEchangeur_in = null
    
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
         ** Recherche la liste des Sequences d'un Tunnel ou d'un Echangeur
         */
         
         select distinct SQAE.sequence, 
         		SQE.nom, 
         		SQAE.bit_comm
         	
         from CFG..EQT_SQA_ECH SQAE, CFG..EQT_SEQ_ECH SQE
         
           where SQAE.echangeur = @va_NumEchangeur_in
             and SQE.numero 	= SQAE.sequence
             
         order by SQAE.sequence    
         
      end
      
#ifdef CI      
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche la liste des Sequences d'un Tunnel ou d'un Echangeur
         */
         
         select distinct SQAE.sequence, 
         		SQE.nom, 
         		SQAE.bit_comm
         	
         from CFT..EQT_SQA_ECH SQAE, CFT..EQT_SEQ_ECH SQE
         
           where SQAE.echangeur = @va_NumEchangeur_in
             and SQE.numero 	= SQAE.sequence
             
         order by SQAE.sequence    
         
      end
#endif      
      
   end
      
   return XDC_OK
         
go  
