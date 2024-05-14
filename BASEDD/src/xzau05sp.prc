/*E*/
/*Fichier : $Id: xzau05sp.prc,v 1.3 2020/11/10 08:02:20 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/10 08:02:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau05sp.prc
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
*  Ajoute un utilisateur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;05
   @va_NomUtilisateur_in	char(25),
   @va_NumUtilisateur_out	smallint = null output,
   @va_Resultat_out		int = XDC_LU output
   as 

/* 
*
* SP	XZAU05SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_NomUtilisateur_in
* 
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumUtilisateur_out
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Cree un utilisateur
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
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
   
   @vl_User	smallint,
   @vl_Site	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NomUtilisateur_in = null
      
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
      
         select @vl_User = numero from CFG..OPR_NOM
           where nom = @va_NomUtilisateur_in
           
         /*B
         ** L'utilisateur n'existe pas
         ** Insertion dans OPR_NOM de l'utilisateur
         */
         
         if @vl_User = null
         begin
         
            select @vl_User = max (numero) from CFG..OPR_NOM
            
            if @vl_User = null
               select @vl_User = 0
                  
            select @va_NumUtilisateur_out = @vl_User + 1
            
            insert into CFG..OPR_NOM
            values ( @va_NumUtilisateur_out, @va_NomUtilisateur_in )
            
            select @va_Resultat_out = XDC_AJOUT
         
         end
         
         /*B
         ** L'utilisateur existe deja
         ** return XDC_NOK
         */
         
         else
           
            select @va_Resultat_out = XDC_ECHEC
         
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
