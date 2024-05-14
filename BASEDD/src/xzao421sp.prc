/*E*/
/* Fichier : $Id: xzao421sp.prc,v 1.8 2020/05/19 12:18:14 pc2dpdy Exp $        $Revision: 1.8 $        $Date: 2020/05/19 12:18:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao421sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	21 Jan 1998	: Creation
* JMG   13/11/07	: ajout site de gestion DEM715  1.2
* JMG	23/03/17 	: correction bug requete CFT  1.3
* LCL 	05/07/17	: modif PRA  1.4
* LCL	11/07/18	: correction syntaxe from  1.5
* JPL	19/10/18	: Nommage des colonnes sur 'insert'  1.6
* JPL	07/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.7
* LCL	11/06/19	: PAL/IP DEM1337 1.8
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao421sp.h" */

/* static char *version = "@(#)xzao421.c	1.1 23/18/94 : xzao421" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un PAL et d'un Picto dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO421' and type = 'P')
	drop procedure XZAO421
go


create procedure XZAO421
   @va_Basedd_in        	char(3),
   @va_NomPAL_in		char(10),
   @va_NomAutoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_NomServeur_in		char(25),
   @va_LiaisonPAL_in		tinyint,
   @va_RgsPAL_in		char(3),
   @va_TypePAL_in		tinyint,
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_Adresse_IP_in		char(20),
   @va_Port_in			int,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO421SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomPAL_in
* XDY_Nom       va_NomAutoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonPAL_in
* XDY_Rgs       va_RgsPAL_in
* XDY_Octet     va_TypePAL_in
* XDY_NomSite   va_NomSite_in
* XDY_Booleen	va_Supprime_in
* XDY_AdresseIP va_Adresse-IP_in
* XDY_Port	va_Port_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PAL et un Picto en base
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
* Insertion ou modification dans les tables EQT_PAL (C_PAL), et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Serveur		smallint,
   @vl_Machine		smallint,
   @vl_NumPAL		smallint,
   @vl_TypePAL		tinyint,
   @vl_Numero		smallint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomPAL_in = null 
   or @va_NomAutoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_NomServeur_in = null 
   or @va_LiaisonPAL_in = null 
   or @va_RgsPAL_in = null 
   or @va_TypePAL_in = null
   or @va_NomSite_in = null
   or @va_Supprime_in = null
   or @va_Adresse_IP_in = null
   or @va_Port_in = null
   
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
         ** que du serveur associe au PAL
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
         ** l'autoroute et le serveur associe au PAL existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PAL
            ** Verification de l'existence et recherche du type PAL
            */
         
            select @vl_NumPAL = numero from CFG..EQT_GEN
            where type	= XDC_EQT_PAL
                  and nom = @va_NomPAL_in
         
              
            /*A
             ** Le PAL n'existe pas
             */
         
             if @vl_NumPAL = null
             begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PAL
                     */
            
                     select @vl_Numero = max(numero) from CFG..EQT_GEN
	       where type = XDC_EQT_PAL
              
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
                     values ( XDC_EQT_PAL, @vl_Numero + 1, @vl_Serveur, XDC_MAI_PAL, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomPAL_in, @vl_Site, @va_Supprime_in)  
            
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
                     ** Insertion dans la table PAL du nouveau PAL
                     */
            
                     insert into CFG..EQT_PAL (numero, type, liaison_serie, rgs, adresse_ip, port)
                     values ( @vl_Numero + 1, @va_TypePAL_in, @va_LiaisonPAL_in, @va_RgsPAL_in, @va_Adresse_IP_in, @va_Port_in ) 
            
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
                  end /* Fin Le PAL n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PAL existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PAL
                    */
            
                    update CFG..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_MAI_PAL,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomPAL_in,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                    where numero = @vl_NumPAL
                      and type   = XDC_EQT_PAL 
    
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
                    ** Modification dans la table du PAL
                    */
            
                    update CFG..EQT_PAL
    	            set rgs     	= @va_RgsPAL_in,
    	                liaison_serie 	= @va_LiaisonPAL_in,
    	                type            = @va_TypePAL_in,
			adresse_ip      = @va_Adresse_IP_in,
			port            = @va_Port_in
                    where numero = @vl_NumPAL
    
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
               
                 end /* Fin Le PAL existe deja */
         
         
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
         ** que du serveur associe au PAL
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
         ** l'autoroute et le serveur associe au PAL existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PAL
            ** Verification de l'existence et recherche de l'equipement PAL
            */
         
            select @vl_NumPAL = numero from CFT..EQT_GEN
            where type	= XDC_EQT_PAL
                      and nom	= @va_NomPAL_in
         
              
            /*A
             ** Le PAL n'existe pas
             */
         
            if @vl_NumPAL = null
            begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PAL
                     */
            
                     select @vl_Numero = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_PAL
              
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
                     values ( XDC_EQT_PAL, @vl_Numero + 1, @vl_Serveur, XDC_MAI_PAL, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomPAL_in, @vl_Site, @va_Supprime_in) 
            
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
                     ** Insertion dans la table PAL du nouveau PAL
                     */
            
                     insert into CFT..EQT_PAL (numero, type, liaison_serie, rgs, adresse_ip, port)
                     values ( @vl_Numero + 1, @va_TypePAL_in, @va_LiaisonPAL_in, @va_RgsPAL_in, @va_Adresse_IP_in, @va_Port_in ) 
            
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
                  end /* Fin Le PAL n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PAL existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PAL
                    */
            
                    update CFT..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_MAI_PAL,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomPAL_in,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                    where numero = @vl_NumPAL
                      and type   = XDC_EQT_PAL 
    
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
                    ** Modification dans la table du PAL
                    */
            
                    update CFT..EQT_PAL
    	            set rgs     	= @va_RgsPAL_in,
    	                liaison_serie 	= @va_LiaisonPAL_in,
    	                type            = @va_TypePAL_in,
			adresse_ip      = @va_Adresse_IP_in,
			port		= @va_Port_in
                    where numero = @vl_NumPAL
    
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
               
                 end /* Fin Le PAL existe deja */
         
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
