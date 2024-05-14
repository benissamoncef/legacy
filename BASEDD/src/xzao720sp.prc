/*E*/
/* Fichier : $Id: xzao720sp.prc,v 1.10 2019/02/06 19:05:31 devgfi Exp $        $Revision: 1.10 $        $Date: 2019/02/06 19:05:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao720sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* JMG   13/11/07 : ajout site de gestion DEM715
* JMG	23/03/17 : correction CFT/CFG
* LCL	05/07/17 : Modif PRA
* LCL	11/07/18 : correction syntaxe from  1.5
* JMG	02/10/18 : ajout IP suppression SAGA DEM1306 1.6
* JPL	17/10/18 : Nommage des colonnes sur 'insert'  1.7
* LCL	22/10/18 : Ajout Id LCR DEM1306 1.8
* JMG	29/01/19 : ajout type 1.9
* JPL	06/02/19 : Ajout du parametre 'supprime' (DEM 1305)  1.10
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao720sp.h" */

/* static char *version = ""$Id: xzao720sp.prc,v 1.10 2019/02/06 19:05:31 devgfi Exp $ $Revision: 1.10 $ $Date: 2019/02/06 19:05:31 $ : xzao720" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un PMVA dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO720' and type = 'P')
	drop procedure XZAO720
go

create procedure XZAO720
   @va_Basedd_in        	char(3),
   @va_NomPMVA_in		char(10),
   @va_NomAutoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_NomServeur_in		char(25),
   @va_LiaisonPMVA_in		tinyint,
   @va_RgsPMVA_in		char(3),
   @va_NomSite_in		char(2),
      @va_AdresseIP_in             char(21),
      @va_PortIP_in                int,
      @va_IdLCR_in		char(50),
   @va_Resultat_out		int output,
   @va_Type_in			tinyint,
   @va_Supprime_in		bit
   as 

/* 
*
* SP	XZAO720SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomPMVA_in
* XDY_NomAuto   va_NomAutoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonPMVA_in
* XDY_Rgs       va_RgsPMVA_in
* XDY_NomSite   va_NomSite_in
* XDY_AdresseIP va_AdresseIP_in
* XDY_Entier    va_PortIP_in
* char		va_IdLCR[50]
* XDY_Octet     va_Type_in
* XDY_Booleen   va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PMVA en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans les tables EQT_PMVA (C_PMVA) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Serveur		smallint,
   @vl_Machine		smallint,
   @vl_NumPMVA		smallint,
   @vl_Numero		smallint,
   @vl_NumPct		smallint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomPMVA_in = null 
   or @va_NomAutoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_NomServeur_in = null 
   or @va_LiaisonPMVA_in = null 
   or @va_RgsPMVA_in = null 
   or @va_NomSite_in = null
      or @va_AdresseIP_in = null
         or @va_PortIP_in = null
         or @va_Type_in = null
	 or @va_IdLCR_in = null
   or @va_Supprime_in = null
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
         
         /*A
         ** Mise a jour de la table EQT_GEN 
         */
         
         /*B
         ** Verification de l'existence et recherche du numero de l'autoroute ainsi
         ** que du serveur associe au PMVA
         */
         
         select @vl_Autoroute = numero from CFG..RES_AUT
           where nom = @va_NomAutoroute_in
           
         select @vl_Serveur = numero from CFG..EQT_GEN
           where nom  = @va_NomServeur_in
             and type = XDC_EQT_SRV
          
       select @vl_Site = numero from CFG..RES_DIS
       where code = @va_NomSite_in

	   if @vl_Site = NULL
	   return XDC_ARG_INV

         /*A
         ** l'autoroute et le serveur associe au PMVA existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PMVA
            */
                          
                  /*B
                  ** Verification de l'existence et Recherche de l'equipement PMVA
                  */
         
                  select @vl_NumPMVA = numero from CFG..EQT_GEN
                    where type	= XDC_EQT_PMVA
                      and nom	= @va_NomPMVA_in
         
              
                  /*A
                  ** Le PMVA n'existe pas
                  */
         
                  if @vl_NumPMVA = null
                  begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PMVA
                     */
            
                     select @vl_Numero = max(numero) from CFG..EQT_GEN
                       where type = XDC_EQT_PMVA
              
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
                     values ( XDC_EQT_PMVA,
                              @vl_Numero + 1,
                              @vl_Serveur,
                              XDC_MAI_PMVA,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_NomPMVA_in,
                              @vl_Site,
                              @va_Supprime_in
                            )
            
                     /*A
                     ** Erreur d'insertion
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
                     ** Insertion dans la table PMVA du nouveau PMVA
                     */
            
                     insert into CFG..EQT_PMVA (numero, liaison_serie, rgs, adresse_ip, port, type)
                     values ( @vl_Numero + 1, @va_LiaisonPMVA_in, @va_RgsPMVA_in ,  @va_AdresseIP_in, @va_PortIP_in, @va_Type_in)
            
                     /*A
                     ** Erreur d'insertion
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
                  end /* Fin Le PMVA n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PMVA existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PMVA
                    */
            
                    update CFG..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_MAI_PMVA,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomPMVA_in,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                    where numero = @vl_NumPMVA
                      and type   = XDC_EQT_PMVA 
    
                    /*A
                    ** Erreur de modificaion
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
                    ** Modification dans la table du PMVA
                    */
            
                    update CFG..EQT_PMVA
    	            set rgs     	= @va_RgsPMVA_in,
    	                liaison_serie 	= @va_LiaisonPMVA_in,
			adresse_ip      = @va_AdresseIP_in,
                        port            = @va_PortIP_in,
			id_lcr		= @va_IdLCR_in,
			type		= @va_Type_in
                    where numero = @vl_NumPMVA
    
                    /*A
                    ** Erreur de modificaion
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
               
                 end /* Fin Le PMVA existe deja */
         

         
 
         
         end /* Fin test sur autoroute et serveur */
         else   
         /*A
         ** L'autoroute ou le serveur n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         begin
         
           select @va_Resultat_out = XDC_ECHEC
	   return XDC_OK
	 end
      
      end /* Fin test base CFG */

