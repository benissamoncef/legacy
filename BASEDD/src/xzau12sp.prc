/*E*/
/*Fichier : $Id: xzau12sp.prc,v 1.4 2020/11/10 08:02:50 devgtie Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/10 08:02:50 $
------------------------------------------------------
* STERIA-GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* gaborit	15 dec 1994	: modif 1.2
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
*  Supprime un profil utilisateur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;12
   @va_NumUtilisateur_in	smallint = null,
   @va_NumProfil_in		tinyint = null
   as 

/* 
*
* SP	XZAU12SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumUtilisateur_in
* XDY_Octet		va_NumProfil_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Supprime un profil utilisateur
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
   @vl_User	smallint,
   @vl_Site	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumProfil_in = null
   or @va_NumUtilisateur_in = null
      
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
           
         select @vl_User = numero from CFG..OPR_NOM
           where numero = @va_NumUtilisateur_in
           
         /*B
         ** Le profil et l'utilisateur existent
         ** Suppression dans OPR_LOG_MDP de la fonction
         */
         
         if @vl_Profil != null and @vl_User != null
            
            delete CFG..OPR_LOG_MDP
            where profil 	= @va_NumProfil_in
              and operateur	= @va_NumUtilisateur_in
            
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
