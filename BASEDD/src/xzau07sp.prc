/*E*/
/*Fichier : $Id: xzau07sp.prc,v 1.5 2021/02/12 14:39:36 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2021/02/12 14:39:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* volcic	29 Nov 1994	: Modif connection distante (v1.2)
* LCL		05/07/17	: Modif PRA
* RGR		10/11/2020	: Correction Pb de compilation
* ABE		06/01/21	: MOVIS ajout PRA DEM-SAE93 1.5
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Modifie le nom d'un utilisateur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;07
   @va_NumUtilisateur_in	smallint = null,
   @va_NumProfil_in		tinyint = null,
   @va_AncienMotDePasse_in	char(8),
   @va_NouveauMotDePasse_in	char(8),
   @va_Resultat_out		int = null output
   as 

/* 
*
* SP	XZAU07SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumUtilisateur_in
* XDY_Octet		va_NumProfil_in
* XDY_Login		va_AncienMotDePasse_in
* XDY_Login		va_NouveauMotDePasse_in
* 
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Modifie le nom d'un utilisateur
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
   @vl_retcode	int,
   @vl_Profil	tinyint,
   @vl_Site	tinyint,
   @vl_serveur_pra char(6)
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumUtilisateur_in = null
   or @va_NumProfil_in = null
   or @va_AncienMotDePasse_in = null
   or @va_NouveauMotDePasse_in = null
      
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
           where numero = @va_NumUtilisateur_in
           
         select @vl_Profil = numero from CFG..OPR_PRF
           where numero = @va_NumProfil_in
           
         /*B
         ** L'utilisateur et le profil existent
         ** Insertion dans OPR_LOG_MDP du nouveau mot de passe
         ** Affectation du parametre de sortie
         */
         
         if @vl_User != null and @vl_Profil != null
         begin
         
            update CFG..OPR_LOG_MDP
            set mot_de_passe = @va_NouveauMotDePasse_in
            where profil 	= @va_NumProfil_in
              and operateur 	= @va_NumUtilisateur_in
              and mot_de_passe	= @va_AncienMotDePasse_in
            
            
            if @@rowcount = 1
                
                select @va_Resultat_out = XDC_OK
            
            else
            
                select @va_Resultat_out = null
         
         end
         
         /*B
         ** L'utilisateur ou le profil n'existe pas
         ** return XDC_NOK
         */
         
         else
            
            return XDC_NOK
         
      end
      
      else
      
      begin
      
      /*A
      ** Le site n'est pas celui du CI
      ** Execution distante
      */

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@vl_Site

	if @vl_serveur_pra = XDC_SQLVC
         exec @vl_retcode = SQL_DM.PRC..XZAU;07 @va_NumUtilisateur_in,
   						@va_NumProfil_in,
   						@va_AncienMotDePasse_in,
   						@va_NouveauMotDePasse_in,
   						@va_Resultat_out output

	else
         exec @vl_retcode = SQL_CI.PRC..XZAU;07 @va_NumUtilisateur_in,
   						@va_NumProfil_in,
   						@va_AncienMotDePasse_in,
   						@va_NouveauMotDePasse_in,
   						@va_Resultat_out output
   
         return @vl_retcode 
         
      end
      
      return ( XDC_OK )
   
   end
         
go  
