/*E*/
/*Fichier : @(#)xzau11sp.prc	1.2      Release : 1.2        Date : 11/29/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau11sp.prc
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
*  Retourne le nom, le profil et les fonctions autorisees sur la machine si le login
*  et le mot de passe sont corrects
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;11
   @va_LoginUtilisateur_in	char(8),
   @va_MotDePasse_in		char(8),
   @va_Machine_in		char(10),
   @va_NumUtilisateur_out	smallint = null output,
   @va_Utilisateur_out		char(25) = null output,
   @va_Machine_out		smallint = null output,
   @va_NumProfil_out		tinyint = null output,
   @va_NomProfil_out		char(25) = null output
   as 

/* 
*
* SP	XZAU11SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Login	va_LoginUtilisateur_in
* XDY_Login	va_MotDePasse_in
* XDY_NomMachine	va_Machine_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot	va_NumUtilisateur_out
* XDY_Nom	va_Utilisateur_out
* XDY_Mot	va_NumProfil_out
* XDY_Nom	va_NomProfil_out
* XDY_Liste	ListeFonctions 
*
* VALEUR RENDUE :
*
* Retourne le nom, le profil et les fonctions autorisees sur la machine si le login
* et le mot de passe sont corrects
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
   
   @vl_MotPasse	char(8)
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_LoginUtilisateur_in = null
   or @va_MotDePasse_in = null
   or @va_Machine_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      select @vl_MotPasse = mot_de_passe from CFG..OPR_LOG_MDP
        where login = @va_LoginUtilisateur_in
        
      /*A
      ** Le mot de passe est correct
      */
      
      if  @vl_MotPasse = @va_MotDePasse_in
      begin
      
         /*B
         ** Affectation des parametres de sortie
         */
         
         select @va_Machine_out		= EQT.numero,
         	@va_NumUtilisateur_out 	= NOM.numero, 
         	@va_Utilisateur_out 	= NOM.nom,
         	@va_NumProfil_out	= PRF.numero,
         	@va_NomProfil_out	= PRF.nom
	 
	 from  CFG..EQT_GEN EQT, CFG..OPR_LOG_MDP IDF, CFG..OPR_NOM NOM, CFG..OPR_PRF PRF
	 
	 where EQT.type			= XDC_EQT_MAC
	   and EQT.nom	 		= @va_Machine_in
	   and IDF.login 		= @va_LoginUtilisateur_in
	   and IDF.mot_de_passe 	= @va_MotDePasse_in
	   and NOM.numero 		= IDF.operateur
	   and PRF.numero		= IDF.profil
	           	
         /*B
         ** Recherche de la liste des fonctions
         */
         
         select PFCT.numero,
         	PFCT.nom
         	
	 from CFG..OPR_FCT_MAC FMAC, CFG..OPR_PRF_FCT FPRF, CFG..OPR_FCT_NOM PFCT     	
         
         where FMAC.machine = @va_Machine_out
           and FPRF.profil  = @va_NumProfil_out
           and PFCT.numero  = FPRF.fonction
           and PFCT.numero  = FMAC.fonction
      
      end
      
   end
   
   return ( XDC_OK )
     
go  
