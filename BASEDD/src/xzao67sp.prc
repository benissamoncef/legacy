/*E*/
/* Fichier : $Id: xzao67sp.prc,v 1.12 2019/01/24 12:47:46 devgfi Exp $        $Revision: 1.12 $        $Date: 2019/01/24 12:47:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao67sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	16 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* cluchague 21 Avr 2005	: correction du type de maitre dans CFT (REPERE1) v1.5
* JMG  		13/11/07	: ajout site de gestion DEM715  1.6
* LCL		05/07/17	: Modif PRA  1.7
* LCL       11/07/18	: correction syntaxe from  1.8
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.9
* JMG		12/11/18	: ajout IP 1.10
* JPL		24/01/19	: Ajout du parametre 'supprime' (DEM 1305)  1.12
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao67sp.h" */

/* static char *version = "@(#)xzao67.c	1.1 23/18/94 : xzao67" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une station RADT dans la table EQT_RAD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO67
go

create procedure XZAO67
   @va_Basedd_in        	char(3),
   @va_NomStation_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_NomServeur_in		char(10),
   @va_Liaison_in		tinyint,
   @va_RGS_in			char(3),
   @va_Seuils_in		char(10),
   @va_Alertes_in		char(10),
   @va_DonneesIndividuelles_in	bit,
   @va_NomSite_in		char(2),
   @va_type_in			tinyint,
   @va_adresseIP_in		char(20),
   @va_port_in			int,
   @va_idLCR_in			char(20),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO67SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomStation_in
* XDY_Nom	va_Autoroute_in	
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_NomEqt	va_NomServeur_in
* XDY_Octet	va_Liaison_in
* XDY_Rgs	va_RGS_in
* XDY_NomFich	va_Seuils_in
* XDY_NomFich	va_Alertes_in
* XDY_Booleen	va_DonneesIndividuelles_in
* XDY_NomSite	va_NomSite_in
* XDY_Octet	va_type_in
* XDY_AdresseIP	va_adresseIP_in
* XDY_Entier	va_port_in
* char(20)	va_idLCR_in
* XDY_Booleen	va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une station de comptage en base
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
* Insertion ou modification dans la table EQT_RAD (STATION RADT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

	@vl_Autoroute	tinyint,
	@vl_Maitre		smallint,
	@vl_NumEqt		smallint,
	@vl_NumStation	smallint,
	@vl_Numero		smallint,
	@vl_Site		tinyint,
    @vl_SiteNom		char(10)
  
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomStation_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_NomServeur_in = null
   or @va_Liaison_in = null 
   or @va_RGS_in = null 
   or @va_Seuils_in = null 
   or @va_Alertes_in = null
   or @va_DonneesIndividuelles_in = null 
   or @va_NomSite_in= null   
      return XDC_ARG_INV
   
   else   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_SiteNom = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_SiteNom=XDC_SQLCI or @vl_SiteNom=XDC_SQLCA
		begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin
         
         select @vl_Autoroute = numero from CFG..RES_AUT
            where nom = @va_Autoroute_in
          
         select @vl_Site = numero from CFG..RES_DIS
    where code = @va_NomSite_in

	if @vl_Site = NULL
	return XDC_ARG_INV
         
         /*A
         ** L'autoroute existe
         */
         
         if @vl_Autoroute <> null
         begin
         
            select @vl_Maitre = numero from CFG..EQT_GEN
               where nom  = @va_NomServeur_in
                 and type = XDC_EQT_SRV
         
            /*A
            ** Le serveur existe comme equipement
            */
         
            if @vl_Maitre <> null
            begin
            
               select @vl_NumEqt = numero from CFG..EQT_GEN
                  where nom         = @va_NomStation_in
                    and type        = XDC_EQT_RAD
           
               /*A
               ** La station RADT existe comme equipement
               */
         
               if @vl_NumEqt <> NULL
               begin
            
                  /*A
                  ** Modification dans la table EQT_GEN de la station
                  */
    
    	          update CFG..EQT_GEN
    	            set PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        autoroute   = @vl_Autoroute,
                        maitre      = @vl_Maitre,
                        type_maitre = XDC_MAI_RAD,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                  where numero      = @vl_NumEqt 
                    and nom         = @va_NomStation_in
                    and type        = XDC_EQT_RAD
           
                  /*A
                  ** Si Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** La station RADT existe dans la table EQT_RAD
                  ** Modification dans la table de la station
                  */
    
    	          update CFG..EQT_RAD
    	          set rgs                   = @va_RGS_in,
                      liaison_serie         = @va_Liaison_in,
                      donnees_individuelles = @va_DonneesIndividuelles_in,
                      seuils                = @va_Seuils_in,
                      alertes               = @va_Alertes_in,
		      type		= @va_type_in,
		      adresse_ip 	= @va_adresseIP_in,
		      port = @va_port_in,
		      id_lcr =    @va_idLCR_in
                  where numero = @vl_NumEqt   
    
                  /*A
                  ** Si Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                     select @va_Resultat_out = XDC_MODIF
               
               end
                  
               else
               
               /*A
               ** La station RADT n'existe pas comme equipement
               */
         
               begin   
                  
                  /*A
                  ** Insertion dans la table EQT_GEN de la station RADT
                  */
    
    	          select @vl_Numero = max(numero) from CFG..EQT_GEN
    	            where type = XDC_EQT_RAD
    	            
    	          if @vl_Numero = null
    	             select @vl_Numero = 0
    	             
                  insert into CFG..EQT_GEN (
                                type,
                                numero,
                                maitre,
                                type_maitre,
                                autoroute,
                                PR,
                                sens,
                                nom,
                                site_gestion,
                                eqt_supprime
                              )
                  values ( XDC_EQT_RAD, @vl_Numero + 1, @vl_Maitre, XDC_MAI_RAD, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomStation_in , @vl_Site, @va_Supprime_in)
                  
                  /*A
                  ** Si Erreur d'insertion
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** La station RADT n'existe pas dans la table EQT_RAD
                  ** Insertion dans la table de la station
                  */
            
                  insert into CFG..EQT_RAD
                  values ( @vl_Numero + 1, @va_RGS_in, @va_Liaison_in, @va_DonneesIndividuelles_in, @va_Seuils_in, @va_Alertes_in,
		  		@va_type_in, @va_adresseIP_in, @va_port_in, @va_idLCR_in) 
                  /*A
                  ** Si Erreur d'insertion
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
               end
            
            end
         
            else
         
            /*A
            ** Le serveur n'existe pas comme equipement
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         end
      
         else      
      
         /*A
         ** L'autoroute n'existe pas comme equipement
         **
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         
            select @va_Resultat_out = XDC_ECHEC
            
      end
      
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin
         
         select @vl_Autoroute = numero from CFT..RES_AUT
            where nom = @va_Autoroute_in
          
           select @vl_Site = numero from CFT..RES_DIS
	    where code = @va_NomSite_in

	if @vl_Site = NULL
	return XDC_ARG_INV
         
         /*A
         ** L'autoroute existe
         */
         
         if @vl_Autoroute <> null
         begin
         
            select @vl_Maitre = numero from CFT..EQT_GEN
               where nom  = @va_NomServeur_in
                 and type = XDC_EQT_SRV
         
            /*A
            ** Le serveur existe comme equipement
            */
         
            if @vl_Maitre <> null
            begin
            
               select @vl_NumEqt = numero from CFT..EQT_GEN
                  where nom         = @va_NomStation_in
                    and type        = XDC_EQT_RAD
           
               /*A
               ** La station RADT existe comme equipement
               */
         
               if @vl_NumEqt <> NULL
               begin
            
                  /*A
                  ** Modification dans la table EQT_GEN de la station
                  */
    
    	          update CFT..EQT_GEN
    	            set PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        autoroute   = @vl_Autoroute,
                        maitre      = @vl_Maitre,
