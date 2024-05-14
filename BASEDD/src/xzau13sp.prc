/*E*/
/*Fichier : $Id: xzau13sp.prc,v 1.4 2020/11/10 08:02:51 devgtie Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/10 08:02:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* volcic	24 Nov 1994	: Ajout parametre resultat (v1.2)
* LCL		05/07/17	: Modif PRA
* RGR		10/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Supprime un profil
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;13
   @va_NumProfil_in		tinyint = null,
   @va_Resultat_out		int = XDC_LU output
   as 

/* 
*
* SP	XZAU13SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumProfil_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Supprime un profil, a condition qu'aucun utilisateur n'est ce profil
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
   
   @vl_Profil	tinyint,
   @vl_Site	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   select @va_Resultat_out = XDC_LU 
               
   if @va_NumProfil_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
      
         select @vl_Profil = numero from CFG..OPR_PRF
           where numero = @va_NumProfil_in
           
         /*B
         ** Le profil existe
         ** Suppression dans OPR_PRF_FCT et dans OPR_PRF du profil
         */
         
         if @vl_Profil != null
         begin
            
            /*B
            ** Verification qu'aucun utilisateur n'est ce profil
            */
            
            if ( select count (*) from CFG..OPR_LOG_MDP where profil = @va_NumProfil_in ) = 0
            begin
            
               delete CFG..OPR_PRF_FCT
               where profil = @va_NumProfil_in
               
               delete CFG..OPR_PRF
               where numero = @va_NumProfil_in
            
            end
            
            else
            
            /*B
            ** Il existe un ou plusieurs utilisateurs avec ce profil
            */
               select @va_Resultat_out = XDC_ECHEC 
                   
         end
            
         /*B
         ** Le profil ou l'utilisateur n'existe pas
         ** return XDC_NOK
         */
         
         else
            
            return XDC_NOK
         
      end
      
      else
      
      /*A
      ** Le site n'est pas celui du CI
      ** return XDC_NOK
      */
      
         return XDC_NOK
      
      
      return ( XDC_OK )
   
   end
         
go  
