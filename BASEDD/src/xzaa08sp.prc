/*E*/
/*Fichier : $Id: xzaa08sp.prc,v 1.12 2020/11/03 15:50:26 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 15:50:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		27/07/21 : MAGISTRA DEM-SAE311 1.1
* ABE       31/05/2022 : Modif vl_creation force a 2 pour catch dans aarp01 et prendre l'horodate de l'alerte et non la courrante DEM-SAE311 1.2
* ABK	26/09/23 : Ajout district CA (DEM-483)
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Remet une alerte archivee dans la liste des alertes 
*  basee sur xzaa;05 mais travaille sur n'importe quelle qualification
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAA08' and type = 'P')
	drop procedure XZAA08
go

create procedure XZAA08
   @va_NumAlerte_in		int = null,
   @va_NomSiteLocal_in		char(2)
   as 

/* 
*
* SP	XZAA08SP
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
* Pour toutes les alertes contrairement à XZAA;05
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
   @vl_texte		char(100),
   @vl_NumEvt		int,
   @vl_CleEvt		tinyint

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

    if @vl_site is null
            return XDC_NOK
      
    /* on force vl_creation a 2 pour recupere cette valeur dans l'arp01 pour forcer a prendre l'horodate de l'alerte et non l'horodate courante */
    select @vl_Creation = 2 
    
    select @vl_Qualification = qualification, @vl_NumEvt = evenement, @vl_CleEvt = cle from EXP..ALT_EXP
    where numero = @va_NumAlerte_in and sit=@vl_site
        
    /*A
    ** L'alerte existe et peut etre liee a un evenement
    */
    
    if @vl_Qualification != null 
    begin
        
        update EXP..FMC_GEN
        set alerte=0, sit_alerte=0
        where numero = @vl_NumEvt 
          and cle = @vl_CleEvt
          and alerte = @va_NumAlerte_in

        update EXP..ALT_EXP
        set qualification = XDC_ALT_INCONNUE,
            evenement = NULL,
            cle = NULL
        where numero = @va_NumAlerte_in 
          and sit = @vl_site
    
        
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
    ** L'alerte n'existe pas
    ** return ( XDC_NOK ) 
    */
        
        return ( XDC_NOK ) 
    
return ( XDC_OK )
         
go  

