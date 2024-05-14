/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa010sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 19/09/22 : Surcharge de la xzaa01, ajout du parametre va_Alerte_out en sortie Consignation bouchon DEM-SAE412
* LCL 16/03/23 : Ajout gestion des FMC sur site ASF
* JPL	22/07/23 : Pas d'alerte inhibee de DAI inhibee selon conditions (feux tricolores) (SAE-440)
* JPL	29/08/23 : Filtrage des alertes provenant des Analyseurs inhibes et non DAI (SAE-440)
* JPL	29/08/23 : Prise en compte de la voie configuree pour inhibition (SAE-440)
* ABK	26/09/23 : Ajout district CA (DEM-483)
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une alerte a la liste des alertes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAA010' and type = 'P')
	drop procedure XZAA010
go

create procedure XZAA010
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_NumEqt_in	smallint = null,
   @va_Type_in		tinyint = null,
   @va_Texte_in		char(100),
   @va_NomSiteLocal_in	char(2) = null,
   @va_Alerte_out   int output
   
   as 

/* 
*
* SP	xzaa010SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_NumEqt_in
* XDY_TypeAlerte	va_Type_in
* XDY_TexteAlerte	va_Texte_in
*
* PARAMETRES EN SORTIE :
* XDY_Entier va_Alerte_out 
*
* VALEUR RENDUE :
*
* Ajoute une alerte a la liste des alertes
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
    
   @vl_Eqt 		smallint,
   @vl_Type		tinyint,
   @vl_NomType		char(25),
   @vl_Numero		int,
   @vl_Serveur		tinyint,
   @vl_Cle		tinyint,
   @vl_Evt		int,
   @vl_Autoroute	tinyint,
   @vl_PR		int,
   @vl_Sens		tinyint,
   @vl_Creation		tinyint,
   @vl_NomEqt		char(10),
   @vl_numero_lit	smallint,
   @vl_fmc		int,
   @vl_pos_voie		int,
   @vl_chn_voie		char(1),
   @vl_num_voie		tinyint,
   @vl_Controleur_LB	smallint
       

   /* NomSiteLocal est optionnel */
   if @va_NomSiteLocal_in = null
   begin
   	if @@servername=XDC_SQLCI
	   	select @va_NomSiteLocal_in = XDC_NOM_SITE_CI
	else
		if @@servername=XDC_SQLVC 
	   		select @va_NomSiteLocal_in = XDC_NOM_SITE_VC
		else
			if @@servername=XDC_SQLDP 
	   			select @va_NomSiteLocal_in = XDC_NOM_SITE_DP
		else
			if @@servername=XDC_SQLCA 
	   			select @va_NomSiteLocal_in = XDC_NOM_SITE_CA
   end

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
   or @va_TypeEqt_in = null
   or @va_NumEqt_in = null
   or @va_Type_in = null
   or @va_NomSiteLocal_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Si le texte est null => mis a ""
      */
      
      if @va_Texte_in = null
         select @va_Texte_in = ' '
      
      select @vl_Eqt = numero ,
         	@vl_Autoroute 	= autoroute,
         	@vl_PR		= PR,
         	@vl_Sens	= sens
	from CFG..EQT_GEN
        where type   = @va_TypeEqt_in
          and numero = @va_NumEqt_in
          
      select @vl_Type = numero, @vl_NomType = nom from CFG..TYP_ALT
        where numero = @va_Type_in
          
      /*A
      ** L'equipement et le type d'alerte existent 
      */
      
      if @vl_Eqt != null and @vl_Type != null
      begin
         
        /* recherche le site local */
        select @vl_Serveur = numero 
        from CFG..RES_DIS
        where code = @va_NomSiteLocal_in
