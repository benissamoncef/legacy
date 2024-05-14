/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	24/03/11	creation
* LCL	28/03/12	Gestion evts communautaires (DEM/1015)
* PNI	09/09/16	Mise en place d'un begin tran v1.4 DEM1179
*  JMG           03/03/17 : regio DEM1220
* LCL	29/06/17	PRA
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* JPL	22/07/23 : Pas d'alerte inhibee de DAI inhibee selon conditions (feux tricolores) (SAE-440)
* JPL	29/08/23 : Filtrage des alertes provenant des Analyseurs inhibes et non DAI (SAE-440)
* JPL	29/08/23 : Prise en compte de la voie configuree pour inhibition (SAE-440)
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

if exists (select * from sysobjects where name = 'XZAA11' and type = 'P')
	drop procedure XZAA11
go


create procedure XZAA11
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_NumEqt_in	smallint = null,
   @va_Type_in		tinyint = null,
   @va_Texte_in		char(100),
   @va_Autoroute_in     char(10),
   @va_PR_in            int,
   @va_Sens_in          tinyint,
   @va_NomSiteLocal_in  char(2) = null

   
   as 

/* 
*
* SP	XZAA01SP
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
   @vl_Portion             smallint,
   @vl_Translation         int,
   @vl_texte		char(100),
   @vl_index		tinyint,
   @vl_nb		int,
   @vl_pos_voie		int,
   @vl_chn_voie		char(1),
   @vl_num_voie		tinyint,
   @vl_Controleur_LB	smallint

       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
   or @va_TypeEqt_in = null
   or @va_NumEqt_in = null
   or @va_Type_in = null
      
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
      
      select @vl_Eqt = numero from CFG..EQT_GEN
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
        /* Determiner le site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Serveur = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Serveur = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

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


        /*recup numero autoroute*/
        select @vl_Autoroute=numero from CFG..RES_AUT
        where nom = @va_Autoroute_in

        if @@rowcount=0
        begin
                return XDC_ARG_INV
        end

	if @va_Type_in=XZAAC_ALERTE_COMMUNAUTAIRE
	begin
		select @vl_index=patindex("%#%",@va_Texte_in)
		select @vl_texte = substring(@va_Texte_in,1,@vl_index-1) + '#%'
		select @vl_nb = count(*) from EXP..ALT_EXP
		where type = @va_Type_in
		and equipement = @va_NumEqt_in
		and qualification = 0
		and type_equipement = @va_TypeEqt_in
		and texte like @vl_texte

		if @vl_nb > 0
		begin
			return XDC_OK
		end
	end


         begin tran
         select @vl_Numero = max ( numero ) from EXP..ALT_EXP where sit=@vl_Serveur
         
         if @vl_Numero = null
            select @vl_Numero = 0
         
         select @vl_Numero = @vl_Numero + 1

         insert into EXP..ALT_EXP
         values ( @vl_Numero, @va_TypeEqt_in, @va_NumEqt_in, @va_Type_in, @va_Horodate_in, @va_Texte_in, XDC_FAUX, null, @vl_Serveur, null )
         commit tran

         insert into EXP..ALT_LOC (numero, sit, autoroute, PR, sens)
         values (@vl_Numero,  @vl_Serveur, @vl_Autoroute, @va_PR_in, @va_Sens_in)


         select @vl_PR          = @va_PR_in,
                @vl_Sens        = @va_Sens_in
         
         select @vl_NomEqt	= nom
         from CFG..EQT_GEN
         where type   = @va_TypeEqt_in
           and numero = @va_NumEqt_in
         
         select  @vl_Portion=numero,
            @vl_Translation=10000*(@vl_PR-PR_debut)/(PR_fin-PR_debut)
                        from CFG..RES_POR
                        where   autoroute=@vl_Autoroute and
                                PR_debut<@va_PR_in and
                                PR_fin>=@va_PR_in

         select @vl_Creation = XDC_VRAI
         
            /* PRA
	    if @@servername = XDC_SQLVC
            */
	    if @vl_Serveur = XDC_VC
	    
               exec TASRV_VC...AARP11 	@vl_Numero, 
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
            				@vl_Serveur,
                                       @vl_Portion,
                                        @vl_Translation

            else
               /* PRA
	       if @@servername = XDC_SQLDP
               */
               if @vl_Serveur = XDC_DP
		begin
		select " TASRV_DP...AARP11"
                  exec TASRV_DP...AARP11 	@vl_Numero, 
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
						@vl_Serveur,
                                        @vl_Portion,
                                        @vl_Translation
		end
                  
            else
                  return ( XDC_NOK )   
      end
       
   end
     
   return ( XDC_OK )
         
go  
