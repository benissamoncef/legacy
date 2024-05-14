/*E*/
/*Fichier : $Id: xzam04sp.prc,v 1.11 2021/03/08 13:46:32 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2021/03/08 13:46:32 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04 Nov 1994	: Creation
* gaborit	17 jan 1995	: modif bug site (1.2)
* guilhou	06 mai 1996	: ajout numero de l'operateur qui acquitte (1.3) DEM/919
* torregrossa	05 sep 1996	: Ajout test sur DS et DC V1.4
* C.P.		07/01/98	: Correction gestion multi-sites dans 
*				  les procs (V 1.5) (dem / 1536)
* torregrossa	30 Jan 1998	: Correction gestion multi-sites (1536) V1.6
* claudel       09 Jul 2007     : Suppression DY
* JMG           02/10/09        : secto DEM887
* PNI		04/11/14	: @va_TypeAlrm_in passe ne smallint DEM1108 v1.9
* JMG   	09/03/17        : REGIONALISATION - DEM1220 1.10
* CGR		06/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.11
* LCL	27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Acquitte une fiche alarme
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;04
   @va_TypeAlrm_in			smallint = null,
   @va_TypeEqt_in			tinyint = null,
   @va_Eqt_in				smallint = null,
   @va_numOperateur_in			smallint = null
   as 

/* 
*
* SP	XZAM04SP
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_NumFicheAlarme_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_Eqt		va_numOperateur_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Acquitte d'une fiche alarme
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
* set operateur_acquit = XDC_VRAI
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
   @vl_Status		int,
   @vl_SiteLocal	char(10),
   @vl_serveur_pra	char(6)
     
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_TypeAlrm_in = null
   or @va_TypeEqt_in = null
   or @va_Eqt_in = null
   or @va_numOperateur_in = null      
      return ( XDC_ARG_INV )
   
   else   
   
   begin
   
   /*A
   ** Si les parametres sont non nuls
   */
       
      select @vl_Alrm = nombre_d_apparitions from EXP..ALM_EQT
        where type 		= @va_TypeAlrm_in
          and type_equipement 	= @va_TypeEqt_in
          and equipement	= @va_Eqt_in
          
      /*A
      ** La fiche alarme existe
      ** Recherche de la fiche alarme
      ** Affectation des parametres de sorties
      */
      
      if @vl_Alrm != null
      begin
      
         select @vl_Site = sit from EXP..ALM_EQT
           where type 		 = @va_TypeAlrm_in
             and type_equipement = @va_TypeEqt_in
             and equipement	 = @va_Eqt_in
             
         if @vl_Site != null
         begin
            select @vl_SiteLocal = serveur from CFG..RES_DIS 
            where numero = @vl_Site
            
            if @vl_SiteLocal is null
                return XDC_NOK
         end       
         else
         
         /*A
         ** Si le site de la fiche alarme est null
         ** return ( XDC_NOK )
         */
            
            return ( XDC_NOK )
                 
               
            /*A
            ** Le site de la fiche alarme est le site local
            ** Update de la table ALM_EQT
            */   
            
            if @@servername = @vl_SiteLocal
            begin
               
               update EXP..ALM_EQT
               
               set operateur_acquit = @va_numOperateur_in
               
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
            
	    /* Init serveur PRA */
	    select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=@vl_Site

            if @vl_Site = XDC_CI
            begin
            	if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAM;04   @va_TypeAlrm_in,
								@va_TypeEqt_in,
								@va_Eqt_in,
								@va_numOperateur_in
		else
               		exec @vl_Status = SQL_CI.PRC..XZAM;04 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
           							@va_numOperateur_in 
               	if @vl_Status != XDC_OK
               		return ( @vl_Status )
            end
               
            else 
            if @vl_Site = XDC_VC
            begin
            	if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAM;04   @va_TypeAlrm_in,
								@va_TypeEqt_in,
								@va_Eqt_in,
								@va_numOperateur_in
		else
               		exec @vl_Status = SQL_DM.PRC..XZAM;04 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
              							@va_numOperateur_in 
               	if @vl_Status != XDC_OK
                	return ( @vl_Status )
            end
               
            else
            if @vl_Site = XDC_DP
            begin
	    	if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAM;04   @va_TypeAlrm_in,
								@va_TypeEqt_in,
								@va_Eqt_in,
								@va_numOperateur_in
		else            
               		exec @vl_Status = SQL_DS.PRC..XZAM;04 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
              							@va_numOperateur_in 
               	if @vl_Status != XDC_OK
               		return ( @vl_Status )
            end
               
            else
            if @vl_Site = XDC_CA
            begin
            
               exec @vl_Status = SQL_CA.PRC..XZAM;04 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
              						@va_numOperateur_in 
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