/*        where serveur = @@servername
*/
        if @vl_Serveur is null
                return XDC_NOK


	/*
	** Si cette alerte peut etre inhibee alors verifier les conditions
	*/

	/* Si c'est une alerte provenant d'une camera DAI alors */
	if @va_TypeEqt_in = XDC_EQT_ANA
	begin
		/* extraire le numero de voie du texte de l'alerte */
		select @vl_pos_voie = charindex (" VOIE ", upper (@va_Texte_in))
		select @vl_chn_voie = substring (@va_Texte_in, @vl_pos_voie + 6, 1)
		select @vl_num_voie = ascii (@vl_chn_voie) - ascii ('0')
		--print "Index voie=%1!, Numero voie=%2!", @vl_pos_voie, @vl_num_voie

		/* determiner si l'alerte est inhibee sous condition (sans prendre en compte la voie) */
		if ( --(@vl_pos_voie > 0)  and  (@vl_num_voie between 0 and 9)  and
		     exists (select 1 from CFG..ALT_INH where type_alerte = @va_Type_in
	                                                  and type_eqt = @va_TypeEqt_in and numero_eqt = @va_NumEqt_in
	                                                  --and voie = @vl_num_voie
		            )
		   )
		begin
			/* si le controleur de feux concerne commande un Plan de feux non clignotants, ignorer l'alerte */
			select @vl_Controleur_LB = numero from CFG..EQT_GEN
			          where type = XDC_EQT_CFE  and  nom like "%LB%"
			if exists (select E.etat from EXP..ETA_EQT E, CFG..DEF_PDF D
			              where E.type = XDC_EQT_CFE  and  E.numero = @vl_Controleur_LB
			                and E.heure_fin is null
			                and E.numero = D.numero  and  E.etat = D.plan_de_feux
			                and upper (rtrim (D.libelle)) not like "%CLIGNO%")
				return ( XDC_OK )
		end
	end


	 /*GAPLA*/
	 /*si eqt PAU et alerte  GAPLA*/
	 if @va_TypeEqt_in=XDC_EQT_PAU and
	 	@va_Type_in = XZAAC_LIT
	begin
	 /*rechercher lit arret associe*/
	  select @vl_numero_lit = numero from CFG..EQT_GEN
	  	where type = XDC_EQT_LIT 
		and autoroute = @vl_Autoroute
		and PR = @vl_PR
		and sens = @vl_Sens

	  if @@rowcount = 0
	  	return XDC_NOK

	 if exists (select * from EXP..ALT_EXP
	 	where type_equipement = XDC_EQT_LIT
		and equipement = @vl_numero_lit
		and sit = @vl_Serveur
		and (qualification=null or qualification=0))
		return XDC_OK

	 	/* regarder is pas de FMC en cours sur lit arret*/
		select @vl_fmc = 0
		select @vl_fmc = count(*)
		from EXP..FMC_GEN g, EXP..FMC_HIS_DER d
		where g.numero = d.numero
		and g.cle = d.cle
		and d.autoroute = @vl_Autoroute
		and d.PR = @vl_PR
		and d.sens = @vl_Sens
		and g.debut is not null 
		and g.fin is null

		if @vl_fmc > 0
		begin
			/*fMC en cours sur le lit d arret*/
			/*pas de creation d alerte*/
			return XDC_OK
		end

		
	 	/* alors creer l alerte*/
		select @va_TypeEqt_in = XDC_EQT_LIT
		select @va_NumEqt_in =  @vl_numero_lit
	end

	 /* alors creer l alerte*/

         begin tran
         select @vl_Numero = max ( numero ) from EXP..ALT_EXP where sit=@vl_Serveur
         
         if @vl_Numero = null
            select @vl_Numero = 0
         
         select @vl_Numero = @vl_Numero + 1
         select @va_Alerte_out = @vl_Numero
         
         insert into EXP..ALT_EXP
         values ( @vl_Numero, @va_TypeEqt_in, @va_NumEqt_in, @va_Type_in, @va_Horodate_in, @va_Texte_in, XDC_FAUX, null, @vl_Serveur, null )
         commit tran
         
         
         select @vl_NomEqt	= nom,
         	@vl_Autoroute 	= autoroute,
         	@vl_PR		= PR,
         	@vl_Sens	= sens
        
         from CFG..EQT_GEN
        
         where type   = @va_TypeEqt_in
           and numero = @va_NumEqt_in
         
         
         select @vl_Creation = XDC_VRAI
        
	    /* Modif PRA 
            if @@servername = XDC_SQLVC
	    */
            if @vl_Serveur = XDC_VC
	    
               exec TASRV_VC...AARP01 	@vl_Numero, 
            				@va_Horodate_in, 
            				@va_Type_in, 
            				@vl_NomType, 
            				@va_TypeEqt_in, 
            				@va_NumEqt_in, 
            				@vl_NomEqt, 
            				@va_Texte_in,
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
            				@vl_Serveur
               
            else
               /*
	       if @@servername = XDC_SQLDP
	       */
	       if @vl_Serveur = XDC_DP or @vl_Serveur = XDC_ASF
            
                  exec TASRV_DP...AARP01 	@vl_Numero, 
            					@va_Horodate_in, 
            					@va_Type_in, 
            					@vl_NomType, 
            					@va_TypeEqt_in, 
            					@va_NumEqt_in, 
            					@vl_NomEqt, 
            					@va_Texte_in,
            					@vl_Autoroute, 
            					@vl_PR, 
            					@vl_Sens, 
            					@vl_Creation,
						@vl_Serveur
             else 
               
                  return ( XDC_NOK )   
      end
       
   end
     
   return ( XDC_OK )
         
go  