/* REPERE1                       type_maitre = XDC_MAI_SRV*/
                        type_maitre = XDC_MAI_RAD,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                  where numero      = @vl_NumEqt 
                    and nom         = @va_NomStation_in
                    and type        = XDC_EQT_RAD
           
                  /*A
                  ** Si Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** La station RADT existe dans la table EQT_RAD
                  ** Modification dans la table de la station
                  */
    
    	          update CFT..EQT_RAD
    	          set rgs                   = @va_RGS_in,
                      liaison_serie         = @va_Liaison_in,
                      donnees_individuelles = @va_DonneesIndividuelles_in,
                      seuils                = @va_Seuils_in,
                      alertes               = @va_Alertes_in,
		      type		= @va_type_in,
		      adresse_ip 	= @va_adresseIP_in,
		      port = @va_port_in,
		      id_lcr =    @va_idLCR_in
                  where numero = @vl_NumEqt   
    
                  /*A
                  ** Si Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                     select @va_Resultat_out = XDC_MODIF
               
               end
                  
               else
               
               /*A
               ** La station RADT n'existe pas comme equipement
               */
         
               begin   
                  
                  /*A
                  ** Insertion dans la table EQT_GEN de la station RADT
                  */
    
    	          select @vl_Numero = max(numero) from CFT..EQT_GEN
    	            where type = XDC_EQT_RAD
    	            
    	          if @vl_Numero = null
    	             select @vl_Numero = 0
    	             
                  insert into CFT..EQT_GEN (
                                type,
                                numero,
                                maitre,
                                type_maitre,
                                autoroute,
                                PR,
                                sens,
                                nom,
                                site_gestion,
                                eqt_supprime
                              )
                  values ( XDC_EQT_RAD, @vl_Numero + 1, @vl_Maitre, XDC_MAI_RAD, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomStation_in , @vl_Site, @va_Supprime_in)
                  
                  /*A
                  ** Si Erreur d'insertion
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** La station RADT n'existe pas dans la table EQT_RAD
                  ** Insertion dans la table de la station
                  */
            
                  insert into CFT..EQT_RAD
                  values ( @vl_Numero + 1, @va_RGS_in, @va_Liaison_in, @va_DonneesIndividuelles_in, @va_Seuils_in, @va_Alertes_in,
		  		@va_type_in, @va_adresseIP_in, @va_port_in, @va_idLCR_in) 
            
                  /*A
                  ** Si Erreur d'insertion
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end
               
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
               end
            
            end
         
            else
         
            /*A
            ** Le serveur n'existe pas comme equipement
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         end
      
         else      
      
         /*A
         ** L'autoroute n'existe pas comme equipement
         **
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         
            select @va_Resultat_out = XDC_ECHEC
            
      end


#endif      
   
   end
   end
      
   return XDC_OK
         
go  
