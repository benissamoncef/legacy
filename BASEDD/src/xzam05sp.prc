/*E*/
/*Fichier : $Id: xzam05sp.prc,v 1.14 2021/03/08 13:46:25 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2021/03/08 13:46:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	05 Nov 1994	: Creation
* torregrossa   29 Mai 1995	: ajout un argument indiquant
*                                 une suppression forcee de 
*                                 l'alarme
* torregrossa   30 Mai 1995	: correction appel autre site 
* gaborit 	04 Aou	1995	: correction mise a jour dispo si suppression alarme active (1.5)
* torregrossa   23 Oct 1995	: correction test sur nbre d'alarme supprimer (1.6)
*                                 pas de retour en erreur si alarme n'existe pas
* torregrossa	05 sep 1996	: Ajout test sur DS et DC V1.7
* C.P.		07/01/98	: Correction gestion multi-sites dans 
*				  les procs (V 1.8) (dem / 1536)
* torregrossa	30 Jan 1998	: Correction erreur gestion multi-sites (dem / 1536) (V 1.9)
* claudel       09 Jul 2007     : Suppression DY (V1.10)
* niepceron	24 Jun 2009	: type alarme tinyint -> smallint v1.11
* JMG           02/10/09        : secto DEM887 1.12
* JMG   	09/03/17        : REGIONALISATION - DEM1220 1.13
* CGR		06/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.14
* LCL	27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Supprime une fiche alarme
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;05
   @va_TypeAlrm_in			smallint = null,
   @va_TypeEqt_in			tinyint = null,
   @va_Eqt_in				smallint = null,
   @va_ForcerSuppression_in		bit
   as 

/* 
*
* SP	XZAM;05SP
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_NumFicheAlarme_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_Booleen		va_ForcerSuppression_in 
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Supprime d'une fiche alarme
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
* delete ALM_EQT
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
   @vl_Etat		bit,
   @vl_horodate		datetime,
   @vl_serveur_pra 	char(6)
     
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
      ** test si elle a deja ete supprimee
      **/
      if @@rowcount = 0
	return ( XDC_OK )

      /*A
      ** La fiche alarme existe
      ** Recherche de la fiche alarme
      ** Affectation des parametres de sorties
      */
      
      if @vl_Alrm != null
      begin
         select @vl_Site = sit, @vl_Etat= etat from EXP..ALM_EQT
           where type 		 = @va_TypeAlrm_in
             and type_equipement = @va_TypeEqt_in
             and equipement	 = @va_Eqt_in
             
         /*A
         ** Si l'alarme est active et pas de forcage demande
         ** return ( XDC_NOK )
         */
         
      	 if @vl_Etat = XDC_VRAI and @va_ForcerSuppression_in = XDC_FAUX
            return ( XDC_ACT_REFUSEE )
         
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
               /* force l'alarme a inactive */
               if exists (select * from EXP..ALM_EQT
				where type               = @va_TypeAlrm_in
				  and type_equipement    = @va_TypeEqt_in
				  and equipement         = @va_Eqt_in
				  and etat		 = XDC_VRAI )
		begin
			select @vl_horodate=getdate()
			exec PRC..XZAM;01 @vl_horodate,@va_TypeEqt_in,@va_Eqt_in,@va_TypeAlrm_in,XDC_FAUX
		end
               
	       delete EXP..ALM_EQT
               where type 		= @va_TypeAlrm_in
                 and type_equipement 	= @va_TypeEqt_in
                 and equipement	 	= @va_Eqt_in
               
            end
            
            else
             
            /*A
            ** Le site de la fiche alarme n'est le site local
            ** Appel de la SP sur le site de la fiche alarme
            */   
            
            if @vl_Site = XDC_CI
            begin
            	if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAM;05   @va_TypeAlrm_in,
								@va_TypeEqt_in,
								@va_Eqt_in,
								@va_ForcerSuppression_in

		else
               		exec @vl_Status = SQL_CI.PRC..XZAM;05 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
								@va_ForcerSuppression_in
               	if @vl_Status != XDC_OK
                	return ( @vl_Status )
            end
               
            else
               
            if @vl_Site = XDC_VC
            begin
	    	if @vl_serveur_pra = XDC_SQLCI
			exec @vl_Status = SQL_DM.PRC..XZAM;05   @va_TypeAlrm_in,
								@va_TypeEqt_in,
								@va_Eqt_in,
								@va_ForcerSuppression_in
		else            
               		exec @vl_Status = SQL_DM.PRC..XZAM;05 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
								@va_ForcerSuppression_in
               	if @vl_Status != XDC_OK
			return ( @vl_Status )
            end
               
            else
            if @vl_Site = XDC_DP
            begin
            	if @vl_serveur_pra = XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAM;05   @va_TypeAlrm_in,
								@va_TypeEqt_in,
								@va_Eqt_in,
								@va_ForcerSuppression_in
		else
			exec @vl_Status = SQL_DS.PRC..XZAM;05 	@va_TypeAlrm_in,
               							@va_TypeEqt_in,
               							@va_Eqt_in,
								@va_ForcerSuppression_in								
		if @vl_Status != XDC_OK
			return ( @vl_Status )
            end
               
            else
            if @vl_Site = XDC_CA
            begin
            
               exec @vl_Status = SQL_CA.PRC..XZAM;05 	@va_TypeAlrm_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
							@va_ForcerSuppression_in
               
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
