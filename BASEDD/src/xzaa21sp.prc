/*E*/
/*Fichier : $Id: xzaa21sp.prc,v 1.8 2020/11/03 16:09:22 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2020/11/03 16:09:22 $
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
* JMG	24/03/11 crration 1.0
* JMG	02/07/11 correction acquittement pour maj des postes voisins
* JMG	11/11/12 communautaire
*  JMG           03/03/17 : regio DEM1220
* LCL 	29/06/17 modif PRA
* LCL	19/01/18 correction bug suite modif PRA
* LCL	18/11/19	: MOVIS DEM-SAE93 1.6
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

drop procedure XZAA21
go

create procedure XZAA21
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
   @vl_texte		varchar(100),
   @vl_texte2           varchar(100),
   @vl_portion		smallint,
   @vl_translation	int,
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
			select @vl_statut =  XZAAC_DEJA_TRT
	end
   end

   select @vl_Type        = ALT.type,
           @vl_texte       = rtrim(ALT.texte)
   from EXP..ALT_EXP ALT
   where numero = @va_NumAlerte_in and sit=@vl_site

   /*si alerte PAU VIRTUEL*/
   if @vl_Type = XZAAC_APPEL_IPHONE
   begin
     /*on vire les 4 derniers chiffres du numero de telephone*/
     select @vl_texte2 = substring(@vl_texte,1,char_length(@vl_texte)-4)+'****'      
     /*select @vl_texte2 = "NUMERO EFFACE"*/
     update EXP..ALT_EXP set texte=@vl_texte2 where numero = @va_NumAlerte_in and sit=@vl_site
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
   if @va_Qualification_in = XDC_ALT_VRAIE
   update EXP..ALT_EXP
   set qualification 	= @va_Qualification_in,
       evenement 	= @va_NumEvt_in,
       cle       	= @va_CleEvt_in
   where numero = @va_NumAlerte_in and sit=@vl_site
      
   else
   if @va_Qualification_in = XDC_ALT_VRAIE_EVT
   update EXP..ALT_EXP
   set qualification = XDC_ALT_CONF_EVT,
       evenement 	 = null,
       cle       	 = null
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
       
   select @vl_portion=0,@vl_translation=0

   select @vl_Autoroute=autoroute,@vl_PR=PR
   from EXP..ALT_LOC
   where numero = @va_NumAlerte_in
   and sit = @vl_site

   select  @vl_portion=numero,
       @vl_translation=10000*(@vl_PR-PR_debut)/(PR_fin-PR_debut)
       from CFG..RES_POR
       where   autoroute=@vl_Autoroute and
       PR_debut<@vl_PR and
	       PR_fin>=@vl_PR

   /* PRA
   if @@servername = XDC_SQLVC
   */
   if @vl_site = XDC_VC

   exec TASRV_VC...AARP11 	@va_NumAlerte_in, 
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
				@vl_site,
				@vl_portion,@vl_translation
               
    else
    /* PRA
    if @@servername = XDC_SQLDP
    */
    if @vl_site = XDC_DP

    exec TASRV_DP...AARP11 		@va_NumAlerte_in, 
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
					@vl_site,
				@vl_portion,@vl_translation

    else
    if @vl_site = XDC_CA

    exec TASRV_CA...AARP11 		@va_NumAlerte_in, 
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
					@vl_site,
				@vl_portion,@vl_translation

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
