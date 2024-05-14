/*E*/
/*Fichier : $Id: xzaa05sp.prc,v 1.12 2020/11/03 15:50:26 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 15:50:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* B.G.  28/12/94	: modif (1.2)
* B.G.  05/01/95	: modif xdc_faux pour qualif(1.3)
* C.T.  29/05/95	: remplace xdc_faux pour qualif par 
*                         XDC_ALT_INCONNUE  et ajout controle
*                         pour alerte liee a un evt (1.3)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.5)
* C.T.  07/01/98	: Ajout des sites suppl (1532) (1.6)
* L.C.  09/07/07        : Suppression DY
* PNI	12/12/07	: RAU au CI 1.8 DEM716
* JMG           02/10/09        : secto DEM887 1.9
*  JMG           03/03/17 : regio DEM1220
* LCL		29/06/17 : modif PRA
* LCL		18/11/19 : MOVIS DEM-SAE93 1.12
* ABK	26/09/23 : Ajout district CA (DEM-483)
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Remet une alerte archivee dans la liste des alertes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;05
   @va_NumAlerte_in		int = null,
   @va_NomSiteLocal_in		char(2)
   as 

/* 
*
* SP	XZAA05SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Alerte		va_NumAlerte_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Remet une alerte archivee dans la liste des alertes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumAlerte inconnu
*
* CONDITIONS D'UTILISATION :
*
* Uniquement pour une alerte vraie avec creation d'evenement qui n'a pas aboutie
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
    
   @vl_Qualification	tinyint,
   @vl_Horodate 	datetime,
   @vl_NumEqt 		smallint,
   @vl_Type		tinyint,
   @vl_TypeEqt		tinyint,
   @vl_NomType		char(25),
   @vl_Autoroute	tinyint,
   @vl_PR		int,
   @vl_Sens		tinyint,
   @vl_Creation		tinyint,
   @vl_NomEqt		char(10),
   @vl_site		tinyint,
   @vl_texte		char(100)
       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumAlerte_in = null
   or @va_NomSiteLocal_in = null
      
      return ( XDC_ARG_INV )
   
   /*A
   ** Si les parametres sont non nuls
   */
      /* recherche le site local */
        select @vl_site = numero 
        from CFG..RES_DIS
        where code = @va_NomSiteLocal_in
/*        where serveur = @@servername
*/

        if @vl_site is null
                return XDC_NOK
      
      
      select @vl_Creation = XDC_VRAI
      
      select @vl_Qualification = qualification from EXP..ALT_EXP
      where numero = @va_NumAlerte_in and sit=@vl_site
          
      /*A
      ** L'alerte existe et n'est pas liee a un evenement
      */
      
      if @vl_Qualification != null and @vl_Qualification != XDC_ALT_VRAIE_EVT
      begin
         
         update EXP..ALT_EXP
         set qualification = XDC_ALT_INCONNUE
         where numero = @va_NumAlerte_in and sit=@vl_site
      
         
         select @vl_Horodate	= ALT.horodate,
         	@vl_Type	= ALT.type,
         	@vl_NomType	= TYP.nom,
         	@vl_TypeEqt	= ALT.type_equipement,
         	@vl_NumEqt	= ALT.equipement,
         	@vl_NomEqt	= EQT.nom,
         	@vl_Autoroute 	= EQT.autoroute,
         	@vl_PR		= EQT.PR,
         	@vl_Sens	= EQT.sens,
         	@vl_texte	= ALT.texte
        
         from CFG..EQT_GEN EQT, EXP..ALT_EXP ALT, CFG..TYP_ALT TYP
        
         where EQT.type   = ALT.type_equipement
           and EQT.numero = ALT.equipement
           and ALT.type   = TYP.numero
           and ALT.numero = @va_NumAlerte_in
           and ALT.sit    = @vl_site
         
         
            /* PRA
	    if @@servername = XDC_SQLVC
	    */
	    if @vl_site = XDC_VC
            
            exec TASRV_VC...AARP01 	@va_NumAlerte_in, 
            				@vl_Horodate, 
            				@vl_Type, 
            				@vl_NomType, 
            				@vl_TypeEqt, 
            				@vl_NumEqt, 
            				@vl_NomEqt,
            				@vl_texte, 
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
					@vl_site
               
            else
	    /* PRA
            if @@servername = XDC_SQLDP
	    */
	    if @vl_site = XDC_DP
            
            exec TASRV_DP...AARP01 		@va_NumAlerte_in, 
            					@vl_Horodate, 
            					@vl_Type, 
            					@vl_NomType, 
            					@vl_TypeEqt, 
            					@vl_NumEqt, 
            					@vl_NomEqt, 
            					@vl_texte,
            					@vl_Autoroute, 
            					@vl_PR, 
            					@vl_Sens, 
            					@vl_Creation,
						@vl_site
                  
            else
            /* PRA
	    if @@servername = XDC_SQLCI
            */
	    if @vl_site = XDC_CI

            exec TASRV_CI...AARP01 		@va_NumAlerte_in, 
            					@vl_Horodate, 
            					@vl_Type, 
            					@vl_NomType, 
            					@vl_TypeEqt, 
            					@vl_NumEqt, 
            					@vl_NomEqt, 
            					@vl_texte,
            					@vl_Autoroute, 
            					@vl_PR, 
            					@vl_Sens, 
            					@vl_Creation,
						@vl_site

                              else
            /* PRA
	    if @@servername = XDC_SQLCA
            */
	    if @vl_site = XDC_CA

            exec TASRV_CA...AARP01 		@va_NumAlerte_in, 
            					@vl_Horodate, 
            					@vl_Type, 
            					@vl_NomType, 
            					@vl_TypeEqt, 
            					@vl_NumEqt, 
            					@vl_NomEqt, 
            					@vl_texte,
            					@vl_Autoroute, 
            					@vl_PR, 
            					@vl_Sens, 
            					@vl_Creation,
						@vl_site
                  
                  
            else
                  
            return ( XDC_NOK )   
         
      end     
      
      else
      
      /*A
      ** L'alerte n'existe pas ou elle est liee a un evenement 
      ** return ( XDC_NOK ) 
      */
         
         return ( XDC_NOK ) 
     
   return ( XDC_OK )
         
go  

