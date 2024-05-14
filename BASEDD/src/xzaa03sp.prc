/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	01/12/97	creation
* C.T.  07/01/98	: Ajout des sites suppl (1532) (1.4)
* claudel       09/07/07 : Suppression DY (1.5)
* JMG           02/10/09        : secto DEM887 1.6
* PNI	09/09/16	Mise en place d'un begin tran v1.5 DEM1179
*  JMG           03/03/17 : regio DEM1220
* LCL		29/06/17	: modif PRA
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
* suivant le site
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;03
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_NumEqt_in	smallint = null,
   @va_Type_in		tinyint = null,
   @va_site_in		smallint = null,
   @va_Texte_in		char(100),
   @va_reference_in	char(50)

   
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
   @vl_numtmp		int,
   @vl_cletmp		tinyint,
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
         
         if @va_site_in = XDC_CI
            
            select @vl_Serveur = XDC_CI
            
         else
            if @va_site_in= XDC_VC
            
               select @vl_Serveur = XDC_VC
               
         else
            if @va_site_in = XDC_DP
            
               select @vl_Serveur = XDC_DP

         else
            if @va_site_in = XDC_CA
            
               select @vl_Serveur = XDC_CA
                  
         else 
               
                  return ( XDC_NOK )   


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


         begin tran
         select @vl_Numero = max ( numero ) from EXP..ALT_EXP where sit=@vl_Serveur
         
         if @vl_Numero = null
         
            select @vl_Numero = 0
         
         select @vl_Numero = @vl_Numero + 1
         
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
         
            if @va_site_in = XDC_VC
            
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
	       /* PRA
               if @@servername = XDC_SQLDP
	       */
	       if @va_site_in = XDC_DP
            
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
	       if @va_site_in = XDC_CA
            
                  exec TASRV_CA...AARP01 	@vl_Numero, 
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
       

	/*archive la situation Strada en base*/
	select @vl_numtmp=null
	select @vl_cletmp=null
	exec XZAE145	@vl_numtmp,@vl_cletmp,@vl_Numero,@va_site_in,@va_reference_in

	update EXP..ADA_SYN
	set transmis = @va_site_in 
	where reference = @va_reference_in
     
   return ( XDC_OK )
         
go  
