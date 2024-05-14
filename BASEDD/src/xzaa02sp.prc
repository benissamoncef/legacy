/*E*/
/*Fichier : $Id: xzaa02sp.prc,v 1.14 2020/11/03 15:49:54 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 15:49:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* EXPTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* gaborit	29 dec 1994	: modif integration
* Torregrossa	27 mar 1995	: modif qualification de l'alerte
* Torregrossa	01 Fev 1996	: modif qualification d'une alerte en
*                                 etat a confirmer si elle date de plus
*                                 d'une heure (1.4)
* C.P.		07/01/98	: Correction gestion multi-sites dans 
*				  les procs (1.5)
* C.T.  	07/01/98	: Ajout des sites suppl (1532) (1.6)
* P.N.		24/11/1998	: retourne XDC_OK si CI 1.7 dem/1724
* L.C.          09/07/2007      : Suppression DY 1.8
* JMG           02/10/09        : secto DEM887 1.9
* JMG		29/05/12	: typage num evt 1.10
* JMG		11/11/12	: traitement communautaire
*  JMG           03/03/17 : regio DEM1220
* LCL		29/06/17 	: modif PRA
* LCL		19/11/19	: MOVIS DEM-SAE93 1.14
* ABK	26/09/23 : Ajout district CA (DEM-483)
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Archive une fiche alerte
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;02
   @va_NumAlerte_in		int = null,
   @va_Qualification_in		tinyint = null,
   @va_NumEvt_in		int = null,
   @va_CleEvt_in		tinyint = null,
   @va_NomSiteLocal_in		char(2)
   
   as 

/* 
*
* SP	XZAA02SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Alerte		va_NumAlerte_in
* XDY_TypeEqt		va_Qualification_in
* XDY_Eqt		va_NumEqt_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Ajoute une alerte a la liste des alertes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumAlerte inconnu
* NumEvt inconnu
*
* CONDITIONS D'UTILISATION :
*
* Si l'alerte est vraie avec creation d'evenement , ne pas remplir le champ NumEvt, qui sera rempli
* automatiquement par XZAE08. Si la creation d'evenement n'a pas lieu, il est possible
* de recuperer l'alerte avec XZAA05
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
    
   @vl_site		tinyint,
   @vl_Horodate 	datetime,
   @vl_Duree 		int,
   @vl_NumEqt 		smallint,
   @vl_Type		tinyint,
   @vl_TypeEqt		tinyint,
   @vl_NomType		char(25),
   @vl_Autoroute	tinyint,
   @vl_PR		int,
   @vl_Sens		tinyint,
   @vl_Creation		tinyint,
   @vl_NomEqt		char(10),
   @vl_texte		char(100),
   @vl_statut tinyint
       
   /*A modif bug applix null */
   
   if @va_NumAlerte_in=0 select @va_NumAlerte_in=null
   if @va_Qualification_in=0 select @va_Qualification_in= null
   if @va_NumEvt_in=0 select @va_NumEvt_in= null
   if @va_CleEvt_in=0 select @va_CleEvt_in=null
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumAlerte_in = null or @va_Qualification_in = null or @va_NomSiteLocal_in = null
   	return ( XDC_ARG_INV )

   /* recherche le site local */
        select @vl_site = numero 
        from CFG..RES_DIS
        where code = @va_NomSiteLocal_in
/*        where serveur = @@servername
*/

        if @vl_site is null
                return XDC_NOK
   
  select @vl_statut = XDC_OK

   /*A si l'alerte n'existe pas NOK */      
   if not exists (select * from EXP..ALT_EXP where numero=@va_NumAlerte_in and sit=@vl_site)
   	return XDC_NOK

   /*A si l'alerte a deja ete traitee XZAAC_DEJA_TRT */
   if not exists (select * from EXP..ALT_EXP
   		where numero = @va_NumAlerte_in
   		  and (qualification=null or qualification=0)
   		  and sit=@vl_site)
   begin
	/*B tester si c'est une alerte a confirmer */
	select @vl_Horodate = horodate from EXP..ALT_EXP
   	where numero = @va_NumAlerte_in
   	  and qualification=XDC_ALT_CONF_EVT
   	  and sit=@vl_site

	if @@rowcount = 0
  		select @vl_statut = XZAAC_DEJA_TRT
	else
	begin
		select @vl_Duree = datediff(minute, @vl_Horodate, getdate())

		/*! si la qualification est a confirmer depuis moins d'heure alors */
		/*! l'alerte consideree en cours de traitement */
		if @vl_Duree < 60
			select @vl_statut = XZAAC_DEJA_TRT
	end
   end

   if @vl_statut = XDC_OK
   begin
   	if @va_Qualification_in=XDC_ALT_EXPLOITATION or @va_Qualification_in=XDC_ALT_FAUSSE
   		update EXP..ALT_EXP
   		set qualification = @va_Qualification_in,
   			evenement 	 = null,
   			cle       	 = null
   		where numero = @va_NumAlerte_in and sit=@vl_site
   
   	else
   		if @va_Qualification_in = XDC_ALT_VRAIE or @va_Qualification_in = XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT
   			update EXP..ALT_EXP
   				set qualification 	= @va_Qualification_in,
       				evenement 	= @va_NumEvt_in,
       				cle       	= @va_CleEvt_in
   			where numero = @va_NumAlerte_in and sit=@vl_site

   	else
   		if @va_Qualification_in = XDC_ALT_VRAIE_EVT
         /* Correctif MAGISTRA_BLM-1281 : update evenement/cle lors de l'association alerte/FMC */
/*   			update EXP..ALT_EXP
   			set qualification = XDC_ALT_CONF_EVT,
       				evenement 	 = null,
       				cle       	 = null
   				where numero = @va_NumAlerte_in and sit=@vl_site
*/
   			update EXP..ALT_EXP
   			set qualification = XDC_ALT_CONF_EVT,
       				evenement 	= @va_NumEvt_in,
       				cle       	= @va_CleEvt_in
   				where numero = @va_NumAlerte_in and sit=@vl_site
   
   	else 
   		return ( XDC_NOK ) 
   end

   select 	@vl_Horodate	= ALT.horodate,
         	@vl_Type	= ALT.type,
         	@vl_texte	= ALT.texte,
         	@vl_NomType	= TYP.nom,
         	@vl_TypeEqt	= ALT.type_equipement,
         	@vl_NumEqt	= ALT.equipement,
         	@vl_NomEqt	= EQT.nom,
         	@vl_Autoroute 	= EQT.autoroute,
         	@vl_PR		= EQT.PR,
         	@vl_Sens	= EQT.sens
        
   from CFG..EQT_GEN EQT, EXP..ALT_EXP ALT, CFG..TYP_ALT TYP
        
   where EQT.type   = ALT.type_equipement
     and EQT.numero = ALT.equipement 
     and TYP.numero = ALT.type
     and ALT.numero = @va_NumAlerte_in
     and ALT.sit = @vl_site
         
   select   @vl_Creation=XDC_NON
       
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
    /* PRA 
    if @@servername = XDC_SQLCI
    */
    if @vl_site = XDC_CI 
	return ( XDC_OK )
     else
     return ( XDC_NOK )   
         
     
   return ( XDC_OK )
         
go  