#ifdef CI
      
       else   
      /*A
      **  Travail dans la base CFT
      */   
      
      begin
         
         /*A
         ** Mise a jour de la table EQT_GEN 
         */
         
         /*B
         ** Verification de l'existence et recherche du numero de l'autoroute ainsi
         ** que du serveur associe au PMVA
         */
         
         select @vl_Autoroute = numero from CFT..RES_AUT
           where nom = @va_NomAutoroute_in
           
         select @vl_Serveur = numero from CFT..EQT_GEN
           where nom  = @va_NomServeur_in
             and type = XDC_EQT_SRV
          
	     select @vl_Site = numero from CFT..RES_DIS
	     where code = @va_NomSite_in

	 if @vl_Site = NULL
	 return XDC_ARG_INV

         /*A
         ** l'autoroute et le serveur associe au PMVA existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PMVA
            */
            
            /*B
            ** Verification de l'existence et recherche du type PMVA
            */

              
                  /*B
                  ** Verification de l'existence et Recherche de l'equipement PMVA
                  */
         
                  select @vl_NumPMVA = numero from CFT..EQT_GEN
                    where type	= XDC_EQT_PMVA
                      and nom	= @va_NomPMVA_in
         
              
                  /*A
                  ** Le PMVA n'existe pas
                  */
         
                  if @vl_NumPMVA = null
                  begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PMVA
                     */
            
                     select @vl_Numero = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_PMVA
              
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
                     values ( XDC_EQT_PMVA,
                              @vl_Numero + 1,
                              @vl_Serveur,
                              XDC_MAI_PMVA,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_NomPMVA_in,
                              @vl_Site,
                              @va_Supprime_in
                            )
            
                     /*A
                     ** Erreur d'insertion
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
                     ** Insertion dans la table PMVA du nouveau PMVA
                     */
            
                     insert into CFT..EQT_PMVA (numero, liaison_serie, rgs, adresse_ip, port, type)
                     values ( @vl_Numero + 1, @va_LiaisonPMVA_in, @va_RgsPMVA_in ,  @va_AdresseIP_in, @va_PortIP_in, @va_Type_in) 
            
                     /*A
                     ** Erreur d'insertion
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
                  end /* Fin Le PMVA n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PMVA existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PMVA
                    */
            
                    update CFT..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_MAI_PMVA,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomPMVA_in,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                    where numero = @vl_NumPMVA
                      and type   = XDC_EQT_PMVA 
    
                    /*A
                    ** Erreur de modificaion
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
                    ** Modification dans la table du PMVA
                    */
            
                    update CFT..EQT_PMVA
    	            set rgs     	= @va_RgsPMVA_in,
    	                liaison_serie 	= @va_LiaisonPMVA_in,
			adresse_ip      = @va_AdresseIP_in,
                        port            = @va_PortIP_in,
			id_lcr		= @va_IdLCR_in,
			type		= @va_Type_in
                    where numero = @vl_NumPMVA
    
                    /*A
                    ** Erreur de modificaion
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
               
                 end /* Fin Le PMVA existe deja */
         
         

         
         end /* Fin test sur autoroute et serveur */
         else   
         /*A
         ** L'autoroute ou le serveur n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         begin
           select @va_Resultat_out = XDC_ECHEC
	   return XDC_OK
	 end
         
      end /* fin test base CFT */
#endif
       
    end /* fin verification base CI */
  end  /* fin arguments valides */ 
  return XDC_OK
         
go  
