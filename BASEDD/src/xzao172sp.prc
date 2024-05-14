/*E*/
/*Fichier : $Id: xzao172sp.prc,v 1.4 2018/07/11 10:28:53 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 10:28:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao172sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao172sp.h" */

/* static char *version = "@(#)xzao172.c	1.1 20/09/94 : xzao172" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence autorisee d'un echangeur dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;172
   @va_Basedd_in        	char(3),
   @va_NomSequence_in		T_NOM,
   @va_NomEch_in		T_NOM,
   @va_Adresse_in		smallint,
   @va_BitComm_in		tinyint,
   @va_BitDiscordance_in	tinyint,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO172SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomSequence_in
* XDY_Nom		va_NomEch_in
* XDY_Mot		va_Adresse_in
* XDY_Octet		va_BitComm_in
* XDY_Octet		va_BitDiscordance_in
* XDY_Booleen		@va_Creation_in	
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un echangeur en base
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
* Insertion ou modification dans la table EQT_SEQ_ECH (EQUIPEMENT_SEQ_ECH)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumEch	smallint,
   @vl_NumE		smallint,
   @vl_NumSeq	tinyint,
   @vl_NumS		tinyint,
   @vl_Site		char(10)
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomSequence_in = null 
   or @va_NomSequence_in = null
   or @va_NomEch_in = null
   or @va_Adresse_in = null 
   or @va_BitComm_in = null 
   or @va_BitDiscordance_in = null
   or @va_Creation_in = null 
   
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
            ** Verification de l'existence de la sequence d'un echangeur dans EQT_SEQ_ECH
            ** et du tunnel dans EQT_SEQ_ECH
            */
         
            select @vl_NumSeq = numero from CFG..EQT_SEQ_ECH
              where nom = @va_NomSequence_in
                
            /*A
            ** La sequence d'un echangeur existe dans EQT_SEQ_ECH
            */
               
            if @vl_NumSeq <> null
            begin
               
               select @vl_NumEch = numero from CFG..EQT_ECH
                 where numero in
                   (select numero from CFG..EQT_GEN
                      where nom = @va_NomEch_in
                        and type = XDC_EQT_ECH)
                
               /*A
               ** Le demi-echangeur existe dans EQT_ECH
               */
               
               if @vl_NumEch <> null
               begin
               
                  select @vl_NumE = echangeur, @vl_NumS = sequence  from CFG..EQT_SQA_ECH
                    where echangeur = @vl_NumEch
                      and sequence  = @vl_NumSeq
               
                  /*A
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_ECH
                  */
               
                  if @vl_NumE = null and @vl_NumS = null
                  begin
               
                     /*A
                     ** Creation positionne =>
                     ** Insertion de la sequence autorisee d'un echangeur dans la table EQT_SQA_ECH
                     */
               
                     if @va_Creation_in = XDC_VRAI
                     begin
               
                        insert into CFG..EQT_SQA_ECH
                        values ( @vl_NumEch, @vl_NumSeq, @va_Adresse_in, @va_BitComm_in, @va_BitDiscordance_in )
                  
                        /*A
                        ** Si erreur d'insertion
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
                     ** Creation non positionne =>
                     ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                     */
               
                        select @va_Resultat_out = XDC_ECHEC
                  
                  end
                  
                  else
                 
                  /*A
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_ECH
                  */
                  
                  begin
                  
                     if @va_Creation_in = XDC_VRAI
                    
                     /*A 
                     ** Creation non positionne =>
                     ** Modification de la sequence autorisee d'un echangeur dans la table EQT_SQA_ECH
                     */
                    
                     begin
                    
                        update CFG..EQT_SQA_ECH
                        set adresse  = @va_Adresse_in,
                            bit_comm = @va_BitComm_in,
                            bit_disc = @va_BitDiscordance_in
                        where echangeur     = @vl_NumEch
                          and sequence = @vl_NumSeq   
                           
                        /*A
                        ** Si erreur de modification
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
                     ** Creation non positionne =>
                     ** Suppression de la sequence autorisee d'un echangeur dans la table EQT_SQA_ECH
		     */
		     
		     begin
               
                        delete CFG..EQT_SQA_ECH
                        where echangeur     = @vl_NumEch
                          and sequence = @vl_NumSeq     
            
                        /*A
                        ** Si erreur de suppression
                        */
                     
                        if @@rowcount <> 1
                        begin
                     
                           /*A
                           ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                           */
         
                           select @va_Resultat_out = XDC_SUPPR
                          
                           return XDC_NOK
                        end
                        
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                        */
         
                        select @va_Resultat_out = XDC_SUPPR
                     end
                  end
               end  
                  
               else
                  
               /*A
               ** Le demi-echengeur n'existe pas 
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
               
                  select @va_Resultat_out = XDC_ECHEC
            end
               
            else
               
            /*A
            ** La sequence de l'echangeur n'existe pas 
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
            ** Verification de l'existence de la sequence d'un echangeur dans EQT_SEQ_ECH
            ** et du tunnel dans EQT_SEQ_ECH
            */
         
            select @vl_NumSeq = numero from CFT..EQT_SEQ_ECH
              where nom = @va_NomSequence_in
                
            /*A
            ** La sequence d'un echangeur existe dans EQT_SEQ_ECH
            */
               
            if @vl_NumSeq <> null
            begin
               
               select @vl_NumEch = numero from CFT..EQT_ECH
                 where numero in
                   (select numero from CFT..EQT_GEN
                      where nom = @va_NomEch_in
                        and type = XDC_EQT_ECH)
                
               /*A
               ** Le demi-echangeur existe dans EQT_ECH
               */
               
               if @vl_NumEch <> null
               begin
               
                  select @vl_NumE = echangeur, @vl_NumS = sequence  from CFT..EQT_SQA_ECH
                    where echangeur = @vl_NumEch
                      and sequence  = @vl_NumSeq
               
                  /*A
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_ECH
                  */
               
                  if @vl_NumE = null and @vl_NumS = null
                  begin
               
                     /*A
                     ** Creation positionne =>
                     ** Insertion de la sequence autorisee d'un echangeur dans la table EQT_SQA_ECH
                     */
               
                     if @va_Creation_in = XDC_VRAI
                     begin
               
                        insert into CFT..EQT_SQA_ECH
                        values ( @vl_NumEch, @vl_NumSeq, @va_Adresse_in, @va_BitComm_in, @va_BitDiscordance_in )
                  
                        /*A
                        ** Si erreur d'insertion
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
                     ** Creation non positionne =>
                     ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                     */
               
                        select @va_Resultat_out = XDC_ECHEC
                  
                  end
                  
                  else
                 
                  /*A
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_ECH
                  */
                  
                  begin
                  
                     if @va_Creation_in = XDC_VRAI
                    
                     /*A 
                     ** Creation non positionne =>
                     ** Modification de la sequence autorisee d'un echangeur dans la table EQT_SQA_ECH
                     */
                    
                     begin
                    
                        update CFT..EQT_SQA_ECH
                        set adresse  = @va_Adresse_in,
                            bit_comm = @va_BitComm_in,
                            bit_disc = @va_BitDiscordance_in
                        where echangeur     = @vl_NumEch
                          and sequence = @vl_NumSeq   
                           
                        /*A
                        ** Si erreur de modification
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
                     ** Creation non positionne =>
                     ** Suppression de la sequence autorisee d'un echangeur dans la table EQT_SQA_ECH
		     */
		     
		     begin
               
                        delete CFT..EQT_SQA_ECH
                        where echangeur     = @vl_NumEch
                          and sequence = @vl_NumSeq     
            
                        /*A
                        ** Si erreur de suppression
                        */
                     
                        if @@rowcount <> 1
                        begin
                     
                           /*A
                           ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                           */
         
                           select @va_Resultat_out = XDC_SUPPR
                          
                           return XDC_NOK
                        end
                        
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                        */
         
                        select @va_Resultat_out = XDC_SUPPR
                     end
                  end
               end  
                  
               else
                  
               /*A
               ** Le demi-echengeur n'existe pas 
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
               
                  select @va_Resultat_out = XDC_ECHEC
            end
               
            else
               
            /*A
            ** La sequence de l'echangeur n'existe pas 
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
               
               select @va_Resultat_out = XDC_ECHEC
                   
         end      
      
#endif      
      
      end
   end         
     
   return XDC_OK
         
go  
