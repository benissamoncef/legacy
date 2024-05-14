/*E*/
/*Fichier : @(#)xzau01sp.prc	1.1      Release : 1.1        Date : 11/15/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau01sp.prc
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
* GGY		26/09/23 	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Cree un nouveau profil et retourne son numero
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;01
   @va_NomProfil_in		char(25),
   @va_NumProfil_out		tinyint = null output,
   @va_Resultat_out		int = null output
   as 

/* 
*
* SP	XZAU01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_NomProfil_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_NumProfil_out
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Cree un nouveau profil et retourne son numero
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
* Si le profil n'existe pas dans la table OPR_PRF, le cree, sinon
* erreur positionnee dans resultat
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
      
   if @va_NomProfil_in = null
      
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
           where nom = @va_NomProfil_in
           
         /*B
         ** Le profil n'existe pas
         ** Insertion dans OPR_PRF du profil
         ** Affectation des paremetres de sortie
         */
         
         if @vl_Profil = null
         begin
            
            select @vl_Profil = max (numero) from CFG..OPR_PRF
            
            if @vl_Profil = null
               select @vl_Profil = 0
               
            insert into CFG..OPR_PRF
            values ( @vl_Profil + 1, @va_NomProfil_in )
            
            select @va_NumProfil_out = @vl_Profil + 1
            
            select @va_Resultat_out = XDC_OK 
            
         end
         
         /*B
         ** Le profil existe deja
         ** Affectation des paremetres de sortie
         */
         
         else
         
         begin
         
            select @va_NumProfil_out = numero from CFG..OPR_PRF
              where nom = @va_NomProfil_in
            
            select @va_Resultat_out = XDC_ECHEC 
            
         end
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
