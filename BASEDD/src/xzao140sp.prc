/*E*/
/* Fichier : $Id: xzao140sp.prc,v 1.9 2018/10/19 15:04:55 devgfi Exp $        $Revision: 1.9 $        $Date: 2018/10/19 15:04:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao140sp.prc
*------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* volcic	31 Oct 1995	: Modif update EQT_MNT (v1.3)
* mismer	07 Mai 1996	: Suppression recherche ds table EQT_MAA (DEM/977)   version  1.5
* JMG   	13/11/07	: ajout site de gestion DEM715  1.6
* LCL		11/07/18	: correction syntaxe from  1.8
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.9
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao140sp.h" */

/* static char *version = "@(#)xzao140.c	1.1 20/09/94 : xzao140" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un moniteur dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;140
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_Code_in			char(3),
   @va_NomSysVideo_in		char(10),
   @va_MachineOperateur_in	char(10),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO140SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_CodeEqt		va_Code_in	
* XDY_NomEqt		va_NomSysVideo_in
* XDY_NomEqt		va_MachineOperateur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un moniteur en base
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
* Insertion ou modification dans la table EQT_MNT (MONITEUR)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumLTVideo	smallint,
   @vl_Machine		smallint,
   @vl_NumMoniteur	smallint,
   @vl_NumMon		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_Nom_in = null 
   or @va_Code_in = null 
   or @va_NomSysVideo_in = null
   or @va_MachineOperateur_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence et recherche du systeme video ainsi que de la machine operateurcomme equipement
            */
         
            select @vl_NumLTVideo = numero from CFG..EQT_GEN
              where type = XDC_EQT_SYV
              	and nom  = @va_NomSysVideo_in 
              	    
            select @vl_Machine = numero from CFG..EQT_GEN
                 where type = XDC_EQT_MAC
                   and nom  = @va_MachineOperateur_in
                   
                 
            /*A
            ** Existence du systeme video et de la machine operateur
            */
              
            if @vl_NumLTVideo <> null and @vl_Machine <> null
            begin
                  
               select @vl_NumMoniteur = numero from CFG..EQT_GEN 
                 where type        = XDC_EQT_MNT
              	   and nom         = @va_Nom_in
              	      
               /*A
               ** Le moniteur n'existe pas comme equipement 
               */
              
               if @vl_NumMoniteur = null
               begin
                    
                  /*A
                  ** Insertion du moniteur dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_MNT
                    
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
                                site_gestion
                              )
                  values ( XDC_EQT_MNT, @vl_Numero + 1, @vl_NumLTVideo, XDC_MAI_MNT, null, null, null, @va_Nom_in , null)
                       
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
                  ** Insertion du moniteur dans la table EQT_MNT
                  */
               
                  insert into CFG..EQT_MNT
                  values ( @vl_Numero + 1, @vl_Machine, @va_Code_in )
                       
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
                     
               end
                  
               else
                     
               /*A
               ** Le moniteur existe deja dans EQT_GEN
               */
                  
               begin
           
                  /*A
                  ** Modification du moniteur dans la table EQT_GEN
                  */
                
                  update CFG..EQT_GEN
    	          set maitre = @vl_NumLTVideo,
    	              type_maitre = XDC_MAI_MNT
    	          where numero = @vl_NumMoniteur
    	            and type   = XDC_EQT_MNT
    	            and nom    = @va_Nom_in
                       
                  /*A
                  ** Erreur de modification
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
                  ** Modification du moniteur dans la table EQT_MNT
                  */
                
                  update CFG..EQT_MNT
    	          set code_moniteur = @va_Code_in,
    	              machine_operateur = @vl_Machine
    	          where numero            = @vl_NumMoniteur
                       
                  /*A
                  ** Erreur de modification
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
            end
            
            else
         
            /*A
            ** Le systeme video ou la machine operateur n'existe pas comme equipement
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
         
            /*A
            ** Verification de l'existence et recherche du systeme video ainsi que de la machine operateurcomme equipement
            */
         
            select @vl_NumLTVideo = numero from CFT..EQT_GEN
              where type = XDC_EQT_SYV
              	and nom  = @va_NomSysVideo_in 
              	    
            select @vl_Machine = numero from CFT..EQT_GEN
                 where type = XDC_EQT_MAC
                   and nom  = @va_MachineOperateur_in
                 
            /*A
            ** Existence du systeme video et de la machine operateur
            */
              
            if @vl_NumLTVideo <> null and @vl_Machine <> null
            begin
                  
               select @vl_NumMoniteur = numero from CFT..EQT_GEN 
                 where type        = XDC_EQT_MNT
              	   and nom         = @va_Nom_in
              	      
               /*A
               ** Le moniteur n'existe pas comme equipement 
               */
              
               if @vl_NumMoniteur = null
               begin
                    
                  /*A
                  ** Insertion du moniteur dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_MNT
                    
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
                                site_gestion
                              )
                  values ( XDC_EQT_MNT, @vl_Numero + 1, @vl_NumLTVideo, XDC_MAI_MNT, null, null, null, @va_Nom_in , null)
                       
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
                  ** Insertion du moniteur dans la table EQT_MNT
                  */
               
                  insert into CFT..EQT_MNT
                  values ( @vl_Numero + 1, @vl_Machine, @va_Code_in )
                       
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
                     
               end
                  
               else
                     
               /*A
               ** Le moniteur existe deja dans EQT_GEN
               */
                  
               begin
           
                  /*A
                  ** Modification du moniteur dans la table EQT_GEN
                  */
                
                  update CFT..EQT_GEN
    	          set maitre = @vl_NumLTVideo,
    	              type_maitre = XDC_MAI_MNT
    	          where numero = @vl_NumMoniteur
    	            and type   = XDC_EQT_MNT
    	            and nom    = @va_Nom_in
                       
                  /*A
                  ** Erreur de modification
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
                  ** Modification du moniteur dans la table EQT_MNT
                  */
                
                  update CFT..EQT_MNT
    	          set code_moniteur = @va_Code_in,
    	              machine_operateur = @vl_Machine
    	          where numero            = @vl_NumMoniteur
                       
                  /*A
                  ** Erreur de modification
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
            end
            
            else
         
            /*A
            ** Le systeme video ou la machine operateur n'existe pas comme equipement
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
