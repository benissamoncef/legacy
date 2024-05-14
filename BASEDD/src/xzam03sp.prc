/*E*/
/*Fichier : $Id: xzam03sp.prc,v 1.10 2021/03/08 13:46:37 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2021/03/08 13:46:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04 Nov 1994	: Creation
* gaborit	17 jan 1995	: modif bug site
* torregrossa	05 Mar 1996	: Si l'alarme sur eqt n'existe pas sur le site local
*                                 alors rechercher le site de l'eqt via sa disponibilite pour effectuer
*                                 cette proc sur le site de l'eqt.(pour resoudre probleme
*                                 de modification d'alarme pas encore repliquee )
*                                 (1024) V1.3
* torregrossa	05 sep 1996	: Ajout test sur DS et DC V1.4
* C.P.		07/01/98	: Correction gestion multi-sites dans 
*				  les procs (V 1.5) (dem / 1536)
* LCL           09/07/07        : Suppression DY
* JMG           02/10/09        : secto DEM887
* PNI		05/02/15	:  @va_TypeAlrm_in passe en smallint DEM1108 v1.8
* JMG           03/03/17 	: regio DEM1220
* ABE		06/01/21   	: MOVIS ajout PRA DEM-SAE93 1.10
* LCL	27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Ecrit les commentaires d'une fiche alarme
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;03
   @va_TypeAlrm_in			smallint = null,
   @va_TypeEqt_in			tinyint = null,
   @va_Eqt_in				smallint = null,
   @va_Commentaitre_in			char(50)
   as 

/* 
*
* SP	XZAM03SP
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_NumFicheAlarme_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_Phrase		va_Commentaitre_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Ecrit les commentaires d'une fiche alarme
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*  NumFicheAlarme inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* update ALM_EQT
* set commentaire = Commentaire
* where numero = NumFicheAlarme
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Alrm		smallint,
   @vl_Site		tinyint,
   @vl_SiteLocal	char(10),
   @vl_Status		int,
   @vl_serveur_pra	 char(6)
   
     
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_TypeAlrm_in = null
   or @va_TypeEqt_in = null
   or @va_Eqt_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   begin
   
   /*A
   ** Si les parametres sont non nuls
   */
       
      if @va_Commentaitre_in = null
        
         select @va_Commentaitre_in = ' '
           
      
      select @vl_Site = sit from EXP..ALM_EQT
      where type 		= @va_TypeAlrm_in
          and type_equipement 	= @va_TypeEqt_in
          and equipement	= @va_Eqt_in
          
      /*! si l'alarme n'a pas encore ete repliquee alors rechercher le site */
      /*! de l'alarme via la disponibilite de l'eqt */
      if @vl_Site is null  
	begin
         	select @vl_Site = sit from EXP..EQT_DSP
           	where type = @va_TypeEqt_in
             	and equipement	 = @va_Eqt_in 
		and dernier = 1
             
		/*! si le site de l'eqt est le site local alors aucune alarme a ete creee : erreur */
         	if @vl_Site = null 
			return ( XDC_NOK )
			
		else if exists (select serveur from CFG..RES_DIS 
				where numero = @vl_Site and serveur = @@servername)
		     return ( XDC_NOK )
			
		
	end

      /*A
      ** La fiche alarme existe
      ** Recherche de la fiche alarme
      ** Affectation des parametres de sorties
      */
      
      if @vl_Site != null
      begin

         /*A
         ** Si le site de la fiche alarme est null
         ** return ( XDC_NOK )
         */
            select @vl_SiteLocal = serveur from CFG..RES_DIS 
            where numero = @vl_Site
            
            if @vl_SiteLocal is null
                return ( XDC_NOK )
         
            /*A
            ** Le site de la fiche alarme est le site local
            ** Update de la table ALM_EQT
            */   
	    
	    /* Init serveur PRA */
            select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@vl_Site
            
	    if @@servername = @vl_SiteLocal
            begin
               
               update EXP..ALM_EQT
               
               set commentaire = @va_Commentaitre_in
               
               where type 		= @va_TypeAlrm_in
                 and type_equipement 	= @va_TypeEqt_in
                 and equipement	 	= @va_Eqt_in
               
               if @@rowcount != 1
                  
                  return ( XDC_NOK )
               else
               
                  return ( XDC_OK ) 
            
            end
            
            else
             
            /*A
            ** Le site de la fiche alarme n'est le site local
            ** Appel de la SP sur le site de la fiche alarme
            */   
            
            if @vl_Site = XDC_CI
            begin

	       /* Redirection de l'action sur le serveur de pra */
               if  @vl_serveur_pra=XDC_SQLDP
	       		exec @vl_Status = SQL_DS.PRC..XZAM;03 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
               							@va_Commentaitre_in

               else 
	       		exec @vl_Status = SQL_CI.PRC..XZAM;03 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
               							@va_Commentaitre_in
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            end
               
            else
               
            if @vl_Site = XDC_VC
            begin
               /* Redirection de l'action sur le serveur de pra */
               if  @vl_serveur_pra=XDC_SQLCI
	       		exec @vl_Status = SQL_CI.PRC..XZAM;03 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
               							@va_Commentaitre_in

               else 
               		exec @vl_Status = SQL_DM.PRC..XZAM;03 	@va_TypeAlrm_in,
		       						@va_TypeEqt_in,
		       						@va_Eqt_in,
	      							@va_Commentaitre_in
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
            else
            if @vl_Site = XDC_DP
            begin
               /* Redirection de l'action sur le serveur de pra */
               if  @vl_serveur_pra=XDC_SQLVC
	       		exec @vl_Status = SQL_DM.PRC..XZAM;03 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
               							@va_Commentaitre_in

               else 
               exec @vl_Status = SQL_DS.PRC..XZAM;03 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
      							@va_Commentaitre_in
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
               
            else
            if @vl_Site = XDC_CA
            begin
            
               exec @vl_Status = SQL_CA.PRC..XZAM;03 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
      							@va_Commentaitre_in
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
            else
            
               return ( XDC_NOK )
            
      end
          
      else
      
      /*A
      ** La fiche alarme n'existe pas
      ** return XDC_NOK
      */
         
         return ( XDC_NOK )
         
           
   end
      
   return ( XDC_OK )
         
go  
