/*E*/
/*Fichier : $Id: xzau03sp.prc,v 1.3 2020/11/10 08:02:18 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/10 08:02:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
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
*  Ajoute une fonction a un profil
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;03
   @va_NumProfil_in		tinyint = null,
   @va_NumFonction_in		smallint = null
   as 

/* 
*
* SP	XZAU03SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumProfil_in
* XDY_Mot		va_NumFonction_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Ajoute une fonction a un profil
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
   @vl_Fonction	smallint,
   @vl_Numero	tinyint	,
   @vl_Site 	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumProfil_in = null
   or @va_NumFonction_in = null
      
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
           
         select @vl_Fonction = numero from CFG..OPR_FCT_NOM
           where numero = @va_NumFonction_in
           
         /*B
         ** Le profil et la fonction existent
         ** Insertion dans OPR_PRF_FCT de la fonction
         */
         
         if @vl_Profil != null and @vl_Fonction != null
         begin
         
            select @vl_Numero = profil from CFG..OPR_PRF_FCT 
               where profil   = @va_NumProfil_in
                 and fonction = @va_NumFonction_in
              
            if @vl_Numero = null
            
               insert into CFG..OPR_PRF_FCT
               values ( @va_NumProfil_in, @va_NumFonction_in )
               
            else 
            
               return XDC_NOK   
         
         end
            
         /*B
         ** Le profil ou la fonction n'existe pas
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
