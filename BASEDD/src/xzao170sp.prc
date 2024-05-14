/*E*/
/*Fichier : $Id: xzao170sp.prc,v 1.4 2018/07/11 10:27:59 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 10:27:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao170sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao170sp.h" */

/* static char *version = "@(#)xzao170.c	1.1 20/09/94 : xzao170" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence autorisee d'un tunnel dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;170
   @va_Basedd_in        	char(3),
   @va_NomSequence_in		T_NOM,
   @va_NomTube_in		T_NOM,
   @va_Adresse_in		smallint,
   @va_BitComm_in		tinyint,
   @va_BitDiscordance_in	tinyint,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO170SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomSequence_in
* XDY_Nom		va_NomTube_in
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
* Ajoute une sequence autorisee d'un tunnel en base
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
* Insertion ou modification dans la table EQT_SEQ_TUB (EQUIPEMENT_SEQ_TUB)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumTUB	smallint,
   @vl_NumT		smallint,
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
   or @va_NomTube_in = null
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
            ** Verification de l'existence de la sequence d'un tunnel dans EQT_SEQ_TUB
            ** et du tunnel dans EQT_TUB
            */
         
            select @vl_NumSeq = numero from CFG..EQT_SEQ_TUB
              where nom = @va_NomSequence_in
                
            /*A
            ** La sequence d'un tunnel existe dans EQT_SEQ_TUB
            */
               
            if @vl_NumSeq <> null
            begin
               
               select @vl_NumTUB = numero from CFG..EQT_TUB
                 where numero in
                   (select numero from CFG..EQT_GEN
                      where nom = @va_NomTube_in
                        and type = XDC_EQT_TUB)
                
               /*A
               ** Le tunnel existe dans EQT_TUB
               */
               
               if @vl_NumTUB <> null
               begin
               
                  select @vl_NumT = tube, @vl_NumS = sequence  from CFG..EQT_SQA_TUB
                    where tube     = @vl_NumTUB
                      and sequence = @vl_NumSeq
               
                  /*A
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_TUB
                  */
               
                  if @vl_NumT = null and @vl_NumS = null
                  begin
               
                     /*A
                     ** Creation positionne =>
                     ** Insertion de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
                     */
               
                     if @va_Creation_in = XDC_VRAI
                     begin
               
                        insert into CFG..EQT_SQA_TUB
                        values ( @vl_NumTUB, @vl_NumSeq, @va_Adresse_in, @va_BitComm_in, @va_BitDiscordance_in )
                  
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
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_TUB
                  */
                  
                  begin
                  
                     if @va_Creation_in = XDC_VRAI
                    
                     /*A 
                     ** Creation non positionne =>
                     ** Modification de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
                     */
                    
                     begin
                    
                        update CFG..EQT_SQA_TUB
                        set adresse  = @va_Adresse_in,
                            bit_comm = @va_BitComm_in,
                            bit_disc = @va_BitDiscordance_in
                        where tube     = @vl_NumTUB
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
                     ** Suppression de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
		     */
		     
		     begin
               
                        delete CFG..EQT_SQA_TUB
                        where tube     = @vl_NumTUB
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
               ** Le tunnel n'existe pas 
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
               
                  select @va_Resultat_out = XDC_ECHEC
            end
               
            else
               
            /*A
            ** La sequence du tunnel n'existe pas 
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
            ** Verification de l'existence de la sequence d'un tunnel dans EQT_SEQ_TUB
            ** et du tunnel dans EQT_TUB
            */
         
            select @vl_NumSeq = numero from CFT..EQT_SEQ_TUB
              where nom = @va_NomSequence_in
                
            /*A
            ** La sequence d'un tunnel existe dans EQT_SEQ_TUB
            */
               
            if @vl_NumSeq <> null
            begin
               
               select @vl_NumTUB = numero from CFT..EQT_TUB
                 where numero in
                   (select numero from CFT..EQT_GEN
                      where nom = @va_NomTube_in
                        and type = XDC_EQT_TUB)
                
               /*A
               ** Le tunnel existe dans EQT_TUB
               */
               
               if @vl_NumTUB <> null
               begin
               
                  select @vl_NumT = tube, @vl_NumS = sequence  from CFT..EQT_SQA_TUB
                    where tube     = @vl_NumTUB
                      and sequence = @vl_NumSeq
               
                  /*A
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_TUB
                  */
               
                  if @vl_NumT = null and @vl_NumS = null
                  begin
               
                     /*A
                     ** Creation positionne =>
                     ** Insertion de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
                     */
               
                     if @va_Creation_in = XDC_VRAI
                     begin
               
                        insert into CFT..EQT_SQA_TUB
                        values ( @vl_NumTUB, @vl_NumSeq, @va_Adresse_in, @va_BitComm_in, @va_BitDiscordance_in )
                  
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
                  ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_TUB
                  */
                  
                  begin
                  
                     if @va_Creation_in = XDC_VRAI
                    
                     /*A 
                     ** Creation non positionne =>
                     ** Modification de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
                     */
                    
                     begin
                    
                        update CFT..EQT_SQA_TUB
                        set adresse  = @va_Adresse_in,
                            bit_comm = @va_BitComm_in,
                            bit_disc = @va_BitDiscordance_in
                        where tube     = @vl_NumTUB
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
                     ** Suppression de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
		     */
		     
		     begin
               
                        delete CFT..EQT_SQA_TUB
                        where tube     = @vl_NumTUB
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
               ** Le tunnel n'existe pas 
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
               
                  select @va_Resultat_out = XDC_ECHEC
            end
               
            else
               
            /*A
            ** La sequence du tunnel n'existe pas 
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
               
               select @va_Resultat_out = XDC_ECHEC
                    
         end 
#endif      
      
      end
   end         
     
   return XDC_OK
         
go  
