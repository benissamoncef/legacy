/*E*/
/*Fichier : $Id: xzau02sp.prc,v 1.1 1994/11/15 12:16:29 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 12:16:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des fonctions (nom + numero) associes a un profil
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;02
   @va_NumProfil_in		tinyint = null
   as 

/* 
*
* SP	XZAU02SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumProfil_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des fonctions (nom + numero) associes a un profil
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
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Profil	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumProfil_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      select @vl_Profil = numero from CFG..OPR_PRF
        where numero = @va_NumProfil_in
           
      /*A
      ** Le profil existe
      ** Recherche de la liste des fonctions associees au profil
      */
         
      if @vl_Profil != null
      begin
         
         select FCT.numero, 
         	FCT.nom
         
         from CFG..OPR_PRF_FCT PFCT, CFG..OPR_FCT_NOM FCT
         
         where FCT.numero 	= PFCT.fonction
           and PFCT.profil 	= @va_NumProfil_in
         
            
      end
         
      /*B
      ** Le profil n'existe pas
      ** return XDC_NOK 
      */
         
      else
         
         return XDC_NOK
      
      
      return ( XDC_OK )
   
   end
         
go  
