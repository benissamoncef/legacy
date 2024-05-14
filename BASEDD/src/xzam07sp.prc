/*E*/
/*Fichier : $Id: xzam07sp.prc,v 1.9 2021/03/08 13:46:12 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2021/03/08 13:46:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04 Nov 1994	: Creation
* torregrossa	05 sep 1996	: Ajout test sur DS et DC V1.2
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.3) (dem / 1536)
* LCL           09 Jul 2007     : Suppression DY
* JMG           02/10/09        : secto DEM887 + DEM914
* PNI		05/02/15	:@va_Type_in passe en smallint DEM1108 v1.6
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.7
* ABE	06/01/21	: MOVIS ajout PRA DEM-SAE93 1.8								
* LCL	27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Cree une nouvelle fiche alarme, ou met a jour celle existant deja
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;07
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_Eqt_in		smallint = null,
   @va_Type_in		smallint = null,
   @va_Etat_in		bit
   as 

/* 
*
* SP	XZAM07SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_TypeAlarme	va_Type_in
* XDY_Booleen		va_Etat_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Cree une nouvelle fiche alarme, ou met a jour celle existant deja (Alarme Manuelle)
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
* recherche dans la table ALARME s'il existe deja une fiche alarme pour le meme
* equipement et le meme type d'alarme.
*
* Si c'est le cas, met a jour cette fiche (etat, horodate dernier changement, horodate
* premiere disparition, nombre dapparitions), sinon, cree une nouvelle fiche
*
* Met a jour l'indisponibilite de l'equipement en fonction du type d'alarme (majeure,
* mineure, critique) et de la desactivation eventuelle + appel ATRP18 (XZIS02)
*
* Incremente le compteur globlal d'alarmes de l'equipement
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Site 		tinyint, 
   @vl_Status	 	int,
   @vl_serveur_pra 	char(6)
   
     
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
   or @va_TypeEqt_in = null
   or @va_Eqt_in = null
   or @va_Type_in = null
   or @va_Etat_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   begin
   
   /*A
   ** Si les parametres sont non nuls
   */
         
      /*A
      ** Le type de l'alarme correspond a un type manuel
      */
      
      if @va_Type_in = XZAMC_MANUELLE_MINEURE
      or @va_Type_in = XZAMC_MANUELLE_MAJEURE
      or @va_Type_in = XZAMC_MANUELLE_CRITIQUE  
      
      begin
      
         /*A
         ** Recherche du site de l'equipement
         */
          
         select @vl_Site = sit from EXP..EQT_DSP
           where type 		= @va_TypeEqt_in
             and equipement	= @va_Eqt_in
            
         
         /*A
         ** Appel de XZAM;07 sur le site correspondant
         */

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@vl_Site

         if @vl_Site = XDC_CI
         begin
            
		/* Redirection de l'action sur le serveur de pra */
               if  @vl_serveur_pra=XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAM;01 	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
	      else
			exec @vl_Status = SQL_CI.PRC..XZAM;01 	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            end
               
            else
               
            if @vl_Site = XDC_VC
            begin
		if  @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAM;01 	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
		else
			exec @vl_Status = SQL_DM.PRC..XZAM;01	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
               
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
               
            else
            if @vl_Site = XDC_DP
            begin
    		if  @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAM;01 	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
		else
               		exec @vl_Status = SQL_DS.PRC..XZAM;01	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
               
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
            
            else
            if @vl_Site = XDC_CA
            begin
            
               exec @vl_Status = SQL_CA.PRC..XZAM;01	@va_Horodate_in,
               						@va_TypeEqt_in,
               						@va_Eqt_in,
            						@va_Type_in,
            						@va_Etat_in
               
               
               if @vl_Status != XDC_OK
               
                  return ( @vl_Status )
            
            end
            else
            
               return ( XDC_NOK )
            
      end
      
      /*A
      ** Le type de l'alarme ne correspond a un type manuel
      */
      
      else
      
         return ( XDC_NOK )
      
      
   end
      
   return ( XDC_OK )
         
go  
