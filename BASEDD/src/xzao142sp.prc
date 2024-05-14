/*E*/
/* Fichier : $Id: xzao142sp.prc,v 1.7 2018/10/19 15:06:11 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/10/19 15:06:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao142sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* JMG   	13/11/07	: ajout site de gestion DEM715  1.4
* LCL		11/07/18	: correction syntaxe from  1.6
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.7
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao142sp.h" */

/* static char *version = "@(#)xzao142.c	1.1 20/09/94 : xzao142" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une MAA dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;142
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_NomServeur_in		char(10),
   @va_Liaison_in		tinyint,
   @va_RGS_in			char(3),
   @va_MachineOperateur_in	char(10),
   @va_NumeroRepondeur_in	char(15),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO142SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomEqt		va_NomServeur_in
* XDY_Octet		va_Liaison_in
* XDY_Rgs		va_RGS_in
* XDY_NomEqt		va_MachineOperateur_in
* XDY_NomEqt		va_NumeroRepondeur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une MAA en base
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
* Insertion ou modification dans la table EQT_MAA (MAA)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Machine		smallint,
   @vl_Serveur		smallint,
   @vl_NumMAA		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null   
   or @va_Nom_in = null
   or @va_NomServeur_in = null 
   or @va_Liaison_in = null 
   or @va_RGS_in = null 
   or @va_MachineOperateur_in = null 
   or @va_NumeroRepondeur_in = null
   
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
            ** Verification de l'existence et recherche de la machine operateur , du serveur et de la MAA comme equipement
            */
         
            select @vl_Machine = numero from CFG..EQT_GEN
              where type = XDC_EQT_MAC
              	and nom  = @va_MachineOperateur_in 
            
            select @vl_Serveur = numero from CFG..EQT_GEN
              where type = XDC_EQT_SRV
              	and nom  = @va_NomServeur_in 
            
            /*A
            ** Existence de la machine operateur et du serveur
            */
            
            if @vl_Machine <> null and @vl_Serveur <> null
            begin
            
               /*A
               ** Existence de la MAA
               */
              
               select @vl_NumMAA = numero from CFG..EQT_GEN
                 where type        = XDC_EQT_MAA
              	   and nom         = @va_Nom_in
              	    
               /*A
               ** La MAA n'existe pas comme equipement
               */
               
               if @vl_NumMAA = null
               begin
                  
                  /*A
                  ** Insertion de la MAA dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                   where type = XDC_EQT_MAA
              
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
                  values ( XDC_EQT_MAA, @vl_Numero + 1, @vl_Serveur, XDC_MAI_MAA, null, null, null,@va_Nom_in , null)
                       
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
                  ** Insertion de la MAA dans la table EQT_MAA
                  */
               
                  insert into CFG..EQT_MAA
                  values ( @vl_Numero + 1, @va_Liaison_in, @va_RGS_in, @vl_Machine, @va_NumeroRepondeur_in )
                       
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
               ** La MAA existe deja dans EQT_GEN
               */
                  
               begin
           
                  /*A
                  ** Modification de la MAA dans la table EQT_GEN
                  */
                  
                  update CFG..EQT_GEN
    	          set maitre      = @vl_Serveur,
    	              type_maitre = XDC_MAI_MAA
    	          where numero = @vl_NumMAA
    	            and type   = XDC_EQT_MAA
                       
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
                  ** Modification de la MAA dans la table EQT_MAA
                  */
                  
                  update CFG..EQT_MAA
    	          set liaison_serie    = @va_Liaison_in,
    	              rgs              = @va_RGS_in,
    	              numero_repondeur = @va_NumeroRepondeur_in,
    	              machine_operateur = @vl_Machine
    	          where numero            = @vl_NumMAA
                       
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
            ** La Machine operateur ou le serveur n'existe pas
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
            ** Verification de l'existence et recherche de la machine operateur , du serveur et de la MAA comme equipement
            */
         
            select @vl_Machine = numero from CFT..EQT_GEN
              where type = XDC_EQT_MAC
              	and nom  = @va_MachineOperateur_in 
            
            select @vl_Serveur = numero from CFT..EQT_GEN
              where type = XDC_EQT_SRV
              	and nom  = @va_NomServeur_in 
            
            /*A
            ** Existence de la machine operateur et du serveur
            */
            
            if @vl_Machine <> null and @vl_Serveur <> null
            begin
            
               /*A
               ** Existence de la MAA
               */
              
               select @vl_NumMAA = numero from CFT..EQT_GEN
                 where type        = XDC_EQT_MAA
              	   and nom         = @va_Nom_in
              	    
               /*A
               ** La MAA n'existe pas comme equipement
               */
               
               if @vl_NumMAA = null
               begin
                  
                  /*A
                  ** Insertion de la MAA dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                   where type = XDC_EQT_MAA
              
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
                  values ( XDC_EQT_MAA, @vl_Numero + 1, @vl_Serveur, XDC_MAI_MAA, null, null, null,@va_Nom_in , null)
                       
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
                  ** Insertion de la MAA dans la table EQT_MAA
                  */
               
                  insert into CFT..EQT_MAA
                  values ( @vl_Numero + 1, @va_Liaison_in, @va_RGS_in, @vl_Machine, @va_NumeroRepondeur_in )
                       
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
               ** La MAA existe deja dans EQT_GEN
               */
                  
               begin
           
                  /*A
                  ** Modification de la MAA dans la table EQT_GEN
                  */
                  
                  update CFT..EQT_GEN
    	          set maitre      = @vl_Serveur,
    	              type_maitre = XDC_MAI_MAA
    	          where numero = @vl_NumMAA
    	            and type   = XDC_EQT_MAA
                       
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
                  ** Modification de la MAA dans la table EQT_MAA
                  */
                  
                  update CFT..EQT_MAA
    	          set liaison_serie    = @va_Liaison_in,
    	              rgs              = @va_RGS_in,
    	              numero_repondeur = @va_NumeroRepondeur_in,
    	              machine_operateur = @vl_Machine
    	          where numero            = @vl_NumMAA
                       
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
            ** La Machine operateur ou le serveur n'existe pas
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
