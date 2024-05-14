/*E*/
/*Fichier : @(#)xzam10sp.prc	1.9      Release : 1.9        Date : 03/09/17
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	05 Nov 1994 : Creation
* torregrossa	05 sep 1996 : Ajout test sur DS et DC V1.2
* C.P.		07/01/98 : Correction gestion multi-sites dans 
*				les procs (V 1.4) (dem / 1536)
* C.T.		30/01/98 : Correction erreur gestion multi-sites (V 1.5) (dem / 1536)
* LCL           09/07/07 : Suppression DY (V1.6)
* JMG           02/10/09        : secto DEM887
* PNI		05/02/15 : @va_TypeAlrm_in passe en smallint DEM1108 v1.8
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.9
* ABE	07/01/21	: MOVIS ajout du serveur PRA DEM-SAE93 1.10
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Associe une demande de travaux EasyCam a une fiche alarme
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;10
   @va_TypeAlrm_in			smallint = null,
   @va_TypeEqt_in			tinyint = null,
   @va_Eqt_in				smallint = null,
   @va_Demande_in			int = null
   as 

/* 
*
* SP	XZAM10SP
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_NumFicheAlarme_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_Entier		va_Demande_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Associe une demande de travaux EasyCam a une fiche alarme
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumFicheAlarme inconnu
* Alarme active
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* update ALM_EQT
* set DT = DemandeTravaux where numero = NumFicheAlarme
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
   @vl_Etat		bit,
   @vl_serveur_pra		char(6)
     
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
             
         /*A
         ** Si le site de la fiche alarme est non null
         ** Traitement
         */
         
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
            
	    /* Init serveur PRA */
	    select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=@vl_Site

            if @@servername = @vl_SiteLocal
            begin
               
               update EXP..ALM_EQT
               
               set demande_de_travaux = @va_Demande_in
               
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
		         if @vl_serveur_pra = XDC_SQLDP

			         exec @vl_Status = SQL_DS.PRC..XZAM;10 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
               						@va_Demande_in            
	    	      else
               		exec @vl_Status = SQL_CI.PRC..XZAM;10 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
               						@va_Demande_in
            
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            end
               
            else
               
            if @vl_Site = XDC_VC
            begin
		         if @vl_serveur_pra = XDC_SQLCI

			         exec @vl_Status = SQL_CI.PRC..XZAM;10 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
               						@va_Demande_in            
	    	      else            
		            exec @vl_Status = SQL_DM.PRC..XZAM;10 	@va_TypeAlrm_in,
		       						@va_TypeEqt_in,
		       						@va_Eqt_in,
		       						@va_Demande_in
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
               
            if @vl_Site = XDC_DP
            begin
		         if @vl_serveur_pra = XDC_SQLVC

			         exec @vl_Status = SQL_DM.PRC..XZAM;10 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
               						@va_Demande_in            
	    	      else            
			         exec @vl_Status = SQL_DS.PRC..XZAM;10 	@va_TypeAlrm_in,
		       						@va_TypeEqt_in,
		       						@va_Eqt_in,
		       						@va_Demande_in
               
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
